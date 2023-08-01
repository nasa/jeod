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
/*
PURPOSE:
   (Single Rigid Body Gravity Gradient Torque Computation)

REFERENCE:
   (((Hughes, Peter) (Spacecraft Attitude Dynamics)
     (Dover Publications) (2004) (Pages 281-346) (ISBN:0-486-43925-9)))

ASSUMPTIONS AND LIMITATIONS:
   ((Uniform mass distribution)
    (Inertia tensor input is referenced to the composite body frame)
    (Spherical Planet)
    (Position vector is inertial referenced)
    (Symmetric Inertia tensor - only upper diagonal is used)
    (Output torques are structural frame referenced about the composite C.G.))

CLASS:
   (derivative)

LIBRARY DEPENDENCY:
   ((../src/verif_grav_torque.cc))


*/

#ifndef JEOD_VERIF_GRAV_TORQUE_H
#define JEOD_VERIF_GRAV_TORQUE_H

#include "environment/gravity/verif/include/point_mass.hh"

//! Namespace jeod
namespace jeod {

    /* ENTRY POINT: */
void verif_grav_torque(     /* RETURN: --  Always return zero.               */
   const PointMass & ptmass   , /* IN: -- grav accel */
   const double sc_mass[3][4],  /* IN: -- s/c point mass model */
   const double sc_att[3],  /* IN: -- s/c attitude euler angles */
   const double pos[3]   , /* IN: -- grav accel */
   const double RNP[3][3],  /* IN: kgM2  Inertia tensor about c.g., Body Ref.*/
   const double T_pf_bdy[3][3],  /* IN: --  Inertia tensor about c.g., Body Ref.*/
   const double sim_time      , /* OUT: NM   Gravity gradient torque, Struct Ref.*/
   const double grav_accel[3]   , /* IN: -- grav accel */
   const double grav_torque[3]   , /* IN: -- grav accel */
   const double grav_pot    );   /* IN: -- grav potential */

} // End JEOD namespace

#endif
