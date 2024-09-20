/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/default_aero.cc
 * Implement a virtual base class for aerodynamic drag calculations.
 * Also implement a specific version, in the base class, that contains
 * ballistic coefficent and coefficient of drag options
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((default_aero.cc)
     (aerodynamics_messages.cc)
     (utils/message/src/message_handler.cc))


*******************************************************************************/

// System includes
#include <cmath>

// JEOD includes
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/aero_drag.hh"
#include "../include/aerodynamics_messages.hh"
#include "../include/default_aero.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The implementation for this aerodynamic drags force and torque
 * calculations. Can be overriden by an inheriting class to create
 * extensibility
 * \param[in] velocity_mag The magnitude of the relative velocity of the vehicle; not used here but some child classes
 * need it\n Units: M/s \param[in] rel_vel_hat The unit vector of the relative velocity of the vehicle, in the
 * structural frame \param[in] aero_drag_param_ptr The aerodynamic drag parameters used to calculate drag \param[in]
 * mass The current mass of the vehicle\n Units: kg \param[out] force The aerodynamic force, in the structural frame\n
 * Units: N \param[out] torque The aerodynamic torque, in the structural frame\n Units: N*M
 */

void DefaultAero::aerodrag_force(const double,
                                 const double rel_vel_hat[3],
                                 AeroDragParameters * aero_drag_param_ptr,
                                 double mass,
                                 double force[3],
                                 double torque[3])
{
    switch(option)
    {
        case DRAG_OPT_CD:

            drag = -aero_drag_param_ptr->dynamic_pressure * area * Cd;
            break;

        case DRAG_OPT_BC:

            if(std::fpclassify(BC) == FP_ZERO)
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     AerodynamicsMessages::runtime_error,
                                     "The Aerodynamics object was told to use ballistic "
                                     "coefficient, but no coefficient was set. This is "
                                     "a fatal error");
            }
            drag = -(aero_drag_param_ptr->dynamic_pressure * mass) / BC;
            break;

        case DRAG_OPT_CONST:

            break; // The drag has been set in the variable "drag" by the user,
            // so nothing further must be done by this algorithm until after
            // the case statement

        default:

            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 AerodynamicsMessages::runtime_warns,
                                 "No option was sent to the Aerodynamics object for calculating "
                                 "drag. Drag has been set to zero.");

            drag = 0.0;

            break;
    }

    Vector3::scale(rel_vel_hat, drag, force);
    Vector3::initialize(torque);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
