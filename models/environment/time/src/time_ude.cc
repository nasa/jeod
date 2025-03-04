/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_ude.cc
 * Define member functions for those times with a User-Defined-Epoch.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_ude.cc)
   (time.cc)
   (time_dyn.cc)
   (time_manager.cc)
   (time_manager_init.cc)
   (time_messages.cc)
   (time_standard.cc)
   (utils/message/src/message_handler.cc))


******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/time_converter.hh"
#include "../include/time_dyn.hh"
#include "../include/time_manager.hh"
#include "../include/time_manager_init.hh"
#include "../include/time_messages.hh"
#include "../include/time_standard.hh"
#include "../include/time_ude.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Returns false in response to the question "does this time class have
 * to be a singleton"
 *
 * \par Assumptions and Limitations
 *  - There can be more than one UDE
 * @return false
 */
bool TimeUDE::must_be_singleton()
{
    return false;
}

/**
 * Adds a UDE type to the initialization tree when it is appropriate to
 * do so.
 *
 * \par Assumptions and Limitations
 *  - The time type from which the UDE updates must be in the tree
 *           above the UDE.
 *  - If the time type in which the epoch is defined is another UDE, it
 *           also must be in the tree above this UDE
 *  - This function is only called when the UDE is NOT being used to
 *           initialize the simulation.
 * \param[in] seeking_status An indicator of relative level of progression in the tree.
 * \param[in] time_manager_init The TM initializer.
 */
void TimeUDE::add_type_initialize(const int seeking_status, TimeManagerInit * time_manager_init)
{
    int conv_index; // -- index in the time_manager_inits register of converters.

    // Make sure that the update_from value is defined.  UDEs must tick with a STD
    // time-type, or from Dyn, and must update with that time.
    verify_update();

    // verify that the epoch value is consistent.
    verify_epoch();

    //  update_from must already be in the initialization tree.
    // verify that this is not a loop
    if(time_manager_init->get_status(update_index) == -1)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "In defining the tree for initializing the time-type, an infinite loop\n"
                             "was encountered at time type %s.  This time type is based on %s,\n"
                             "which in turn depends on %s for its initialization.\n",
                             name.c_str(),
                             update_from_name.c_str(),
                             name.c_str());
    }

    // check that a converter exists to do the updating
    conv_index = update_index * time_manager->num_types + index;
    if(time_manager_init->get_conv_dir_init(conv_index) == 0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "An attempt was made to define time-type '%s'\n"
                             "as the basis for '%s', but no converter was provided\n"
                             "to convert from '%s' to '%s' at initialization.  \n",
                             update_from_name.c_str(),
                             name.c_str(),
                             update_from_name.c_str(),
                             name.c_str());
    }

    // if it is not in the tree, add it (recursive addition).
    if(time_manager_init->get_status(update_index) == 0)
    {
        time_manager->get_time_ptr(update_index)->add_type_initialize(seeking_status, time_manager_init);

        // if it cannot be added into the tree at this time, exit.
        if(time_manager_init->get_status(update_index) == 0)
        {
            time_manager_init->set_status(index, 0);
            return;
        }
    }

    if((epoch_index >= 0) && (epoch_index != update_index))
    {
        // if epoch is a UDE, could get circularity in initialization, so "this"
        // must be initialized after epoch.
        auto * epoch_ptr_ude = dynamic_cast<TimeUDE *>(time_manager->get_time_ptr(epoch_index));
        if(epoch_ptr_ude != nullptr)
        {
            // verify that this is not a loop
            if(time_manager_init->get_status(epoch_index) == -1)
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::invalid_setup_error,
                                     "\n"
                                     "In defining the tree for initializing the time-type, an infinite\n"
                                     "loop was encountered at time type %s.  This time type has an epoch\n"
                                     "which in turn depends on %s for its initialization.\n",
                                     name.c_str(),
                                     epoch_defined_in_name.c_str(),
                                     name.c_str());
            }

            // if it is not in the tree, add it (recursive addition).
            if(time_manager_init->get_status(epoch_index) == 0)
            {
                time_manager->get_time_ptr(epoch_index)->add_type_initialize(seeking_status, time_manager_init);
            }

            // if it cannot be added into the tree at this time, exit.
            if(time_manager_init->get_status(epoch_index) == 0)
            {
                time_manager_init->set_status(index, 0);
                return;
            }
        }

        // there must be a converter to do the conversion from "epoch" to "update"
        conv_index = epoch_index * time_manager->num_types + update_index;
        if(time_manager_init->get_conv_dir_init(conv_index) == 0)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::incomplete_setup_error,
                                 "\n"
                                 "An attempt was made to define a UDE time-type '%s' \n"
                                 "based on time-type %s, and \n"
                                 "with epoch as %s. \n"
                                 "However, no converter was available to convert from %s to %s at "
                                 "initialization.\n",
                                 name.c_str(),
                                 update_from_name.c_str(),
                                 epoch_defined_in_name.c_str(),
                                 epoch_defined_in_name.c_str(),
                                 update_from_name.c_str());
        }
    }

    //  Already returned if cannot be added, failed at errors.
    //  Get here only if the type is ready to be added.
    time_manager_init->increment_status(index, update_index);
}

/**
 * Converts the time, as specified in the epoch time-type to the
 * update_from time-type.  This sets the update_from time at the epoch
 * of "this", and allows for the initialization of the converter.
 *
 * \par Assumptions and Limitations
 *  - That there is a converter available to do this in one step
 *  - Future work may include an extension to this routine to cover
 *           other cases.
 * \param[in] epoch_ptr pointer to the epoch time-type
 * \param[in] update_from_ptr pointer to the time-type from which this time-type will be updated.
 * \param[in] time_manager_init The TM initializer.
 */
void TimeUDE::convert_epoch_to_update(JeodBaseTime * epoch_ptr,
                                      JeodBaseTime * update_from_ptr,
                                      TimeManagerInit * time_manager_init)
{
    // Unique converter index formula
    int conv_index = epoch_index * time_manager->num_types + update_index;
    int conv_dir = time_manager_init->get_conv_dir_init(conv_index);

    // determine whether direct converter exists from epoch to parent.
    if(conv_dir == 0)
    { // impossible error, bad initialization
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "In initializing the epoch of time type %s, it was necessary to \n"
                             "convert from %s to %s. %s is initialized from %s and takes epoch "
                             "from %s. No converter was made available to \n"
                             "perform this task.\n",
                             name.c_str(),
                             epoch_defined_in_name.c_str(),
                             update_from_name.c_str(),
                             name.c_str(),
                             update_from_name.c_str(),
                             epoch_defined_in_name.c_str());
    }

    TimeConverter * converter_ptr = time_manager->get_converter_ptr(time_manager_init->get_conv_ptr_index(conv_index));

    // For safety, unless the epoch is a UDE (in which case the initializer
    // between them must already be initialized) re-initialize the converter so
    // that it is completely valid at the epoch time.  Then, flag it as being
    // uninititalized so that it will be re-initialized (again) at the
    // sim-start-time if it is needed at the update tree build.
    auto * epoch_ptr_ude = dynamic_cast<TimeUDE *>(epoch_ptr);
    // If the being converted is not a UDE time
    if(epoch_ptr_ude == nullptr)
    {
        update_from_ptr->override_initialized(false);
        converter_ptr->initialize(epoch_ptr, update_from_ptr, conv_dir);
        converter_ptr->override_initialized(false);
    }

    // overwrite the parent values (don't worry, they are recorded elsewhere)
    if(conv_dir == 1)
    {
        converter_ptr->convert_a_to_b();
    }
    else
    {
        converter_ptr->convert_b_to_a();
    }
}

/**
 * Initializes this time-type
 *
 * \par Assumptions and Limitations
 *  - The subject object has a parent, a time-type with which it ticks.
 *            This has already been tested for.
 * \param[in] time_manager_init The TM initializer.
 */
void TimeUDE::initialize_from_parent(TimeManagerInit * time_manager_init)
{
    int conv_index = 0; // -- index of converter in time_manager_inits register.
    int conv_dir = 0;   // -- direction in which to apply converter.
    TimeConverter * converter_ptr = nullptr;
    double temp_ppp_secs = 0; // -- for storing parent values
    bool revert_parent = false;

    //  Verify that the initialization sequence is appropriate, and assign the
    //  initial values.
    verify_init();
    initialized = true;

    if(!initializing_data_present && !epoch_data_present)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "Typically, a UDE is initialized by setting its value at sim-start,\n "
                             "or by defining an epoch, and calculating its value at sim-start "
                             "from that epoch.\n"
                             "Both values for time-type %s are currently zero.  \n"
                             "This could be intentional or could be an oversight.\n"
                             "Simulation will continue with these values",
                             name.c_str());
    }

    // still need to initialize the converter between "update_from" and "this"
    // That could necessitate epoch considerations.

    JeodBaseTime * update_from_ptr = time_manager->get_time_ptr(update_index);

    if(!update_from_ptr->is_initialized())
    {
        update_from_ptr->initialize_from_parent(time_manager_init);
    }

    // Can initialize by initial value, or by epoch value.  If epoch-type is set,
    // assume that initialization is being done by epoch-value.
    if(epoch_index != -2)
    {
        // EPOCH TYPE IS SET
        // Will be initializing by epoch

        // if epoch-type and initial-values are both set, it is unclear which
        // to use, so terminate.
        if(initializing_data_present)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::redundancy_error,
                                 "\n"
                                 "The time type %s is overconstrained.\n"
                                 "The simulation start time is specified alsewhere; both the epoch of \n"
                                 "this type, and the time-since-epoch at simulation start are defined.\n"
                                 "One of these values is redundant (sim_start = epoch + time_since_epoch"
                                 ")\nand the combination may be inconsistent.\n"
                                 "Remove the specification on either the initial value, or on the "
                                 "epoch-type\n",
                                 name.c_str());
        }

        // Store off the values of the parent type.
        temp_ppp_secs = update_from_ptr->seconds;

        // Have to set value of "parent" at epoch in order to initialize the
        // parent->this converter.
        // Need to convert "epoch" to "this", using one of 2 cases:
        //   1. "epoch" and "parent" are the same thing
        //   2. "parent" value must be set first, then "parent" takes on the role of
        //      "epoch".
        if(epoch_index == update_index)
        {
            set_epoch_times(update_from_ptr);
        }
        else
        {
            // Define the epoch.
            JeodBaseTime * epoch_ptr = time_manager->get_time_ptr(epoch_index);

            // Must have the epoch-parent converter established here before setting
            // epoch times if epoch is a UDE.
            auto * epoch_ptr_ude = dynamic_cast<TimeUDE *>(epoch_ptr);
            if(epoch_ptr_ude != nullptr)
            { // if epoch is a UDE
                conv_index = epoch_index * time_manager->num_types + update_index;
                converter_ptr = time_manager->get_converter_ptr(time_manager_init->get_conv_ptr_index(conv_index));
                if(!converter_ptr->is_initialized())
                {
                    if(!epoch_ptr->is_initialized() || !update_from_ptr->is_initialized())
                    {
                        MessageHandler::fail(__FILE__,
                                             __LINE__,
                                             TimeMessages::initialization_error,
                                             "\n"
                                             "Error in building initialization-tree.  Type %s requires\n"
                                             "types %s and %s, but attempted to initialize %s before %s and\n"
                                             "%s had been initialized.\n",
                                             name.c_str(),
                                             epoch_ptr->name.c_str(),
                                             update_from_ptr->name.c_str(),
                                             name.c_str(),
                                             epoch_ptr->name.c_str(),
                                             update_from_ptr->name.c_str());
                    }
                    conv_dir = time_manager_init->get_conv_dir_init(conv_index);
                    converter_ptr->initialize(epoch_ptr, update_from_ptr, conv_dir);
                }
            }

            // If epoch is already initialized, those values are going to
            // be overridden; if not, it will have to appear as though it is in order
            // to initialize the converters.
            // Either way, this has to be corrected later.
            double temp_eee_secs = 0; // -- for storing epoch values.
            bool revert_epoch;
            if(epoch_ptr->is_initialized())
            {
                revert_epoch = true;
                temp_eee_secs = epoch_ptr->seconds;
            }
            else
            {
                revert_epoch = false;
            }

            set_epoch_times(epoch_ptr);

            convert_epoch_to_update(epoch_ptr, update_from_ptr, time_manager_init);

            update_from_ptr->override_initialized(true);

            // revert epoch values if necessary
            if(revert_epoch)
            {
                epoch_ptr->set_time_by_seconds(temp_eee_secs);
            }
            else
            {
                epoch_ptr->override_initialized(false);
            }
        }

        // Continuing with this case (CASE #2), also pick up CASE #1
        // In both cases, will need to revert the parent / update type
        revert_parent = true;

        // For initializing the converter at epoch, enforce "this" time to zero:
        seconds = 0;
        days = 0;
    }

    // Also pick up the case in which this is being initialized using initial
    // values, rather than an epoch definition.
    else
    {
        revert_parent = false; // parent retains its current setting.
    }

    // Ensure there is a converter available from "update" to "this".
    conv_index = update_index * time_manager->num_types + index;
    conv_dir = time_manager_init->get_conv_dir_init(conv_index);
    if(conv_dir == 0)
    { // converter does not exist.  Redundant check,
        // impossible error.
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "In initializing the epoch of time type %s, it was necessary to \n"
                             "convert from %s, because that was defined as the update_from "
                             "time type. No converter was made available to \n"
                             "perform this task.\n",
                             name.c_str(),
                             update_from_name.c_str());
    }

    converter_ptr = time_manager->get_converter_ptr(time_manager_init->get_conv_ptr_index(conv_index));

    converter_ptr->initialize(update_from_ptr, this, conv_dir);

    if(revert_parent)
    { // parent currently has epoch value in it, change back to
      // sim-start value and propagate to "this".

        // NOTE - temp_ppp_secs is valid.  In order to be here, revert_parent is
        // true; if revert_parent is true, the first branch of above if-block was
        // processed; first branch includes assignment to temp_ppp_secs.
        update_from_ptr->set_time_by_seconds(temp_ppp_secs);
        // initialize "this" with the reverted values of "update_from"
        if(conv_dir == 1)
        {
            converter_ptr->convert_a_to_b();
        }
        else
        {
            converter_ptr->convert_b_to_a();
        }
    }
    initial_value = seconds;
}

/**
 * Each time type is initialized from its parent in the initialization
 * tree, except one.  In order to have an absolute reference time, one of
 * the time types must be defined ahead of time.  This is called the
 * initializer time. This function initializes the initializer time.
 * \param[in] time_manager_init The TM initializer.
 */
void TimeUDE::initialize_initializer_time(TimeManagerInit * time_manager_init)
{
    bool updates_from_dyn = false;

    // Make sure that the update_from value is defined.  UDEs must tick with a STD
    // time-type, or from Dyn, and must update with that time.
    verify_update();

    JeodBaseTime * update_from_ptr = time_manager->get_time_ptr(update_index);
    if(dynamic_cast<TimeDyn *>(update_from_ptr) != nullptr)
    {
        updates_from_dyn = true;
    }

    // Make sure that the initial values are not problematic.
    if(time_manager_init->sim_start_format != initial_value_format)
    {
        if(time_manager_init->sim_start_format == TimeEnum::undefined)
        {
            MessageHandler::inform(__FILE__,
                                   __LINE__,
                                   TimeMessages::duplicate_methods,
                                   "\n"
                                   "Initializing the simulation from time-type %s, which is a UDE type.\n"
                                   "Using 'initial_value_format' as the sim-start_format.\n",
                                   name.c_str());
        }
        else if(initial_value_format == TimeEnum::undefined)
        {
            MessageHandler::inform(__FILE__,
                                   __LINE__,
                                   TimeMessages::duplicate_methods,
                                   "\n"
                                   "Initializing the simulation from time-type %s, which is a UDE type.\n"
                                   "Using the defined sim-start_format to initialize this time-type.\n",
                                   name.c_str());
            initial_value_format = time_manager_init->sim_start_format;
        }
        else
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::invalid_setup_error,
                                 "\n"
                                 "Initializing the simulation from time-type %s, which is a UDE type.\n"
                                 "Both the time-type's 'initial_value_format' and the simulation's \n"
                                 "'sim_start_format' were defined, but are different.  Since the\n"
                                 "initialization of this time initializes the simulation, it is \n"
                                 "unclear how the user intends to initialize the simulation.\n",
                                 name.c_str());
        }
    }

    verify_init();

    if(!initializing_data_present)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "The initial (i.e. simulation-start) value of the initializer time-type "
                             " (%s) is zero.\n"
                             "No values were set, this could be an oversight.\n",
                             name.c_str());
    }

    // verify that the epoch value is consistent.
    verify_epoch();
    JeodBaseTime * epoch_ptr = time_manager->get_time_ptr(epoch_index);

    //
    // epoch not defined
    if((epoch_index == -2) && !updates_from_dyn)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "It is not possible to initialize the simulation with a UDE \n"
                             "time representation when there is no knowledge of an epoch.\n"
                             "If an epoch is desired, it is necessary to define the time-type in "
                             "which it is set.\n"
                             "If no epoch needed, it is sufficient to use dyn-time only.");
    }

    if(((epoch_index != -2) || epoch_data_present) && updates_from_dyn)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n"
                             "For UDEs that initialize the simulation AND update from Dyn, \n"
                             "the definition of an epoch is redundant and raises the potential \n"
                             "for inconsistent data definitions.  The UDE must have some initial\n"
                             "value in order to initialize the simulation, and Dyntime always has\n"
                             "an initial value = 0.  Suggest deleting reference to epochs for the\n"
                             "initializing time-type");
    }

    if(!epoch_data_present && !updates_from_dyn)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "The value of the epoch time-type at the epoch of the initializer\n"
                             "time-type (%s) is zero.\n"
                             "No values were set, this could be an oversight.\n",
                             name.c_str());
    }

    if((dynamic_cast<TimeUDE *>(epoch_ptr)) != nullptr)
    { // i.e. epoch is a UDE.
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n"
                             "Attempted to initialize the simulation from a time-type with a\n"
                             "user-defined-epoch (a UDE type), which in turn had an epoch defined in\n"
                             "another UDE type.  This is not a legal initialization method.\n");
    }

    //  Initializing with a UDE at an epoch defined in dyn_time can
    //  only work if there are no absolute (standard) times.
    if(dynamic_cast<TimeDyn *>(epoch_ptr) != nullptr)
    { // i.e. epoch is TimeDyn
        if(time_manager->time_standards_exist())
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::invalid_setup_error,
                                 "\n"
                                 "Initializing the simulation with time-type %s.  \n"
                                 "The epoch of this type is specified in DynamicTime (TimeDyn), which\n"
                                 "breaks connectivity to any standard time classes.\n"
                                 "The simulation contains standard times, which cannot be initialized\n"
                                 "with this initialization plan.\n",
                                 name.c_str());
        }
    }

    // Ensure there is a converter available to convert "this" to "update".
    int conv_index = update_index * time_manager->num_types + index;
    int conv_dir = time_manager_init->get_conv_dir_init(conv_index);
    if(conv_dir == 0)
    { // converter does not exist.  Redundant check,
        // impossible error.
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "In initializing the epoch of time type %s, it was necessary to \n"
                             "convert from %s, because that was defined as the update_from "
                             "time type. No converter was made available to \n"
                             "perform this task.\n",
                             name.c_str(),
                             update_from_name.c_str());
    }
    TimeConverter * converter_ptr = time_manager->get_converter_ptr(time_manager_init->get_conv_ptr_index(conv_index));

    // If the time-type does not update from Dyn, have a little extra work to do.
    if(!updates_from_dyn)
    {
        // set the time at epoch in the epoch-type
        set_epoch_times(epoch_ptr);
        epoch_ptr->override_initialized(true);

        // convert that time to a matching time in the update_from type
        if(epoch_index != update_index)
        {
            convert_epoch_to_update(epoch_ptr, update_from_ptr, time_manager_init);
        }
        epoch_ptr->override_initialized(false);
        update_from_ptr->override_initialized(true);

        // store any initial values, and set to epoch values.
        double init_secs = seconds;
        double init_days = days;
        seconds = 0.0;
        days = 0.0;

        initialized = true;
        converter_ptr->initialize(update_from_ptr, this, conv_dir);

        if((std::fpclassify(init_secs) != FP_ZERO) || (std::fpclassify(init_days) != FP_ZERO))
        {
            // revert the initial values on this time type
            seconds = init_secs;
            days = init_days;

            // and propagate to the "update_from" type.
            // First make sure the converter is valid.
            conv_index = index * time_manager->num_types + update_index;
            conv_dir = time_manager_init->get_conv_dir_init(conv_index);
            if(conv_dir == 0)
            { // converter is not valid.
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::incomplete_setup_error,
                                     "\n"
                                     "In initializing the simulation from a UDE, it is necessary to \n"
                                     "convert the initial values of the initializer time (%s) to its\n "
                                     "'update_from' time (%s).  However, while a converter is available\n"
                                     "between these two types, it has been declared as invalid for this\n"
                                     "particular direction at initialization.\n",
                                     name.c_str(),
                                     update_from_name.c_str());
            }
            if(conv_dir == 1)
            {
                converter_ptr->convert_a_to_b();
            }
            else
            {
                converter_ptr->convert_b_to_a();
            }
        }
    }
    else
    { // time does update from Dyn, trivial case, just initialize converter.
        initialized = true;
        converter_ptr->initialize(update_from_ptr, this, conv_dir);
    }
}

/**
 * To set the times in the epoch time type coincident with the
 * zero-point of this time-type
 *
 * \par Assumptions and Limitations
 *  - "This" is being defined by epoch.
 * \param[in] epoch_ptr pointer to the epoch time-type
 */
void TimeUDE::set_epoch_times(JeodBaseTime * epoch_ptr)
{
    auto * epoch_ptr_std = dynamic_cast<TimeStandard *>(epoch_ptr);
    auto * epoch_ptr_ude = dynamic_cast<TimeUDE *>(epoch_ptr);
    auto * epoch_ptr_dyn = dynamic_cast<TimeDyn *>(epoch_ptr);

    // Make "epoch" take on the epoch time.  Method depends on the time type
    if(epoch_ptr_std != nullptr)
    {
        set_epoch_std(epoch_ptr_std);
    }
    else if(epoch_ptr_ude != nullptr)
    {
        set_epoch_ude(epoch_ptr_ude);
    }
    else if(epoch_ptr_dyn != nullptr)
    {
        set_epoch_dyn(epoch_ptr_dyn);
    }
    else
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n"
                             "The declared type of epoch is unrecognized."); // Impossible error.
    }
}

/**
 * Temporarily overwrites the simulation data in time type "epoch"
 * with the epoch value.
 *
 * \par Assumptions and Limitations
 *  - "Epoch" is DynTime
 * \param[in] epoch_ptr pointer to the epoch time-type
 */
void TimeUDE::set_epoch_dyn(TimeDyn * epoch_ptr)
{
    if((epoch_year != 0) || (epoch_month != 0) || (epoch_day != 0) || (epoch_hour != 0) || (epoch_minute != 0))
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n"
                             "Can only use epoch_initializing_value to define an "
                             "epoch with respect to dyn_time. \n");
    }

    // This was already tested if !is_initializer, but fails here too if
    // is_initializer.
    if(initializing_data_present && epoch_data_present)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::redundancy_error,
                             "\n"
                             "Epoch values are overconstrained; Dyn time starts at zero, also "
                             "specifying an epoch time in dyn_time, and an initial time in the "
                             "relative time type can lead to inconsistent definitions of starting "
                             "time.\n");
    }

    if(epoch_value_is_set_number)
    {
        switch(epoch_format)
        {
            case TimeEnum::undefined:
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::invalid_setup_error,
                                     "\n"
                                     "%s was defined as the time-type for the epoch of time-type %s, but no "
                                     "format was defined for interpreting the value in that time-type.\n",
                                     epoch_defined_in_name.c_str(),
                                     name.c_str());
                break;
            case TimeEnum::Julian:
            case TimeEnum::julian:
            case TimeEnum::modified_julian:
            case TimeEnum::truncated_julian:
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::invalid_setup_error,
                                     "\n"
                                     "%s was defined as the time-type for the epoch of time-type %s.\n  "
                                     "The format for interpreting the value in that time-type was defined\n"
                                     "as being a Julian-based format, but %s is a Relative Derived Time,\n"
                                     "and as such Julian-based data has no meaning.\n",
                                     epoch_defined_in_name.c_str(),
                                     name.c_str(),
                                     epoch_defined_in_name.c_str());
                break;

            case TimeEnum::calendar:
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::invalid_setup_error,
                                     "\n"
                                     "Calendar is an invalid format for initializing the epoch based on \n"
                                     "an epoch_initializing_value.\n");
                break;

            case TimeEnum::clock:
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::invalid_setup_error,
                                     "\n"
                                     "Clock is an invalid format for initializing the epoch based on \n"
                                     "an epoch_initializing_value.\n");
                break;

            case TimeEnum::days_since_epoch:
                if(!epoch_value_is_set_number)
                {
                    MessageHandler::warn(__FILE__,
                                         __LINE__,
                                         TimeMessages::incomplete_setup_error,
                                         "\n"
                                         "The epoch of time-type %s is defined as being in a days-since-epoch "
                                         "representation of time type %s.  However, no values were set for "
                                         "that representation.  The simulation will proceed assuming that "
                                         "this value should be zero.\n",
                                         name.c_str(),
                                         epoch_defined_in_name.c_str());
                }
                epoch_ptr->set_time_by_days(epoch_initializing_value);
                break;

            case TimeEnum::seconds_since_epoch:
                if(!epoch_value_is_set_number)
                {
                    MessageHandler::warn(__FILE__,
                                         __LINE__,
                                         TimeMessages::incomplete_setup_error,
                                         "\n"
                                         "The epoch of time-type %s is defined as being in a "
                                         "seconds-since-epoch "
                                         "representation of time type %s.  However, no values were set for "
                                         "that representation.  The simulation will proceed assuming that "
                                         "this value should be zero.\n",
                                         name.c_str(),
                                         epoch_defined_in_name.c_str());
                }
                epoch_ptr->set_time_by_seconds(epoch_initializing_value);
                break;

            default:
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::invalid_setup_error,
                                     "\n"
                                     "Unrecognized format for epoch-type %s.\n",
                                     name.c_str());
        }
    }
}

/**
 * Overwrites the data in time type "epoch" with that in this class that
 * specifies the epoch.
 *
 * \par Assumptions and Limitations
 *  - "Epoch" is Absolute Derived Time
 * \param[in] epoch_ptr pointer to the epoch time-type
 */
void TimeUDE::set_epoch_std(TimeStandard * epoch_ptr)
{
    switch(epoch_format)
    {
        case TimeEnum::undefined:
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::incomplete_setup_error,
                                 "\n"
                                 "%s was defined as the time-type for the epoch of time-type %s, but no "
                                 "format was defined for interpreting the value in that time-type.\n",
                                 epoch_defined_in_name.c_str(),
                                 name.c_str());
            break;
        case TimeEnum::calendar:

            if(!epoch_value_is_set_calendar)
            {
                MessageHandler::warn(__FILE__,
                                     __LINE__,
                                     TimeMessages::incomplete_setup_error,
                                     "\n"
                                     "The epoch of time-type %s is defined as being in a calendar "
                                     "representation of time type %s.  However, no values were set for "
                                     "that representation.  The simulation will proceed assuming that "
                                     "these values are zero.\n",
                                     name.c_str(),
                                     epoch_defined_in_name.c_str());
            }

            // calendar defined and workable
            epoch_ptr->calendar_year = epoch_year;
            epoch_ptr->calendar_month = epoch_month;
            epoch_ptr->calendar_day = epoch_day;
            epoch_ptr->calendar_hour = epoch_hour;
            epoch_ptr->calendar_minute = epoch_minute;
            epoch_ptr->calendar_second = epoch_second;
            epoch_ptr->convert_from_calendar();
            break;

        case TimeEnum::clock:
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::invalid_setup_error,
                                 "\n"
                                 "Clock is an invalid format for initializing the epoch in a "
                                 "TimeStandard time type \n");
            break;

        case TimeEnum::Julian:
        case TimeEnum::julian:

            if(!epoch_value_is_set_number)
            {
                MessageHandler::warn(__FILE__,
                                     __LINE__,
                                     TimeMessages::incomplete_setup_error,
                                     "\n"
                                     "The epoch of time-type %s is defined as being in a Julian "
                                     "representation of time type %s.  However, no values were set for "
                                     "that representation.  The simulation will proceed assuming that "
                                     "this value should be zero.\n",
                                     name.c_str(),
                                     epoch_defined_in_name.c_str());
            }
            epoch_ptr->set_time_by_trunc_julian(epoch_initializing_value - 2440000.5);
            break;

        case TimeEnum::modified_julian:
            if(!epoch_value_is_set_number)
            {
                MessageHandler::warn(__FILE__,
                                     __LINE__,
                                     TimeMessages::incomplete_setup_error,
                                     "\n"
                                     "The epoch of time-type %s is defined as being in a Modified-Julian "
                                     "representation of time type %s.  However, no values were set for "
                                     "that representation.  The simulation will proceed assuming that "
                                     "this value should be zero.\n",
                                     name.c_str(),
                                     epoch_defined_in_name.c_str());
            }

            epoch_ptr->set_time_by_trunc_julian(epoch_initializing_value - 40000.0);
            break;

        case TimeEnum::truncated_julian:
            if(!epoch_value_is_set_number)
            {
                MessageHandler::warn(__FILE__,
                                     __LINE__,
                                     TimeMessages::incomplete_setup_error,
                                     "\n"
                                     "The epoch of time-type %s is defined as being in a Truncated-Julian"
                                     " representation of time type %s.  However, no values were set for "
                                     "that representation.  The simulation will proceed assuming that "
                                     "this value should be zero.\n",
                                     name.c_str(),
                                     epoch_defined_in_name.c_str());
            }

            epoch_ptr->set_time_by_trunc_julian(epoch_initializing_value);
            break;

        case TimeEnum::days_since_epoch:
            if(!epoch_value_is_set_number)
            {
                MessageHandler::warn(__FILE__,
                                     __LINE__,
                                     TimeMessages::incomplete_setup_error,
                                     "\n"
                                     "The epoch of time-type %s is defined as being in a days-since-epoch "
                                     "representation of time type %s.  However, no values were set for "
                                     "that representation.  The simulation will proceed assuming that "
                                     "this value should be zero.\n",
                                     name.c_str(),
                                     epoch_defined_in_name.c_str());
            }

            epoch_ptr->set_time_by_days(epoch_initializing_value);
            break;

        case TimeEnum::seconds_since_epoch:
            if(!epoch_value_is_set_number)
            {
                MessageHandler::warn(__FILE__,
                                     __LINE__,
                                     TimeMessages::incomplete_setup_error,
                                     "\n"
                                     "The epoch of time-type %s is defined as being in a "
                                     "seconds-since-epoch "
                                     "representation of time type %s.  However, no values were set for "
                                     "that representation.  The simulation will proceed assuming that "
                                     "this value should be zero.\n",
                                     name.c_str(),
                                     epoch_defined_in_name.c_str());
            }
            epoch_ptr->set_time_by_seconds(epoch_initializing_value);
            break;

        default:
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::invalid_setup_error,
                                 "\n"
                                 "Unrecognized format for epoch-type %s.\n",
                                 name.c_str());
    }
}

/**
 * Overwrites the data in time type "epoch" with that in this class
 * that specifies the epoch.
 *
 * \par Assumptions and Limitations
 *  - "Epoch" is a User-Defined-Epoch Time.
 * \param[in] epoch_ptr pointer to the epoch time-type
 */
void TimeUDE::set_epoch_ude(TimeUDE * epoch_ptr)
{
    switch(epoch_format)
    {
        case TimeEnum::undefined:
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::incomplete_setup_error,
                                 "\n"
                                 "%s was defined as the time-type for the epoch of time-type %s, but no "
                                 "format was defined for interpreting the value in that time-type.\n",
                                 epoch_defined_in_name.c_str(),
                                 name.c_str());
            break;
        case TimeEnum::Julian:
        case TimeEnum::julian:
        case TimeEnum::modified_julian:
        case TimeEnum::truncated_julian:
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::invalid_setup_error,
                                 "\n"
                                 "%s was defined as the time-type for the epoch of time-type %s.\n  "
                                 "The format for interpreting the value in that time-type was defined\n"
                                 "as being a Julian-based format, but %s is a User Defined Epoch Time,\n"
                                 "and as such Julian-based data has no meaning.\n",
                                 epoch_defined_in_name.c_str(),
                                 name.c_str(),
                                 epoch_defined_in_name.c_str());
            break;

        case TimeEnum::calendar:
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::invalid_setup_error,
                                 "\n"
                                 "%s was defined as the time-type for the epoch of time-type %s.\n  "
                                 "The format for interpreting the value in that time-type was defined\n"
                                 "as being a calendar-based format, but %s is a User Defined Epoch Time,\n"
                                 "and as such calendar-based data has no meaning.\n",
                                 epoch_defined_in_name.c_str(),
                                 name.c_str(),
                                 epoch_defined_in_name.c_str());
            break;

        case TimeEnum::clock:
            if(!epoch_value_is_set_clock)
            {
                MessageHandler::warn(__FILE__,
                                     __LINE__,
                                     TimeMessages::incomplete_setup_error,
                                     "\n"
                                     "The epoch of time-type %s is defined as being in a clock "
                                     "representation of time type %s.  However, no values were set for "
                                     "that representation.  The simulation will proceed assuming that "
                                     "these values are zero.\n",
                                     name.c_str(),
                                     epoch_defined_in_name.c_str());
            }

            // clock defined and workable
            epoch_ptr->clock_day = epoch_day;
            epoch_ptr->clock_hour = epoch_hour;
            epoch_ptr->clock_minute = epoch_minute;
            epoch_ptr->clock_second = epoch_second;
            epoch_ptr->set_time_by_clock();
            break;

        case TimeEnum::days_since_epoch:
            if(!epoch_value_is_set_number)
            {
                MessageHandler::warn(__FILE__,
                                     __LINE__,
                                     TimeMessages::incomplete_setup_error,
                                     "\n"
                                     "The epoch of time-type %s is defined as being in a days-since-epoch "
                                     "representation of time type %s.  However, no values were set for "
                                     "that representation.  The simulation will proceed assuming that "
                                     "this value should be zero.\n",
                                     name.c_str(),
                                     epoch_defined_in_name.c_str());
            }

            epoch_ptr->set_time_by_seconds(epoch_initializing_value * 86400);
            break;

        case TimeEnum::seconds_since_epoch:
            if(!epoch_value_is_set_number)
            {
                MessageHandler::warn(__FILE__,
                                     __LINE__,
                                     TimeMessages::incomplete_setup_error,
                                     "\n"
                                     "The epoch of time-type %s is defined as being in a "
                                     "seconds-since-epoch "
                                     "representation of time type %s.  However, no values were set for "
                                     "that representation.  The simulation will proceed assuming that "
                                     "this value should be zero.\n",
                                     name.c_str(),
                                     epoch_defined_in_name.c_str());
            }
            epoch_ptr->set_time_by_seconds(epoch_initializing_value);
            break;

        default:
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::invalid_setup_error,
                                 "\n"
                                 "Unrecognized format for epoch-type %s.\n",
                                 name.c_str());
    }
}

/**
 * Sets the initial value of this type from the myriad of initialization
 * options
 *
 * \par Assumptions and Limitations
 *  - At least one of the following is non-zero:
 *           initializing value, clock_day, clock_hour, clock_minute,
 *           clock_second, seconds, days
 */
void TimeUDE::set_initial_times()
{
    bool days_set = false;
    bool secs_set = false;
    bool clock_set = false;
    bool init_set = false;

    // Count the number of ways in which the initial value is specified (start
    // with 0):
    int count_set = 0;
    if(std::fpclassify(days) != FP_ZERO)
    {
        days_set = true;
        count_set++;
    }
    if(std::fpclassify(seconds) != FP_ZERO)
    {
        secs_set = true;
        count_set++;
    }
    if((clock_day != 0) || (clock_hour != 0) || (clock_minute != 0) || (std::fpclassify(clock_second) != FP_ZERO))
    {
        clock_set = true;
        count_set++;
    }
    if(std::fpclassify(initializing_value) != FP_ZERO)
    {
        init_set = true;
        count_set++;
    }

    // No initializing data:
    if(count_set == 0)
    {
        initializing_data_present = false;
    }
    // Too much initializing data
    else if(count_set > 1)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::redundancy_error,
                             "\n"
                             "The initial value for time type %s was over-defined.\n"
                             "Duplicate declarations were made (from days, seconds, clock, and\n"
                             "initializing_value) and there was no indication of which to use.",
                             name.c_str());
    }
    else
    { // And this porridge is just right:  1 and only 1 value set.
        initializing_data_present = true;

        //  If set by signle initial value, interpret that value:
        if(init_set)
        {
            switch(initial_value_format)
            {
                case TimeEnum::undefined:
                    if(std::fpclassify(initializing_value) != FP_ZERO)
                    {
                        MessageHandler::fail(__FILE__,
                                             __LINE__,
                                             TimeMessages::incomplete_setup_error,
                                             "\n"
                                             "The initial_value_format must be set in order to correctly \n"
                                             "interpret the initializing_value.\n");
                    }
                    break;
                case TimeEnum::seconds_since_epoch:
                    seconds = initializing_value;
                    days = seconds / 86400;
                    break;
                case TimeEnum::days_since_epoch:
                    days = initializing_value;
                    seconds = days * 86400;
                    break;
                case TimeEnum::calendar:
                case TimeEnum::Julian:
                case TimeEnum::julian:
                case TimeEnum::modified_julian:
                case TimeEnum::truncated_julian:
                case TimeEnum::clock:
                default:
                    MessageHandler::fail(__FILE__,
                                         __LINE__,
                                         TimeMessages::invalid_setup_error,
                                         "\n"
                                         "Invalid sim-start-format for initializing the simulation.\n");
            }
        }
        // or set by days, or ...
        else if(days_set)
        {
            seconds = days * 86400;
        }
        else if(secs_set)
        {
            days = seconds / 86400;
        }
        else if(clock_set)
        {
            seconds = clock_day * 86400 + clock_hour * 3600 + clock_minute * 60 + clock_second;
            days = seconds / 86400;
        }
        // and if none of those, something went badly wrong.
        else
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::memory_error,
                                 "\n"
                                 "Unknown error.  Should never get here.  There are 4 ways of"
                                 "initializing, and counting them yields that one is being used, but"
                                 "none of the four paths were followed.\n");
        }
    }
}

/**
 * Given a seconds value, sets days and clock values
 * \param[in] new_days new value for days\n Units: day
 */
void TimeUDE::set_time_by_days(const double new_days)
{
    JeodBaseTime::set_time_by_days(new_days);
    clock_update();
}

/**
 * Given a seconds value, sets days and clock values
 * \param[in] new_seconds new value for seconds\n Units: s
 */
void TimeUDE::set_time_by_seconds(const double new_seconds)
{
    JeodBaseTime::set_time_by_seconds(new_seconds);
    clock_update();
}

/**
 * sets the decimal representation of time by the clock
 *
 * \par Assumptions and Limitations
 *  - 24 hrs = 1 day; 60 minutes - 1 hour ; 60 seconds = 1 minute
 */
void TimeUDE::set_time_by_clock()
{
    seconds = clock_day * 86400 + clock_hour * 3600 + clock_minute * 60 + clock_second;
    days = seconds / 86400;
}

/**
 * sets the initial epoch value
 *
 * \par Assumptions and Limitations
 *  - Assumes that the number that is passed in is correctly entered with
 *         the correct units interpretation.
 * \param[in] simtime Used to verify that this is at initialization
 * \param[in] epoch the value to be used.
 */
void TimeUDE::set_epoch_initializing_value(const double simtime, const double epoch)
{
    if(simtime > 0)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n"
                             "An attempt was made to define the epoch for a TimeUDE called %s\n"
                             "at some time other than initialization.  This is not a valid\n"
                             "process.\n",
                             name.c_str());
    }
    else
    {
        epoch_initializing_value = epoch;
    }
}

/**
 * converts the decimal seconds value to a clock interface
 *
 * \par Assumptions and Limitations
 *  - 24 hrs = 1 day; 60 minutes - 1 hour ; 60 seconds = 1 minute
 */
void TimeUDE::clock_update()
{
    double scratch = seconds;
    clock_day = static_cast<int>(scratch / (24 * 60 * 60));
    scratch -= clock_day * 24 * 60 * 60;
    clock_hour = static_cast<int>(scratch / (60 * 60));
    scratch -= clock_hour * 60 * 60;
    clock_minute = static_cast<int>(scratch / 60);
    clock_second = scratch - clock_minute * 60;

    if(clock_second > 60 - clock_resolution)
    {
        clock_second = 0;
        clock_minute++;
        if(clock_minute == 60)
        {
            clock_minute = 0;
            clock_hour++;
            if(clock_hour == 24)
            {
                clock_hour = 0;
                clock_day++;
            }
        }
    }
}

/**
 * Verifies that the epoch assignments are legitimate, and tests for the
 * presence and legitimacy of values for defining the epoch.
 */
void TimeUDE::verify_epoch()
{
    epoch_value_is_set_number = false;
    epoch_value_is_set_calendar = false;
    epoch_value_is_set_clock = false;

    //  EPOCH TYPE
    epoch_index = time_manager->time_lookup(epoch_defined_in_name);

    if(epoch_index == -1)
    { // epoch_defined_in_name not found
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n"
                             "The value for 'epoch_defined_in' (%s) for type %s  was not found.\n",
                             epoch_defined_in_name.c_str(),
                             name.c_str());
    }
    if(epoch_index == index)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n"
                             "Time type %s has an epoch defined in its own time type.\n"
                             "Epochs must be defined with respect to some other time type. \n"
                             "Try adjusting %s.epoch_defined_in_name. \n",
                             name.c_str(),
                             name.c_str());
    }

    // EPOCH_VALUE
    if(std::fpclassify(epoch_initializing_value) != FP_ZERO)
    {
        epoch_value_is_set_number = true;
    }

    if((epoch_month != 0) && (epoch_day != 0))
    { // 0th month and 0th day are nonsense
        epoch_value_is_set_calendar = true;
    }

    if(((epoch_day != 0) || (epoch_hour != 0) || (epoch_minute != 0) || (std::fpclassify(epoch_second) != FP_ZERO)) &&
       ((epoch_year == 0) && (epoch_month == 0)))
    {
        epoch_value_is_set_clock = true;
    }

    if(epoch_value_is_set_number && (epoch_value_is_set_calendar || epoch_value_is_set_clock))
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::redundancy_error,
                             "\n"
                             "The epoch value was defined using both a numerical value "
                             "(epoch_initializing_value)\n"
                             "and a calendar or clock value.  It is not possible to use both.\n");
    }

    if(epoch_value_is_set_number || epoch_value_is_set_clock || epoch_value_is_set_calendar)
    {
        epoch_data_present = true;
    }

    // SANITY CHECKS

    if(epoch_index == -2)
    { // epoch type is undefined

        if(epoch_data_present)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::incomplete_setup_error,
                                 "\n"
                                 "The epoch value for Relative time type %s is set, but the \n"
                                 "time-type in which that value should be interpreted has not been "
                                 "defined.\n"
                                 "Either delete the specification of '%s.epoch_***', or set a time-type"
                                 "\n"
                                 "in '%s.epoch_defined_in_name' and specify an 'epoch_format'.\n",
                                 name.c_str(),
                                 name.c_str(),
                                 name.c_str());
        }

        if(epoch_format != TimeEnum::undefined)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::incomplete_setup_error,
                                 "\n"
                                 "Declared a format for the epoch of time-type %s, "
                                 "but no time-type in which to interpret that epoch, and no value to "
                                 "interpret. \n"
                                 "Unsure how to proceed.  Suggest deleting the 'epoch_format' "
                                 "assignment if it is not needed. \n",
                                 name.c_str());
        }
    }
}

/**
 * Verifies that any assignment to initialize_from is flagged as
 * inappropriate, and tests for the presence of initializing data.
 */
void TimeUDE::verify_init()
{
    int init_index = time_manager->time_lookup(initialize_from_name);

    if(init_index == -1)
    { // init_from_name not found
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             TimeMessages::redundancy_error,
                             "\n"
                             "For time %s:\n"
                             "'initialize_from_name' = %s, but this is not recognized as a valid\n"
                             "name for any of the registered time-types.  This specification is\n"
                             "not required for this time-type, suggest deleting it.\n",
                             name.c_str(),
                             initialize_from_name.c_str());
    }

    if(init_index != -2)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             TimeMessages::redundancy_error,
                             "\n"
                             "An initialize_from value was set for %s, a time class of type \n"
                             "'user-defined-epoch'.  UDE types do not have a specification of \n"
                             "initialize_from, because they have to be initialized from their parent\n"
                             "type (update_from)\n",
                             name.c_str());
    }

    set_initial_times();
}

/**
 * Ensures that the time-type identified as "update_from" is legitimate
 */
void TimeUDE::verify_update()
{
    update_index = time_manager->time_lookup(update_from_name);

    if(update_index == -2)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "'update_from_name' has not been defined.  UDE times must have an\n"
                             "update_from_name set.\n");
    }
    if(update_index == -1)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::incomplete_setup_error,
                             "\n"
                             "'update_from_name' = %s, but that name is not recognized as a valid "
                             "time-type.\n",
                             update_from_name.c_str());
    }
    auto * update_ptr_ude = dynamic_cast<TimeUDE *>(time_manager->get_time_ptr(update_index));
    if(update_ptr_ude != nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n"
                             "UDE-types cannot be updated from other UDEs, but should be updated from a"
                             "STD or Dyn type.");
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
