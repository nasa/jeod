//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
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
  (Class for testing gravity models.)

Assumptions and Limitations:
  ((Assumes a system of 12 point masses.))

Library Dependency:
   ((../src/gravity_verif.cc))


*******************************************************************************/
#ifndef JEOD_GRAVITY_VERIF_HH
#define JEOD_GRAVITY_VERIF_HH

//! Namespace jeod
namespace jeod {

/*****************************************************************************
PointMassProperties
Purpose: Decompose the gravity body into a set of these points.
*****************************************************************************/
class PointMassProperties
{
 // Member data
 public:
   double mass; /* trick_units(kg) @n
      mass */
   double latitude; /* trick_units(rad) @n
      latitude*/
   double longitude; /* trick_units(rad) @n
      longitude*/
   double radius; /* trick_units(m) @n
      radius (from planet-center) */
   double position[3]; /* trick_units(m) @n
      position vector (expressed in pfix coords or equivalently in inertial).*/

   void generate_position();
};

/*****************************************************************************
VehiclePointProperties
Purpose:(Decompose the vehicles to a set of these points)
*****************************************************************************/
class VehiclePointProperties
{
 public:
  double mass;
  double position[3];  /* trick_units(m) @n
      position of vehicle point in vehicle body-frame.*/
};


/*****************************************************************************
GravityVerification
Purpose:(Compares the gravity and gravity torque outputs against a set of data
         generated for a finite set of mass points interacting with
         another finite set of mass points.)
*****************************************************************************/
class GravityVerification
{
 // Member data
 public:
  static const unsigned int num_pts = 12; /* trick_units(count) @n
     Number of point gravity-masses */
  static const unsigned int num_veh_pts = 3; /* trick_units(count) @n
     Number of point masses in the vehicle */
  double rad_per_deg; /* trick_units(--) @n
     For conversion from degrees to radians */
  double G; /* trick_units( m3/kg/s2) @n
     universal gravitation constant */

  PointMassProperties gravity_pt[num_pts]; /* trick_units(--) @n
     Point mass properties */

  VehiclePointProperties vehicle_pt[num_veh_pts];

  double gravity_magnitude;
  double verif_gravity_magnitude;
  double gravity_magnitude_error;

  double verif_grav_accel[3];
  double accel_error[3];

  double verif_grav_torque[3];
  double torque_error[3];

  GravityVerification();

  void verify_acc_magnitudes( const double grav_accel[3],
                              const double inertial_position[3] );
  void verify_grav_accel( const double veh_pos_inrtl[3],
                          const double grav_accel_inrtl[3]);

  void verify_grav_torque( const double sc_attitude[3], // PYR in degrees
                           const double veh_pos_pfix[3], // pos of veh in pfix
                           const double grav_torque[3]);
};
} // End JEOD namespace

#endif
