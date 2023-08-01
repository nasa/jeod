/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup PlanetFixed
 * @{
 * @addtogroup NorthEastDown
 * @{
 *
 * @file models/utils/planet_fixed/north_east_down/src/north_east_down.cc
 * NorthEastDown class methods
 */

/*******************************************************************************
Purpose:
   ()

Library Dependency:
   ((north_east_down.cc)
    (utils/message/src/message_handler.cc)
    (utils/planet_fixed/planet_fixed_posn/src/planet_fixed_messages.cc)
    (utils/planet_fixed/planet_fixed_posn/src/planet_fixed_posn.cc)
    (utils/quaternion/src/quat_from_mat.cc)
    (utils/ref_frames/src/ref_frame.cc))

 
*******************************************************************************/

// System includes
#include <cstdio>
#include <cmath>

// Trick Includes

// JEOD Includes
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_messages.hh"

#include "../include/north_east_down.hh"


//! Namespace jeod
namespace jeod {

/**
 * NorthEastDown constructor.
 */
NorthEastDown::NorthEastDown(
   void)
{
   altlatlong_type = undefined;
}


/**
 * NorthEastDown destructor.
 */
NorthEastDown::~NorthEastDown(
   void)
{
   ; // Nothing; class does not allocate resources
}


/**
 * Update from Cartesian position input.
 * \param[in] cart Cartesian coords, PCPF\n Units: M
 */
void NorthEastDown::update_from_cart(
      const double cart[3])
{
   PlanetFixedPosition::update_from_cart(cart);

   Vector3::copy(this->cart_coords, this->ned_frame.state.trans.position);

   return;
}


/**
 * Update from Spherical position input.
 * \param[in] spher Spherical AltLatLong position
 */
void NorthEastDown::update_from_spher(
      const AltLatLongState &spher)
{
   PlanetFixedPosition::update_from_spher(spher);

   Vector3::copy(this->cart_coords, this->ned_frame.state.trans.position);

   return;
}


/**
 * Update from Elliptical position input.
 * \param[in] ellip Elliptical AltLatLong position
 */
void NorthEastDown::update_from_ellip(
      const AltLatLongState &ellip)
{
   PlanetFixedPosition::update_from_ellip(ellip);

   Vector3::copy(this->cart_coords, this->ned_frame.state.trans.position);

   return;
}


/**
 * Build NED frame state based on current reference point information.
 */
void NorthEastDown::build_ned_orientation(
   void)
{
   // Compute the sine and cosine of the latitude and longitude used to specify
   // the NED frame orientation
   double sinlat = 0.0;
   double coslat = 0.0;
   double sinlon = 0.0;
   double coslon = 0.0;

   if (this->altlatlong_type == spherical) {
      sinlat = std::sin (sphere_coords.latitude);
      coslat = std::cos (sphere_coords.latitude);
      sinlon = std::sin (sphere_coords.longitude);
      coslon = std::cos (sphere_coords.longitude);
   }
   else if (this->altlatlong_type == elliptical) {
      sinlat = std::sin (ellip_coords.latitude);
      coslat = std::cos (ellip_coords.latitude);
      sinlon = std::sin (ellip_coords.longitude);
      coslon = std::cos (ellip_coords.longitude);
   }
   // Any other value is undefined/invalid
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, PlanetFixedMessages::invalid_request,
         "AltLatLong set used to define North-East-Down frame not specified.");
      return;
   }

   // Construct the NED frame's orientation
   ned_frame.state.rot.T_parent_this[0][0] = -sinlat * coslon;
   ned_frame.state.rot.T_parent_this[0][1] = -sinlat * sinlon;
   ned_frame.state.rot.T_parent_this[0][2] =  coslat;
   ned_frame.state.rot.T_parent_this[1][0] = -sinlon;
   ned_frame.state.rot.T_parent_this[1][1] =  coslon;
   ned_frame.state.rot.T_parent_this[1][2] =  0;
   ned_frame.state.rot.T_parent_this[2][0] = -coslat * coslon;
   ned_frame.state.rot.T_parent_this[2][1] = -coslat * sinlon;
   ned_frame.state.rot.T_parent_this[2][2] = -sinlat;
   ned_frame.state.rot.compute_quaternion ();
   Vector3::initialize (ned_frame.state.rot.ang_vel_this);
   ned_frame.state.rot.ang_vel_mag = 0.0;

   return;
}


/**
 * Build NED frame state based on current reference point information.
 * \param[in] pos Cartesian position, PCPF\n Units: M
 * \param[in] vel Cartesian velocity, PCPF\n Units: M/s
 */
void NorthEastDown::set_ned_trans_states(
      double const pos[3],
      double const vel[3])
{
   Vector3::copy(pos, this->ned_frame.state.trans.position);
   Vector3::copy(vel, this->ned_frame.state.trans.velocity);

   PlanetFixedPosition::update_from_cart(pos);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
