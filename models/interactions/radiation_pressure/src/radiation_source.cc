/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_source.cc
 * Definition of methods associated with Primary Sources
 */

/******************************* TRICK HEADER *********************************
PURPOSE:
    ()
REFERENCE:
    (((None)))
ASSUMPTIONS AND LIMITATIONS:
    ((A Primary Source is defined as one that emits its own light.  The only
      realistic Primary Source is Sun.)
     (Primary Sources can be shadowed by 3rd bodies)
     (It is assumed that Primary sources emit radiation isotropically, and that
     the radiation received depends only on distnace from the source, and the
     projected area of the receiver.))

CLASS:
    (scheduled)

LIBRARY DEPENDENCY:
   ((radiation_source.cc)
    (radiation_messages.cc)
    (radiation_third_body.cc)
    (utils/message/src/message_handler.cc))



******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame.hh"

// Model includes
#include "../include/radiation_source.hh"
// #include "../include/radiation_third_body.hh"
// #include "../include/radiation_pressure.hh"
#include "../include/radiation_messages.hh"

//! Namespace jeod
namespace jeod
{

/* ENTRY POINT: */

/**
 * calculates the flux vector from the vehicle's position.
 * \param[in] veh_struc_frame the vehicle structural reference frame
 * \param[in] center_gravity position of the center of mass\n Units: M
 */

/*****************************************************************************
  Purpose:
    (calculates the flux vector from the vehicle's position.)
*****************************************************************************/
void RadiationSource::calculate_flux(RefFrame & veh_struc_frame, const double center_gravity[3])
{
    if(luminosity < 1.0E-6)
    {
        flux_mag = 0.0;
        return;
    }

    //  Calculate the position of the vehicle with respect to Primary Source.
    //  Start by getting the position of the origin of the vehicle structural
    //  frame.
    veh_struc_frame.compute_position_from(*inertial_frame_ptr, source_to_struc_origin);

    // center_gravity is in structural.  Need it in inertial to complete the
    // position vector (center_gravity is more closely set to the vehicle than the
    // origin of the structural frame).  Since the parent-frame of any root
    // vehicle is an inertial-frame, the vehicle's T_parent_this will convert
    // structural to inertial.
    Vector3::transform_transpose(veh_struc_frame.state.rot.T_parent_this, center_gravity, inertial_cg);

    // Add the vector to structural origin to the vector from structural origin to
    // vehicle center.
    Vector3::sum(source_to_struc_origin, inertial_cg, source_to_cg);

    // Now normalize this vector.  Since we need the magnitude later, do this in
    // two steps, rather than use the available "normalize" function.

    d_source_to_cg = Vector3::vmag(source_to_cg);

    Vector3::scale(source_to_cg, 1 / d_source_to_cg, flux_hat);

    flux_mag = luminosity / (d_source_to_cg * d_source_to_cg * 4 * M_PI);

    Vector3::scale(flux_hat, flux_mag, flux_inertial);

    // Transform this vector back into structural reference and normalize.
    Vector3::transform(veh_struc_frame.state.rot.T_parent_this, flux_inertial, flux_struc);

    Vector3::normalize(flux_struc, flux_struc_hat);
}

/**
 * Initializes the source object for use in the Radiation Pressure model
 * \param[in] dyn_mgr_ptr pointer to the dynamics manager
 */
void RadiationSource::initialize(DynManager * dyn_mgr_ptr)
{
    Planet * source_planet = dyn_mgr_ptr->find_planet(name);
    if(source_planet != nullptr)
    {
        inertial_frame_ptr = &(source_planet->inertial);
    }
    else
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::incomplete_setup_error,
                             "\n"
                             "Source (%s) not found by DynManager.\n",
                             name.c_str());
    }

    dyn_mgr_ptr->subscribe_to_frame(*inertial_frame_ptr);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
