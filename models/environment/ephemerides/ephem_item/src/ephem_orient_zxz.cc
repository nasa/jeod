/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemItem
 * @{
 *
 * @file models/environment/ephemerides/ephem_item/src/ephem_orient_zxz.cc
 * Define member functions for the EphemItem class and subclasses.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((ephem_orient_zxz.o)
   (ephem_orient.o)
   (ephem_item.o)
   (utils/quaternion/quat.o)
   (utils/quaternion/quat_norm.o)
   (utils/quaternion/quat_to_mat.o))



*******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_interface.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"
#include "environment/planet/include/base_planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/quaternion/include/quat.hh"

// Model includes
#include "../include/ephem_orient_zxz.hh"


// EPSILON_TIME
// Limit designating an overly small time step, which typically result from
// numerical errors.
#define EPSILON_TIME 1e-12

// TAYLOR_CUTOFF:
// Limit for approximating the quaternion exponential Q_rot = e^(phi*uhat) via a
// normalized two-term Taylor expansion. The error in this normalized expansion
// is bounded by phi^5/30.
// Because of finite-precision arithmetic, the error in the propagated
// quaternion Q_prop = Q_rot^T Q will be more than 1e-12 for even very small
// angles. Keeping the error in Q_rot smaller than 1 part in 1e-12 means
// that phi^5/30 <= 1e-12, or phi = 30e-12^0.2 = 0.00786.
#define TAYLOR_CUTOFF 0.00786



//! Namespace jeod
namespace jeod {

/**
 * Construct an ephemeris angle.
 */
EphemerisZXZOrientation::EphemerisZXZOrientation (
   void)
:
   EphemerisOrientation()
{
   Vector3::initialize (euler_angle_313);
   Vector3::initialize (euler_rate_313);
}


/**
 * Destroy an ephemeris angle.
 */
EphemerisZXZOrientation::~EphemerisZXZOrientation (
   void)
{
   ; // Empty; object does not allocate memory
}


/**
 * Return the Euler angles.
 * @return Euler angles
 */
const double *
EphemerisZXZOrientation::get_euler_angles (
   void)
const
{
   return euler_angle_313;
}


/**
 * Return the Euler angles.
 * \param[out] angles Euler angles\n Units: r
 */
void
EphemerisZXZOrientation::get_euler_angles (
   double * angles)
const
{
   angles[0] = euler_angle_313[0];
   angles[1] = euler_angle_313[1];
   angles[2] = euler_angle_313[2];

   return;
}


/**
 * Return the Euler rates.
 * @return Euler rates
 */
const double *
EphemerisZXZOrientation::get_euler_rates (
   void)
const
{
   return euler_rate_313;
}


/**
 * Return the Euler angles.
 * \param[out] rates Euler rates\n Units: r/s
 */
void
EphemerisZXZOrientation::get_euler_rates (
   double * rates)
const
{
   rates[0] = euler_rate_313[0];
   rates[1] = euler_rate_313[1];
   rates[2] = euler_rate_313[2];
}


/**
 * Propagate the orientation to the current time.
 *
 * \par Assumptions and Limitations
 *  - The orientation has been computed.
 *  - Rotation is constant during the update interval.
 * \param[in] to_time Target dynamic time\n Units: s
 */
void
EphemerisZXZOrientation::propagate (
   double to_time)
{
   RefFrameRot * ref_state = &(target_frame->state.rot);

   double deltat;
   double hwdt, hwdtsq;
   double coshwdt, sinhwdt;
   Quaternion q;


   // Calculate the time change and return if too small.
   deltat = to_time - update_time;
   if ((deltat > -EPSILON_TIME) && (deltat < EPSILON_TIME)) {
      return;
   }


   // The propagated quaternion is
   //   Q_prop = exp(-wmag*dt/2*[0;what]) * Q
   // The quaternion exponential is exactly
   //   exp(-wmag*dt/2*[0;what]) = [cos(wmag*dt/2), sin(wmag*dt/2)*what]
   // Approximate cos(wmag*dt/2), sin(wmag*wdt/2) via 2-term Taylor expansions
   // for small deltat.
   hwdt   = 0.5 * ref_state->ang_vel_mag * deltat;
   if (hwdt < TAYLOR_CUTOFF) {
      hwdtsq = hwdt * hwdt;
      coshwdt = 1.0 - 0.5*hwdtsq;
      sinhwdt = hwdt * (1.0 - (1.0/6.0)*hwdtsq);
   }

   // Over limit: Calculate cos(wmag*dt/2), sin(wmag*wdt/2) via math lib.
   else {
      coshwdt = std::cos(hwdt);
      sinhwdt = std::sin(hwdt);
   }

   // Compute the quaternion exp(wmag*dt/2*what)
   q.scalar = coshwdt;
   Vector3::scale (ref_state->ang_vel_unit, sinhwdt, q.vector);

   // Propagate the ephemeris-based quaternion to the current time via
   //   Q(t+deltat) = exp(-wmag*dt/2*what)*Q(t)
   q.conjugate_multiply (ref_state->Q_parent_this);
   q.normalize_integ (ref_state->Q_parent_this);

   // Compute the corresponding transformation matrix.
   ref_state->compute_transformation ();

   // Set timestamps.
   update_time = to_time;
   target_frame->set_timestamp (update_time);
}


/**
 * Compute a JEOD rotational state given a 3-1-3 inertial-to-planet-fixed
 * Euler sequence and the time derivatives of the Euler angles.
 * \param[in] angles zxz Euler angles\n Units: r
 * \param[in] derivs zxz Euler angle time derivatives\n Units: r/s
 * \param[in] time Update time\n Units: s
 */
void
EphemerisZXZOrientation::update (
   const double * angles,
   const double * derivs,
   double time)
{
   RefFrameRot * ref_state = &(target_frame->state.rot);

   double phi, theta, psi;
   double phidot, thetadot, psidot;
   double cosphi, costheta, cospsi;
   double sinphi, sintheta, sinpsi;
   double htheta, hphi_psi_dif, hphi_psi_sum;
   double coshtheta, sinhtheta;
   double coshphi_psi_dif, sinhphi_psi_dif;
   double coshphi_psi_sum, sinhphi_psi_sum;


   // Extract Euler angles and their rates from input vectors.
   phi   = euler_angle_313[0] = angles[0];
   theta = euler_angle_313[1] = angles[1];
   psi   = euler_angle_313[2] = angles[2];
   phidot   = euler_rate_313[0] = derivs[0];
   thetadot = euler_rate_313[1] = derivs[1];
   psidot   = euler_rate_313[2] = derivs[2];


   // Compute sines, cosines of the Euler angles.
   cospsi   = std::cos(psi);
   sinpsi   = std::sin(psi);
   costheta = std::cos(theta);
   sintheta = std::sin(theta);
   cosphi   = std::cos(phi);
   sinphi   = std::sin(phi);


   // Construct the inertial to planet-fixed transformation matrix as a
   // yaw-roll-yaw Euler rotation:
   //   T_parent_this = T_yaw(psi)*T_roll(theta)*T_yaw(phi)
   ref_state->T_parent_this[0][0] =  cospsi*cosphi - sinpsi*costheta*sinphi;
   ref_state->T_parent_this[0][1] =  cospsi*sinphi + sinpsi*costheta*cosphi;
   ref_state->T_parent_this[0][2] =                  sinpsi*sintheta;

   ref_state->T_parent_this[1][0] = -sinpsi*cosphi - cospsi*costheta*sinphi;
   ref_state->T_parent_this[1][1] = -sinpsi*sinphi + cospsi*costheta*cosphi;
   ref_state->T_parent_this[1][2] =                  cospsi*sintheta;

   ref_state->T_parent_this[2][0] =  sintheta*sinphi;
   ref_state->T_parent_this[2][1] = -sintheta*cosphi;
   ref_state->T_parent_this[2][2] =  costheta;


   // Construct the body's rotation rate vector (planet-fixed coordinates),
   // derived by
   //   dT'/dt = T'*skew(w)
   // where
   //   T' = transpose(T)
   //   skew(w) = skew-symmetric cross product matrix generated by w
   // Thus
   //   skew(w) = T*dT'/dt
   //           = T*(psidot*dT/dsi + thetadot*dT/dtheta + phidot*dT/dphi)'
   // From which the equations below follow.
   ref_state->ang_vel_this[0] = phidot*sintheta*sinpsi + thetadot*cospsi;
   ref_state->ang_vel_this[1] = phidot*sintheta*cospsi - thetadot*sinpsi;
   ref_state->ang_vel_this[2] = phidot*costheta        + psidot;


   // Compute data needed for propagating the lunar RNP between updates.
   ref_state->ang_vel_mag = Vector3::vmag (ref_state->ang_vel_this);
   Vector3::scale (ref_state->ang_vel_this, 1.0 / ref_state->ang_vel_mag,
                   ref_state->ang_vel_unit);


   // Construct the inertial to planet-fixed left transformation quaternion as
   // a * yaw-roll-yaw Euler rotation:
   //   Q_parent_this = Q_yaw(psi)*Q_roll(theta)*Q_yaw(phi)
   htheta       = 0.5 * theta;
   hphi_psi_dif = 0.5 * (phi - psi);
   hphi_psi_sum = 0.5 * (phi + psi);

   coshtheta = std::cos(htheta);
   sinhtheta = std::sin(htheta);
   coshphi_psi_dif = std::cos(hphi_psi_dif);
   sinhphi_psi_dif = std::sin(hphi_psi_dif);
   coshphi_psi_sum = std::cos(hphi_psi_sum);
   sinhphi_psi_sum = std::sin(hphi_psi_sum);

   ref_state->Q_parent_this.scalar    =  coshphi_psi_sum * coshtheta;
   ref_state->Q_parent_this.vector[0] = -coshphi_psi_dif * sinhtheta;
   ref_state->Q_parent_this.vector[1] = -sinhphi_psi_dif * sinhtheta;
   ref_state->Q_parent_this.vector[2] = -sinhphi_psi_sum * coshtheta;


   // Timestamp the reference state.
   update_time = time;
   target_frame->set_timestamp (update_time);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
