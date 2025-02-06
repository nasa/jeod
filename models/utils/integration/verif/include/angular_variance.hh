//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/*******************************************************************************

Purpose:
  (Define the class AngularVariance, which is used to compute the error between
   the true and integrated rotational state and to model the behavior of the
   rotational simple harmonic oscillator.)

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((angular_variance.o))

 

*******************************************************************************/


#ifndef JEOD_ANGULAR_VARIANCE_HH
#define JEOD_ANGULAR_VARIANCE_HH

// System includes

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/quaternion/include/quat.hh"


//! Namespace jeod
namespace jeod {

/**
  Model the accumulated angular deviation from some reference orientation.
 */
class AngularVariance {

   JEOD_MAKE_SIM_INTERFACES(AngularVariance)

// Member data
private:

   double theta_; /* trick_units(rad) @n
      Accumulated angular variance. */

   double uhat_[3]; /* trick_units(--) @n
      Eigen axis unit vector. */


   int nwrap_; /* trick_units(--) @n
      Number of times angular variance has crossed pi. */


   double t_prev_; /* trick_units(rad) @n
      Previous eigen angle from the eigen rotation decomposition of the
      previous left transformation quaternions. */

   double u_prev_[3]; /* trick_units(--) @n
      Previous eigen axis from the eigen rotation decomposition of the
      previous left transformation quaternions. */

   double w_prev_[3]; /* trick_units(rad/s) @n
      Previous angular rate. */


// Member functions
public:
   // Default constructor
   AngularVariance (void);

   // Non-default constructor
   AngularVariance (
      const Quaternion & q_init,
      const double w_init[3]);

   // Copy constructor
   AngularVariance (const AngularVariance &src);

   // Assignment operator
   AngularVariance & operator= (const AngularVariance &src);

   // Accessor methods
   int nwrap (void) const {return nwrap_; }
   double theta (void) const {return theta_; }
   const double * uhat (void) const {return uhat_; }


   // Initialize an AngularVariance instance
   void initialize (
      const Quaternion & q_init,
      const double w_init[3]);

   // Update an AngularVariance instance (various interfaces)
   void update (
      const Quaternion & q,
      const double w[3]);

   void update (
      const Quaternion & qprop,
      const Quaternion & qref,
      const double w_prop[3]);

   void update (
      const Quaternion & qprop,
      const Quaternion & qref,
      const double w_prop[3],
      const double w_ref[3]);
};


/*
Purpose:
  (Construct an AngularVariance instance.)
*/
inline
AngularVariance::AngularVariance (  // Return: -- N/A (constructor)
   void)
:
   theta_(0.0),
   nwrap_(0),
   t_prev_(0.0)
{
   Vector3::initialize (uhat_);
   Vector3::initialize (u_prev_);
}


/*
Purpose:
  (Construct an AngularVariance instance.)
*/
inline
AngularVariance::AngularVariance (  // Return: -- N/A (constructor)
   const Quaternion & q_init,       // In:     -- Initial quaternion
   const double w_init[3])          // In:     -- Initial rate
{
   initialize (q_init, w_init);
}


/*
Purpose:
  (Construct an AngularVariance instance.)
*/
inline
AngularVariance::AngularVariance (  // Return: -- N/A (constructor)
   const AngularVariance &src)      // In:     -- Source variance
{
   *this = src;
}


/*
Purpose:
  (Copy an AngularVariance instance.)
*/
inline AngularVariance &
AngularVariance::operator= (        // Return: -- *this
   const AngularVariance &src)      // In:     -- Source variance
{
   theta_  = src.theta_;
   t_prev_ = src.t_prev_;
   nwrap_  = src.nwrap_;
   Vector3::copy (src.uhat_, uhat_);
   Vector3::copy (src.u_prev_, u_prev_);
   Vector3::copy (src.w_prev_, w_prev_);
   return *this;
}


} // End JEOD namespace

#endif
