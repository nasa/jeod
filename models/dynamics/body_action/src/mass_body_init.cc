/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/mass_body_init.cc
 * Define methods for the mass body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((mass_body_init.cc)
   (body_action.cc)
   (body_action_messages.cc)
   (dynamics/mass/src/mass.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (dynamics/mass/src/mass_properties_init.cc)
   (environment/ephemerides/ephem_interface/src/ephem_ref_frame.cc)
   (utils/message/src/message_handler.cc)
   (utils/ref_frames/src/ref_frame.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/mass/include/mass.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/mass_body_init.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Initialize the core mass properties of the subject MassBody.
 * \param[in,out] dyn_manager Jeod manager
 */
void MassBodyInit::apply(DynManager & dyn_manager)
{
    // Initialize the mass properties and set the mass points.
    mass_subject->initialize_mass(properties, points);

    // Debug.
    MessageHandler::debug(__FILE__,
                          __LINE__,
                          BodyActionMessages::trace,
                          "%s: %s mass properties initialized.",
                          action_identifier.c_str(),
                          mass_subject->name.c_str());

    // Forward the action up the chain.
    BodyAction::apply(dyn_manager);
}

/**
 * Allocate points.
 * \param[in] num_points number of additional points to be allocated.
 */
void MassBodyInit::allocate_points(size_t num_points)
{
    for(size_t i = 0; i < num_points; ++i)
    {
        points.push_back(JEOD_ALLOC_CLASS_OBJECT(MassPointInit, ()));
    }
}

/**
 * Destructor
 */
MassBodyInit::~MassBodyInit()
{
    while(!points.empty())
    {
        JEOD_DELETE_OBJECT(points.back());
        points.pop_back();
    }
}

/**
 * Access a point in the vector.
 * \param[in] index the index of the point in the vector.
 */
MassPointInit * MassBodyInit::get_mass_point(size_t index)
{
    if(points.empty() || index > points.size() - 1)
    {
        return nullptr;
    }
    return points.at(index);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
