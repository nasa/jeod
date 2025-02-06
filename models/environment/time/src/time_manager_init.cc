/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_manager_init.cc
 * Define member functions for the Time Manager Initialization.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_manager_init.cc)
   (time_manager.cc)
   (time.cc)
   (time__add_type_update.cc)
   (time_ude.cc)
   (time_converter.cc)
   (time_converter_tai_utc.cc)
   (time_converter_tai_ut1.cc)
   (time_messages.cc)
   (utils/message/src/message_handler.cc))


******************************************************************************/

// System includes
#include <algorithm>
#include <cstddef>
#include <typeinfo>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/time_converter_tai_ut1.hh"
#include "../include/time_converter_tai_utc.hh"
#include "../include/time_manager.hh"
#include "../include/time_manager_init.hh"
#include "../include/time_messages.hh"
#include "../include/time_ude.hh"

//! Namespace jeod
namespace jeod
{
/**
 * The master program behind the initialization of the time types and
 * the time converters
 *
 * \par Assumptions and Limitations
 *  - None
 * \param[in,out] time_mgr The time manager
 */
void TimeManagerInit::initialize_manager(TimeManager * time_mgr)
{
    time_manager = time_mgr;

    // set up variables used by TimeManagerInit and verify that there are no
    // incompatibilities between time types.
    initialize();

    // Associate the TimeManager converter-function vector with the time-types.
    populate_converter_registry();

    // verify that there are no incompatibilities between the converters.
    verify_converter_setup();

    //  Generate a tree structure for initialization to ensure that all time-types
    //  can "see" the initializer type through a converter function, or a
    //  sequential chain of converter functions.
    //  Then initialize all time-types from the pre-defined "initializer".
    create_init_tree();
    initialize_time_types();

    //  Generate a tree structure for runtime updates to ensure that all time-types
    //  can "see" DynTime (the time-type that keeps track of elapsed seconds)
    //  through a sequential chain of converters,
    //      e.g. TimeDyn->TimeXXX->TimeYYY->TimeTHIS)
    //  Check also that those converter functions are appropriately set up, and
    //   that the TimeManager has a record of which time types are updated from
    //   which.
    create_update_tree();
}

/**
 * The TimeManagerInit determines initialization and update paths for
 * conversions between time-types.  This function creates and
 *             initializes the data structures necessary for these determinations
 *
 * \par Assumptions and Limitations
 *  - None
 */
void TimeManagerInit::initialize()
{
    dyn_time_index = time_manager->dyn_time.index;
    initializer_index = time_manager->time_lookup(initializer);

    time_manager->dyn_time.seconds = 0.0;
    time_manager->dyn_time.initialized = true;

    // Ensure that the time-type settings are mutually-consistent
    verify_times_setup();

    int num = time_manager->time_vector.size(); /* --
                                            local value used to size vector  */
    int num2 = num * num;                       // -- local value used to size vector

    // Allocate arrays in which individual elements represent a time type:
    status = JEOD_ALLOC_PRIM_ARRAY(num, int);

    // Allocate arrays in which individual elements represent a translation
    // from one time type to another (i.e., num*num elements).

    // the location, in the TimeManager->converter_ptr vector, of the converter
    // between TimeXXX and TimeYYY.
    converter_ptrs_index = JEOD_ALLOC_PRIM_ARRAY(num2, int);

    // The direction in which the converter should be applied at initialization
    // (value = 0 if the converter is not valid at initialization)
    init_converter_dir_table = JEOD_ALLOC_PRIM_ARRAY(num2, int);

    // The direction in which the converter should be applied at runtime
    // (value = 0 if the converter is not valid at runtime)
    update_converter_dir_table = JEOD_ALLOC_PRIM_ARRAY(num2, int);

    // for each time_type:
    for(int ii = 0; ii < num; ++ii)
    {
        // Initialize each time in the form that will cause a termination if not
        // corrected.
        status[ii] = 0; // status is the "generation" of a time type, used
                        // in defining the initialization and update trees.
                        // The head of the tree has status = 1, anything
                        // that it initializes/updates has status 2, etc.
    }

    // Initialize each converter pair as empty.
    for(int ii = 0; ii < num2; ++ii)
    {
        converter_ptrs_index[ii] = -1;
        init_converter_dir_table[ii] = 0;
        update_converter_dir_table[ii] = 0;
    }
}

/**
 * A number of checks that the setup is self-consistent
 *
 * \par Assumptions and Limitations
 *  - None
 */
void TimeManagerInit::verify_times_setup()
{
    if(initializer_index == -1) // initializer defined but not found
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n"
                             "The initializer time-type was defined with name %s, but this "
                             "time-type was not found in the registry.\n",
                             initializer.c_str());
    }

    if(initializer_index == -2) /* initializer undefined */
    {
        if(time_manager->num_types > 1)
        { // more than one time-type
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::incomplete_setup_error,
                                 "\n"
                                 "The initializer time-type was undefined and there is more than one\n"
                                 "time-type.  It is not possible to determine which time type should\n"
                                 "be used for initialization.  \n"
                                 "Set the variable 'time.time_manager_init.initializer = ' \n"
                                 "in the input file.");
        }
        else
        {
            initializer_index = 0; // the only time-type initializes itself!
        }
    }

    // initializer_index is now safely defined.

    // Check for duplicity
    for(int ii = 0; ii < time_manager->num_types; ++ii)
    {
        for(int jj = ii + 1; jj < time_manager->num_types; ++jj)
        {
            // Check for multiple instances of any time type.
            auto timeAPtr = time_manager->time_vector[ii];
            auto timeBPtr = time_manager->time_vector[jj];
            if((typeid(*timeAPtr) == typeid(*timeBPtr)) && time_manager->time_vector[ii]->must_be_singleton())
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::redundancy_error,
                                     "\n"
                                     "Time types '%s' and '%s' are the same type.\n"
                                     "Time classes should be used only once to prevent mis-assignments.\n"
                                     "If multiple instantiations are desired it is best to create a \n"
                                     "new class.  The exception to this rule is for UDE and MET times\n",
                                     time_manager->time_vector[ii]->name.c_str(),
                                     time_manager->time_vector[jj]->name.c_str());
            }
            // Check for multiple instances of any time name.
            if(time_manager->time_vector[ii]->name == time_manager->time_vector[jj]->name)
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::invalid_setup_error,
                                     "\n"
                                     "Two time types have been given the same name (%s).  This will cause "
                                     "confusion whenever a time-type has to be found by name only. \n",
                                     time_manager->time_vector[ii]->name.c_str());
            }
        }
    }
}

/**
 * The converter registry accounts for all of the converter functions
 * that provide conversions between time types.
 * This function populates that registry so that the existence of
 * functional converter functions can be tested efficiently.
 *
 * \par Assumptions and Limitations
 *  - None
 */
void TimeManagerInit::populate_converter_registry()
{
    int conv_indx_ab;
    int conv_indx_ba;

    for(unsigned int ii = 0; ii < time_manager->converter_vector.size(); ++ii)
    {
        int index_a = time_manager->time_lookup(time_manager->converter_vector[ii]->a_name);
        int index_b = time_manager->time_lookup(time_manager->converter_vector[ii]->b_name);
        if(index_a < 0)
        {
            MessageHandler::inform(__FILE__,
                                   __LINE__,
                                   TimeMessages::redundancy_error,
                                   "\n"
                                   "A time converter was defined using the names %s and %s for the two \n"
                                   "time-types involved, but %s was not recognized as one of the \n"
                                   "registered time-types.  This time-converter cannot be used in this \n"
                                   "simulation and is being ignored. \n",
                                   time_manager->converter_vector[ii]->a_name.c_str(),
                                   time_manager->converter_vector[ii]->b_name.c_str(),
                                   time_manager->converter_vector[ii]->a_name.c_str());
        }
        else if(index_b < 0)
        {
            MessageHandler::inform(__FILE__,
                                   __LINE__,
                                   TimeMessages::redundancy_error,
                                   "\n"
                                   "A time converter was defined using the names %s and %s for the two \n"
                                   "time-types involved, but %s was not recognized as one of the \n"
                                   "registered time-types.  This time-converter cannot be used in this \n"
                                   "simulation and is being ignored. \n",
                                   time_manager->converter_vector[ii]->a_name.c_str(),
                                   time_manager->converter_vector[ii]->b_name.c_str(),
                                   time_manager->converter_vector[ii]->b_name.c_str());
        }
        else // type_a and type_b both found
        {
            conv_indx_ab = index_a * time_manager->num_types + index_b;
            conv_indx_ba = index_b * time_manager->num_types + index_a;

            //  converter (from a to b) and/or (from b to a) is found at position ii
            //  in the time_converters_ptrs vector.
            //  check that it has not already been found (these values are initialized
            //  to -1, and only reset when a converter has been found):
            if((converter_ptrs_index[conv_indx_ab] >= 0) || (converter_ptrs_index[conv_indx_ba] >= 0))
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::redundancy_error,
                                     "\n"
                                     "There is a duplicity of conversion options"
                                     " between class %s and class %s \n"
                                     "(i.e. more than one converter has been registered to convert "
                                     " between these two types.  \n"
                                     "Unable to distinguish which type should be used. \n",
                                     time_manager->time_vector[index_a]->name.c_str(),
                                     time_manager->time_vector[index_b]->name.c_str());
            }

            // record the index value of the converter for both directions, even if
            // only one is valid for use.
            converter_ptrs_index[conv_indx_ab] = ii;
            converter_ptrs_index[conv_indx_ba] = ii;

            // record the functionality of the converter in the direction tables.
            if(time_manager->converter_vector[ii]->can_convert(TimeConverter::A_TO_B_INIT))
            {
                init_converter_dir_table[conv_indx_ab] = 1;
            }
            if(time_manager->converter_vector[ii]->can_convert(TimeConverter::A_TO_B_UPDATE))
            {
                update_converter_dir_table[conv_indx_ab] = 1;
            }
            if(time_manager->converter_vector[ii]->can_convert(TimeConverter::B_TO_A_INIT))
            {
                init_converter_dir_table[conv_indx_ba] = -1;
            }
            if(time_manager->converter_vector[ii]->can_convert(TimeConverter::B_TO_A_UPDATE))
            {
                update_converter_dir_table[conv_indx_ba] = -1;
            }
        }
    }
}

/**
 * To verify that there are no incompatibilities between specific
 * converters
 *
 * \par Assumptions and Limitations
 *  - The instance of TimeTAI, if it exists, has name "TAI"
 *  - The instance of TimeUTC, if it exists, has name "UTC"
 *  - The instance of TimeUT1, if it exists, has name "UT1"
 */
void TimeManagerInit::verify_converter_setup()
{
    // check on the appropriate usage of "override_data_table" in
    // TimeConverter_TAI_UTC and TimeConverter_TAI_UT1.
    // first check whether the time types are registered
    int tai_index = time_manager->time_lookup("TAI");
    int utc_index = time_manager->time_lookup("UTC");
    int ut1_index = time_manager->time_lookup("UT1");
    if((tai_index > 0) && (utc_index > 0) && (ut1_index > 0))
    {
        // next check whether the converters are registered.
        int tai_utc_conv_index = converter_ptrs_index[utc_index + tai_index * time_manager->num_types];
        int tai_ut1_conv_index = converter_ptrs_index[ut1_index + tai_index * time_manager->num_types];
        if((tai_utc_conv_index > 0) && tai_ut1_conv_index)
        {
            // finally check whether the flags are set differently in the two
            // converter classes
            auto * temp_conv1 = static_cast<TimeConverter_TAI_UTC *>(
                time_manager->converter_vector[tai_utc_conv_index]);
            auto * temp_conv2 = static_cast<TimeConverter_TAI_UT1 *>(
                time_manager->converter_vector[tai_ut1_conv_index]);

            if(temp_conv1->override_data_table != temp_conv2->override_data_table)
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::invalid_setup_error,
                                     "\n"
                                     "The UTC and UT1 time-types are usually calculated from look-up\n"
                                     "tables of the offsets.  These tables are tied very closely \n"
                                     "together, and while the override_data_table flag provides the \n"
                                     "functionality to override the data values in those tables, it \n"
                                     "is required that this flag be set the same way for both tables.\n");
            }
        }
    }
}

/**
 * Build and verify a "tree-like" structure to ensure that all time
 * representations can be initialized from the single "initializer"
 * representation.
 * Create_init_tree builds the tree, using add_type_initialize to
 * populate the tree recursively.
 *
 * \par Assumptions and Limitations
 *  - This is vastly improved if the user defines the parent type
 *                 "initialize_from" for each time representation, except the
 *                 top-level initializer type
 *  - Otherwise, the code will build the tree automatically, but it
 *                 takes longer and may be less than ideal
 */
void TimeManagerInit::create_init_tree()
{
    int seeking_status = 0; /* -- for automated tree-builder routine
                             *    looks for time-types with
                             *    status = seeking_status
                             */

    // All status values = 0, already set in initialize_TMI_settings
    status[dyn_time_index] = -2;
    status[initializer_index] = 1;

    if(initializer_index != dyn_time_index) // Dyn time is not the time initialization source
    {
        // if initializer is a UDE, also need to place its "update_from" in the
        // initialization tree with status = 2, because it will need to
        // communicate with a standard time.
        auto * init_ude_ptr = dynamic_cast<TimeUDE *>(time_manager->time_vector[initializer_index]);
        if(init_ude_ptr != nullptr)
        {
            int upd_index = time_manager->time_lookup(init_ude_ptr->update_from_name);
            if(upd_index < 0) // If unable to tie the UDE to a standard: fatal error
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     TimeMessages::invalid_setup_error,
                                     "\n"
                                     "The inititializer (%s) update_from_name not registered.\n"
                                     "The initializer is a UDE, so must have an update_from_name.",
                                     init_ude_ptr->update_from_name.c_str());
            }
            else // else found the standard to convert the UDE to a nonarbitrary form
            {
                status[upd_index] = 2; // from 1st gen initializer time, so 2
                num_added_total = 3;
            }
        }

        else // else not initializing from a UDE
        {
            // Since they are fundamental (root) times, don't need to place dyn_time
            // or initializer in the tree, so consider these placed
            num_added_total = 2;
        }
    }
    else // else dyn_time is the initializer
    {
        num_added_total = 1; // avoid double-counting dyn_time and initializer
    }

    num_added_pass = num_added_total; // Set to initialize while loop below

    // Add types to the initialization tree
    // until all types have been added or nothing else can be added.
    while(num_added_total < time_manager->num_types)
    {
        if(num_added_pass > 0)
        {
            num_added_pass = 0;
            seeking_status++;
            for(int ii = 0; ii < time_manager->num_types; ++ii)
            {
                if(status[ii] == 0)
                {
                    time_manager->time_vector[ii]->add_type_initialize(seeking_status, this);
                }
            }
            // add the number of types added in this pass to the total number of types
            // added.
            num_added_total += num_added_pass;
        }
        else
        {
            // went through the last pass without adding anything, this pass will be
            // no different.  Tree-build has failed.
            // Report the first culprit (only one is reported) and quit simulation.
            for(int ii = 0; ii < time_manager->num_types; ++ii)
            {
                if(status[ii] == 0)
                {
                    MessageHandler::fail(__FILE__,
                                         __LINE__,
                                         TimeMessages::invalid_setup_error,
                                         "\n"
                                         "Could not get time-type %s into the tree structure for "
                                         "initialization.\n"
                                         "There must be insufficient time converters registered - that \n"
                                         "are valid at initialization - to allow access to this time-type\n"
                                         "from the initializer (%s).\n",
                                         time_manager->time_vector[ii]->name.c_str(),
                                         time_manager->time_vector[initializer_index]->name.c_str());
                }
            }
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::initialization_error,
                                 "\n"
                                 "Error in generating the initialization tree.\n");
        }
    }
}

/**
 * Initialize each time type so that it has a starting value
 * corresponding to dynamic_time = 0 and such that the starting values
 * are consistent.
 * Initialize_time_types repetitively calls initialize_from_parent for
 * each time type; initialize_from_parent recursively adds types moving
 * up the tree as necessary
 *
 * \par Assumptions and Limitations
 *  - An initializer time defined by the user
 */
void TimeManagerInit::initialize_time_types()
{
    // First, take care of the head of the tree
    time_manager->time_vector[initializer_index]->initialize_initializer_time(this);

    // new:
    for(int ii = 0; ii < time_manager->num_types; ++ii)
    {
        JeodBaseTime * time_ptr = time_manager->get_time_ptr(ii);
        if(!time_ptr->initialized)
        {
            time_ptr->initialize_from_parent(this);
        }
    }
}

/**
 * (To verify that the update procedures have a tree-like structure,
 * and that all time representations can be updated from the dynamic time.)
 *  (Contains 3 functions - create_update_tree builds the tree, populated
 *   recursively by add_type_update.  record_update records the update paths
 * to facilitate runtime updates)
 *
 * \par Assumptions and Limitations
 *  - None
 */
void TimeManagerInit::create_update_tree()
{
    int seeking_status = 0; /* desired status value of next time when during the
                             * fully-automated building of the update tree. */

    for(int ii = 0; ii < time_manager->num_types; ++ii)
    {
        status[ii] = 0;
    }
    status[dyn_time_index] = 1; // dyn_time is update source, always 1st gen.

    num_added_total = 1; // number of time-types placed in the update tree
                         // (Dyn-time has already been added at status 0).
    num_added_pass = 1;  /* counter for number of time-types that have
                          * been added into the update tree in any given
                          * pass. initial pass is only dyn_time, so set 1. */

    //  while there is something to add to the tree, and capability to add it:
    while(num_added_total < time_manager->num_types)
    {
        if(num_added_pass > 0)
        {
            num_added_pass = 0;
            seeking_status++;
            for(int ii = 0; ii < time_manager->num_types; ++ii)
            {
                if(status[ii] == 0)
                {
                    time_manager->time_vector[ii]->add_type_update(seeking_status, this);
                }
            }
        }
        else
        {
            // went through an entire without adding anything. This pass will be
            // no different, because the auto-seek found nothing. Tree-build has
            // failed. Report the first uninitialized culprit and quit
            // the simulation.
            for(int ii = 0; ii < time_manager->num_types; ++ii)
            {
                if(status[ii] == 0)
                {
                    MessageHandler::fail(__FILE__,
                                         __LINE__,
                                         TimeMessages::incomplete_setup_error,
                                         "\n"
                                         "Could not get time-type %s into the tree structure for "
                                         "run-time updates.\n"
                                         "There must be insufficient time converters registered for \n"
                                         "run-time updates to allow access to this time-type from "
                                         "TimeDyn.\n",
                                         time_manager->time_vector[ii]->name.c_str());
                }
            }
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 TimeMessages::initialization_error,
                                 "\n"
                                 "Error in generating the update tree.\n");
        }
    }

    organize_update_list();
}

/**
 * Reorganizes the update list according to initialization status
 */
void TimeManagerInit::organize_update_list()
{
    // vector of finalized index positions
    std::vector<unsigned> order;
    order.reserve(time_manager->num_types);

    // Create index vector
    for(int ii = 1; ii <= time_manager->num_types; ii++)
    {
        for(int jj = 0; jj < time_manager->num_types; jj++)
        {
            if(status[jj] == ii)
            {
                order.push_back(jj);
            }
        }
    }

    // Apply index vector
    for(unsigned ii = 0, jj = 0; ii < order.size() - 1; ii++)
    {
        if(ii != order[ii])
        {
            for(jj = ii + 1; jj < order.size(); jj++) // Find order swap
            {
                if(order[jj] == ii) // found
                {
                    break;
                }
            }
            std::swap(time_manager->time_vector[ii], time_manager->time_vector[order[ii]]);
            std::swap(status[ii], status[order[ii]]);
            std::swap(order[ii], order[jj]);
        }
    }
}

/**
 * Takes a calculated converter index - calculated by combining the two
 * time-type indices - and return the index in the time_manager's vector of
 * converters that corresponds to those two time-types.
 * @return Index corresponding to TimeConverter
 * \param[in] index_in Index of object
 */
int TimeManagerInit::get_conv_ptr_index(const int index_in)
{
    int index_out;

    if(index_in < 0)
    {
        index_out = -1;
    }
    else
    {
        index_out = converter_ptrs_index[index_in];
    }
    return index_out;
}

/**
 * Takes a calculated converter index - calculated by combining the
 * indices of two time-types, a "from" and a "to" - and returns the direction
 * needed to use the appropriate converter to go from "from" to "to".
 *
 * \par Assumptions and Limitations
 *  - Returns 0 if no suitable converter available at initialization
 * @return Index corresponding to TimeConverter
 * \param[in] index Index of object
 */
int TimeManagerInit::get_conv_dir_init(const int index)
{
    int direction;

    if(index < 0)
    {
        direction = 0;
    }
    else
    {
        direction = init_converter_dir_table[index];
    }
    return direction;
}

/**
 * Takes a calculated converter index - calculated by combining the
 * indices of two time-types, a "from" and a "to" - and returns the direction
 * needed to use the appropriate converter to go from "from" to "to".
 *
 * \par Assumptions and Limitations
 *  - Returns 0 if no suitable converter available at update
 * @return Index corresponding to TimeConverter
 * \param[in] index Index of object
 */
int TimeManagerInit::get_conv_dir_upd(const int index)
{
    int direction;

    if(index < 0)
    {
        direction = 0;
    }
    else
    {
        direction = update_converter_dir_table[index];
    }
    return direction;
}

/**
 * Returns the status of a time-type
 * @return Integer corresponding to Status
 * \param[in] index Index of object
 */
int TimeManagerInit::get_status(const int index)
{
    int status_out;

    if(index < 0)
    {
        status_out = 0;
    }
    else
    {
        status_out = status[index];
    }
    return status_out;
}

/**
 * Receives an updated value for the status of a time-type
 * \param[in] index Index of object
 * \param[in] new_status New status value
 */
void TimeManagerInit::set_status(const int index, const int new_status)
{
    status[index] = new_status;
}

/**
 * Modifies the status of one time-type to be one higher than that of
 * another type for initialization purposes
 * \param[in] index_slave Index of object
 * \param[in] index_master Index of object
 */
void TimeManagerInit::increment_status(const int index_slave, const int index_master)
{
    status[index_slave] = status[index_master] + 1;
    num_added_pass++;
}

/**
 * Destroy a TimeManagerInit
 */
TimeManagerInit::~TimeManagerInit()
{
    JEOD_DELETE_ARRAY(status);
    JEOD_DELETE_ARRAY(converter_ptrs_index);
    JEOD_DELETE_ARRAY(init_converter_dir_table);
    JEOD_DELETE_ARRAY(update_converter_dir_table);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
