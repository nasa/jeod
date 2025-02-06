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
   ((gravity_gradient.o))


*/

#ifndef JEOD_GRAVITY_GRADIENT_VERIF_FUNCTION_HH
#define JEOD_GRAVITY_GRADIENT_VERIF_FUNCTION_HH

/* System includes. */
#include <math.h>

/* Trick includes. */
#include "sim_services/include/Flag.h"
#include "trick_utils/math/include/vector_macros.h"
#include "trick_utils/math/include/matrix_macros.h"

//! Namespace jeod
namespace jeod {

    /* ENTRY POINT: */
int gravity_gradient(     /* RETURN: --  Always return zero.               */
   int    active,         /* IN: --    Gravity gradient on/off flag.       */
   double mu,             /* IN: M3/s2 Planetary gravitational parameter.  */
   double pos[3],         /* IN: M     Position of c.g., Inrtl Ref.        */
   double T_b_I[3][3],    /* IN: --    Body to Inertial attitude matrix.   */
   double T_s_b[3][3],    /* IN: --    Structure to Body attitude matrix.  */
   double inertia[3][3],  /* IN: kgM2  Inertia tensor about c.g., Body Ref.*/
   double gg_torq_bdy[3], /* OUT: NM   Gravity gradient torque, Body Ref.  */
   double gg_torq[3]    ); /* OUT: NM   Gravity gradient torque, Struct Ref.*/


} // End JEOD namespace

#endif
