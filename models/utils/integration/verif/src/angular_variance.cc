/*******************************************************************************

Purpose:
  (Define methods for the class AngularVariance.)

Reference:
  (((TBS)))

Library dependencies:
  ((angular_variance.o)
   (utils/quaternion/quat.o)
   (utils/quaternion/quat_to_eigenrot.o))

 

*******************************************************************************/


// System includes
#include <cmath>

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/quaternion/include/quat.hh"

// Model includes
#include "../include/angular_variance.hh"


//! Namespace jeod
namespace jeod {

/*
Purpose:
  (Initialize an AngularVariance instance.)
 */
void AngularVariance::initialize (
   const Quaternion & q_init,
   const double w_init[3])
{
   nwrap_ = 0;
   q_init.left_quat_to_eigen_rotation (&theta_, uhat_);
   t_prev_ = theta_;
   Vector3::copy (uhat_,  u_prev_);
   Vector3::copy (w_init, w_prev_);
}


/*
Purpose:
  (Update an AngularVariance instance based on a new quaternion and
   angular velocity.)
Assumptions and limitations:
  ((The angular rate is sufficiently low.))
*/
void AngularVariance::update (
   const Quaternion & q,
   const double w[3])
{
   double q_theta, q_uhat[3];
   bool n_even = (nwrap_ % 2) == 0;

   q.left_quat_to_eigen_rotation (&q_theta, q_uhat);

   if (Vector3::dot (q_uhat, u_prev_) < 0.0) {
      double w_bar[3];
      bool over_top;

      Vector3::sum (w_prev_, w, w_bar);
      over_top = ((t_prev_ + q_theta) > 1.5*M_PI) ||
                 (((t_prev_ + q_theta) > 0.5*M_PI) &&
                  (Vector3::dot (w_bar, u_prev_) > 0.0));

      nwrap_ += (n_even == over_top) ? 1 : -1;
      n_even  = !n_even;
   }

   if (n_even) {
      Vector3::copy (q_uhat, uhat_);
      theta_ = (static_cast <double> (nwrap_)) * M_PI + q_theta;
   }
   else {
      Vector3::negate (q_uhat, uhat_);
      theta_ = (static_cast <double> (nwrap_+1)) * M_PI - q_theta;
   }

   t_prev_ = q_theta;
   Vector3::copy (q_uhat, u_prev_);
   Vector3::copy (w,      w_prev_);
}


/*
Purpose:
  (Update an AngularVariance instance based on a new quaternion and
   angular velocity.)
*/
void AngularVariance::update (
   const Quaternion & q_prop,
   const Quaternion & q_ref,
   const double w_prop[3])
{
   Quaternion q_err;

   q_prop.multiply_conjugate (q_ref, q_err);

   update (q_err, w_prop);
}


/*
Purpose:
  (Update an AngularVariance instance based on a new quaternion and
   angular velocity.)
*/
void AngularVariance::update (
   const Quaternion & q_prop,
   const Quaternion & q_ref,
   const double w_prop[3],
   const double w_ref[3])
{
   Quaternion q_err;
   double w_err[3];

   q_prop.multiply_conjugate (q_ref, q_err);
   Vector3::diff (w_prop, w_ref, w_err);

   update (q_err, w_err);
}
} // End JEOD namespace
