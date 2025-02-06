/* *

PURPOSE:
   (Logging for the aerodynamics verification sim)

LIBRARY DEPENDENCY:
   ((aero_logging.o))

ASSUMPTIONS:

   ((ONLY TO BE USED WITH THE AERO VERIFICATION.
     THERE IS NO ERROR CHECKING))



*/

#include "../include/aero_logging.hh"
#include "interactions/aerodynamics/include/aero_surface.hh"
#include "trick_utils/math/include/trick_math.h"

//! Namespace jeod
namespace jeod {

void AeroLogging::aero_logging_initialize(
   AeroSurface& surface){

   for(unsigned int ii = 0; ii < surface.facets_size; ++ii){
      plates[ii].plate =
         dynamic_cast<FlatPlateAeroFacet*> (surface.aero_facets[ii]);
   }
   size_of_plates = surface.facets_size;
}

void AeroLogging::aero_logging(
   double inert_vel[3],
   double mass,
   double force[3]){

   for(unsigned int ii = 0; ii < size_of_plates; ++ii){
      plates[ii].force_n = plates[ii].plate->force_n;
      plates[ii].force_t = plates[ii].plate->force_t;

      V_COPY(plates[ii].force, plates[ii].plate->force);
      V_COPY(plates[ii].torque, plates[ii].plate->torque);

      double u_vel_struct[3];
      V_NORM(u_vel_struct, inert_vel);

      // get the angle of attack for both sides of the plate
      // (plate 1 and plate 2)

      plates[ii].sin_alpha = V_DOT(plates[ii].plate->normal, u_vel_struct);
      double norm_to_v_on_plate[3];

      V_CROSS(norm_to_v_on_plate, plates[ii].plate->normal, u_vel_struct);
      V_NORM(norm_to_v_on_plate, norm_to_v_on_plate);

      double tangent[3];
      V_CROSS(tangent, norm_to_v_on_plate, plates[ii].plate->normal);
      V_NORM(tangent, tangent);

      plates[ii].cos_alpha = V_DOT(tangent, u_vel_struct);

      plates[ii].angle_attack = asin(plates[ii].sin_alpha);
      if(plates[ii].angle_attack < 0) {
         plates[ii].angle_attack += M_PI;

      }

   }// plates for loop

   /* Compute acceleration in the structural frame */
   V_SCALE(accel, force, 1/mass);

   accel_mag = V_MAG(accel);

   /* compute ratio of accelerations */
   if(accel[2] >= 1e-12){
      accel_ratio = accel[0] / accel[2];
   }
   else {
      accel_ratio = 0.0;
   }


}
} // End JEOD namespace
