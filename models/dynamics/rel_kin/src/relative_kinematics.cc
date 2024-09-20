/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup RelKin
 * @{
 *
 * @file models/dynamics/rel_kin/src/relative_kinematics.cc
 * Define methods for the RelativeKinematics class.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((relative_kinematics.cc)
   (rel_kin_messages.cc)
   (dynamics/derived_state/src/relative_derived_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc))


*******************************************************************************/

// System includes
#include <algorithm> // std::find
#include <cstddef>

// JEOD includes
#include "dynamics/derived_state/include/relative_derived_state.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/rel_kin_messages.hh"
#include "../include/relative_kinematics.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Construct a RelativeKinematics object.
 */
RelativeKinematics::RelativeKinematics()
{
    JEOD_REGISTER_CLASS(RelativeKinematics);
    JEOD_REGISTER_CLASS(RelativeDerivedState);
    JEOD_REGISTER_CHECKPOINTABLE(this, relative_states);
}

/**
 * Destruct a RelativeKinematics object.
 */
RelativeKinematics::~RelativeKinematics()
{
    JEOD_DEREGISTER_CHECKPOINTABLE(this, relative_states);
}

/**
 * Add a relative state to the list of ones maintained by this model.
 * \param[in] relstate Relstate to add
 */
void RelativeKinematics::add_relstate(RelativeDerivedState & relstate)
{
    // Check to see if rel state name is valid
    NamedItem::validate_name(__FILE__,
                             __LINE__,
                             relstate.name,
                             "Name of relative state to be added",
                             "input parameter relstate.name");

    // See if given relstate is already in list
    RelativeDerivedState * found_relstate = nullptr;
    found_relstate = find_relstate(relstate.name);
    if(found_relstate != nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RelKinMessages::duplicate_entry,
                             "Relative state %s already in RelKin list",
                             relstate.name.c_str());
        return;
    }

    // If not duplicate, then add to list of relstates and increment counter
    relative_states.push_back(&relstate);
    ++num_rel_states;
}

/**
 * Remove a relative state from the list of ones maintained by this model.
 * \param[in] relstate  Relstate to remove
 */
void RelativeKinematics::remove_relstate(RelativeDerivedState & relstate)
{
    auto it = std::find(relative_states.begin(), relative_states.end(), &relstate);

    if(it == relative_states.end())
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RelKinMessages::entry_not_found,
                              "Relative state %s is not in the RelKin list",
                              relstate.name.c_str());
        return;
    }

    relative_states.erase(it);
    --num_rel_states;
}

/**
 * Find a specific relative state maintained by this model.
 * @return Void
 * \param[in] relstate_name Relstate to find
 */
RelativeDerivedState * RelativeKinematics::find_relstate(const std::string & relstate_name)
{
    RelativeDerivedState * found_relstate = nullptr;
    unsigned int n_relstates = num_rel_states;

    for(unsigned int ii = 0; ii < n_relstates; ++ii)
    {
        if(relstate_name == relative_states[ii]->name)
        {
            found_relstate = relative_states[ii];
            break;
        }
    }

    return found_relstate;
}

/**
 * Set flag for a relative state to be activated or
 * deactivated by the RelKin manager.
 * \param[in] relstate  Relstate to activate/deactivate
 * \param[in] raf  bool Relstate activation flag
 */
void RelativeKinematics::activate_relstate(RelativeDerivedState & relstate, bool raf)
{
    // Check if given relstate is in the relstate list
    RelativeDerivedState * found_relstate = nullptr;
    found_relstate = find_relstate(relstate.name);
    if(found_relstate == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RelKinMessages::entry_not_found,
                             "Relative state %s is not in the RelKin list",
                             relstate.name.c_str());
        return;
    }

    // Check if bool input is valid. Valid inputs are either
    // true or false
    if(raf != true && raf != false)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RelKinMessages::invalid_entry,
                             "%s is an invalid input. Enter either true or false",
                             &raf);
        return;
    }

    // If relstate found and Relstate activation flag is valid,
    // set activation flag status
    relstate.set_activation_flag(raf);
}

/**
 * Update a single relative state maintained by this model.
 * \param[in] relstate_name Relstate to update
 */
void RelativeKinematics::update_single(const std::string & relstate_name)
{
    // Find the relative state and call its update function
    find_relstate(relstate_name)->update();
}

/**
 * Update all relative states maintained by this model.
 * relstates that have been deactivated from RelKin will
 * not be update.
 */
void RelativeKinematics::update_all()
{
    unsigned int n_relstates = num_rel_states;

    for(unsigned int ii = 0; ii < n_relstates; ++ii)
    {
        if(relative_states[ii]->active == true)
        {
            relative_states[ii]->update();
        }
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
