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
   ((gravity_gradient.cc))


*/

// System includes
#include <math.h>

// JEOD includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/gravity_gradient.hh"


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
   double gg_torq[3]    ) /* OUT: NM   Gravity gradient torque, Struct Ref.*/

{
   /* Declare working parameteres. */
   double pos_bdy[3]; /* Position of the vehicle in vehicle body ref frame. */
   double rsq;
   double fact;
   double IR[3];
   double RxIR[3];

   /* Only compute the torque if active. */
   if ( active ) {

      /* Transform vehicle inertial position vector to body coordinates. */
      Vector3::transform(T_b_I, pos, pos_bdy);

      /*
       * Compute the vehicle body frame gravity gradient
       *    torq = 3*GM*(R cross inertia*R)/R^5
       * where
       *       R = pos_bdy.
       */

      /* Compute R^2. */
      rsq = Vector3::dot( pos_bdy, pos_bdy );

      /* Check for small position vectors. */
      if ( rsq > 1e-12 ) {

         /* Compute gravity torque factor (3 * GM / R^5). */
         fact = 3.0 * mu / (rsq * rsq * sqrt(rsq));

         /* Compute R cross IR. */
         Vector3::transform(inertia, pos_bdy, IR);
         Vector3::cross(pos_bdy, IR, RxIR);

         /* Compute the gravity gradient torque in the body frame. */
         Vector3::scale(RxIR, fact, gg_torq_bdy);

         /* Convert to structural coordinates. */
         Vector3::transform_transpose(T_s_b, gg_torq_bdy, gg_torq);

      } else {

         /* Zero out the torque vectors. */
         Vector3::initialize( gg_torq_bdy );
         Vector3::initialize( gg_torq );

      }

   }
   else {

      /* Zero out the torque vectors. */
      Vector3::initialize( gg_torq_bdy );
      Vector3::initialize( gg_torq );

   }

   /* Always return zero. */
   return( 0 );

}
} // End JEOD namespace
