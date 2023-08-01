/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/flat_plate_aero_facet.cc
 * Individual facets for use with aero environment interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

Library dependencies:
    ((flat_plate_aero_facet.cc)
     (aero_facet.cc)
     (aerodynamics_messages.cc)
     (utils/message/src/message_handler.cc))


*******************************************************************************/

#include <cstddef>

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/surface_model/include/facet.hh"

// Model includes
#include "../include/flat_plate_aero_facet.hh"
#include "../include/aerodynamics_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

FlatPlateAeroFacet::FlatPlateAeroFacet (
   void)
: // Return: -- void
   center_pressure(nullptr),
   normal(nullptr),
   force_n(0.0),
   force_t(0.0),
   coef_method(AeroDragEnum::Specular),
   calculate_drag_coef(false),
   epsilon(0.0),
   temp_reflect(0.0),
   drag_coef_norm(0.0),
   drag_coef_tang(0.0),
   drag_coef_spec(0.0),
   drag_coef_diff(0.0),
   temperature(0.0),
   area(0.0)
{
   return;
}

/**
 * Destructor
 */

FlatPlateAeroFacet::~FlatPlateAeroFacet (
   void)
{
   // empty for now

}

/**
 * The FlatPlateAeroFacet specific implementation of aerodynamic
 * drag force, based on the given parameters
 * \param[in] rel_vel_mag The magnitude of the relative velocity\n Units: M/s
 * \param[in] rel_vel_struct_hat The unit vector of the total relative velocity, in the structural frame
 * \param[in] aero_drag_param_ptr The aerodynamic drag parameters used for drag calculation
 * \param[in] center_grav The center of gravity of the vehicle, in the structural frame\n Units: M
 */

void
FlatPlateAeroFacet::aerodrag_force (
   const double rel_vel_mag,
   const double rel_vel_struct_hat[3],
   AeroDragParameters* aero_drag_param_ptr,
   double center_grav[3])
{
   double sin_alpha; /* --
      alpha is the angle of attack, the angle between the plate and the relative
       velocity vector of the palte w.r.t. the ambient gas; this is the sine of
       that angle.  */
   double force_base; /* N
      common force calculation, before scaling with the appropriate drag
       coefficient ( = dynamic pressure * area)  */
   double vec_force_n[3]; /* N vector force normal to the plate  */
   double vec_force_t[3]; /* N
      vector force tangential to the plate, or parallel with the velocity vector,
       depending on application */
   double tangent[3]; /* --
      the projection of the velocity vector onto the plate, tangential to the
       plate */
   Vector3::initialize(tangent);

   // The base facet's temperature can be updated from dynamically, so copy
   // that over.

   temperature = base_facet->temperature;

   // alpha is the angle of attack, the angle between the plate and the velocity
   // vector (w.r.t. wind).

   sin_alpha = Vector3::dot (normal, rel_vel_struct_hat);
   // If the plate is not windward-facing, it has no aerodynamic drag on it.
   if (sin_alpha <= 0.0) {
      Vector3::initialize (force);
      force_n = 0.0;
      force_t = 0.0;
      return;
   }

   // consistency check
   if ((calculate_drag_coef == false) &&
       (coef_method == AeroDragEnum::Calc_coef)) {
      MessageHandler::warn (
         __FILE__, __LINE__,
         AerodynamicsMessages::runtime_warns,
         "Must have calculate_drag_coef set to 'true' if \n"
         "using the calc_coef method of obtaining the drag coefficient.\n"
         "Resetting the value of calculate_drag_coef.\n");
      calculate_drag_coef = true;
   }


   if (calculate_drag_coef == true) {

      if (std::fpclassify(aero_drag_param_ptr->gas_const) == FP_ZERO ||
          std::fpclassify(aero_drag_param_ptr->temp_free_stream) == FP_ZERO) {
         MessageHandler::fail (
            __FILE__, __LINE__, AerodynamicsMessages::runtime_error,
            "Either the gas_const or temp_free_stream field(s) of "
            "aero_drag_param_ptr was not initialized.  "
            "Please initialize both of these values.");
      }

      /* --
      dimensionless parameter equal to the square root of the ratio of the
       vehicle velocity to the mean free-molecular thermal speed (not
       velocity, not relative speed) */
      double s = rel_vel_mag / sqrt (2.0 * aero_drag_param_ptr->gas_const *
                              aero_drag_param_ptr->temp_free_stream);

      double s_2; // s^2 (see definition of s)
      double exp_ssa2; // e^(-s_sa2) 
      double temp_ratio; // ratio of the temperature of the reflected molecules to that of the incident molecules
      switch (coef_method) {

      case AeroDragEnum::Calc_coef:
      {
         /* Calculate variables used in coef calcs */
         // This is an implementation of the coefficient calculations found
         // in the documentation
         double one_p_epsilon =  1 + epsilon; /* epsilon is the fraction of incident molecules that "bounce"; this is 1+epsilon */
         double one_m_epsilon =  1 - epsilon; /* 1-epsilon (see one_p_epsilon)  */
         s_2           = s * s;
         double s_sinalpha    = s * sin_alpha;
         double s_sa2         = s_sinalpha * s_sinalpha;
         exp_ssa2      = exp (-s_sa2);

         double erf_ssa = erf (s_sinalpha);

         // temperature of the reflected molecules
         double local_temp_reflect = (one_m_epsilon) * temperature +
                        epsilon * aero_drag_param_ptr->temp_free_stream;
         // ratio of the temperature of the reflected molecules to that of the incident molecules
         temp_ratio = local_temp_reflect / aero_drag_param_ptr->temp_free_stream;


         drag_coef_norm = (
            (M_2_SQRTPI * one_p_epsilon) * s_sinalpha * exp_ssa2 +
            one_m_epsilon * sqrt (temp_ratio) *
            (2.0 / M_2_SQRTPI) * s_sinalpha +
            one_p_epsilon * (1.0 + 2.0 * s_sa2) * erf_ssa) /
                          (s_2);

         // if plate is full-on (alpha = 90 degrees), there is no tangential drag
         if (!Numerical::compare_exact(sin_alpha,1)) {
            double cos_alpha = sqrt (1 - (sin_alpha * sin_alpha));
            Vector3::scale (normal, sin_alpha, tangent);
            Vector3::diff (rel_vel_struct_hat, tangent, tangent);
            double cos_a_inv = 1 / cos_alpha;
            Vector3::scale (cos_a_inv, tangent);

            drag_coef_tang = fabs (
               (2.0 * one_m_epsilon / (2.0 / M_2_SQRTPI)) * s *
               cos_alpha * (exp (-s_sinalpha * s_sinalpha) +
                            (2.0 / M_2_SQRTPI) * s_sinalpha *
                            erf (s_sinalpha))) / (s * s);
            // Note that fabs was added to the equation because direction is
            // accounted for later
         }
         break;
      }
      case AeroDragEnum::Specular:
      {
         // assumes all molecules "bounce" from the plate

         // Calculate the drag coef
         // assumes angle of attack = 90 degrees for unknown reason, therefore
         // s_sinalpha = s
         // assumes epsilon = 1; so 1+epsilon =2; 1-epsilon = 0; 2*(1+epsilon)=4
         s_2            = s * s;
         exp_ssa2       = exp (-s_2);
         drag_coef_spec = ((2.0 * M_2_SQRTPI) * s * exp_ssa2 +
                           (2.0 + 4.0 * s_2)) / (s_2);
         break;
      }
      case AeroDragEnum::Diffuse:
      {
         // Calculate the drag coef
         // assumes angle of attack = 90 degrees for unknown reason, therefore
         // s_sinalpha = s
         // assumes epsilon = 0; so 1+epsilon = 1-epsilon = 1; 2*(1+epsilon) = 4
         s_2      = s * s;
         exp_ssa2 = exp (-s_2);

         /* Assume the temperature of reflected molecules */
         temp_ratio = temperature / aero_drag_param_ptr->temp_free_stream;

         drag_coef_diff = ((M_2_SQRTPI)*s * exp_ssa2 +
                           sqrt (temp_ratio) * (2.0 / M_2_SQRTPI) * s +
                           (1.0 + 2.0 * s_2)) / (s * s);
         break;
      }
      case AeroDragEnum::Mixed:
      {
         s_2            = s * s;
         exp_ssa2       = exp (-s_2);
         drag_coef_spec = ((2.0 * M_2_SQRTPI) * s * exp_ssa2 +
                           (2.0 + 4.0 * s_2)) / (s_2);
         temp_ratio     = temperature / aero_drag_param_ptr->temp_free_stream;
         drag_coef_diff = ((M_2_SQRTPI)*s * exp_ssa2 +
                           sqrt (temp_ratio) * (2.0 / M_2_SQRTPI) * s +
                           (1.0 + 2.0 * s_2)) / (s * s);
         break;
      }
      default:
      MessageHandler::fail (
         __FILE__, __LINE__, AerodynamicsMessages::runtime_error,
         "The choice for calculating the coefficients of drag in the "
         "flat plate model for aerodynamics was invalid. Please supply "
         "a valid choice.\n");
      }
   }

   //  Have calculated drag coeffs if desired, or using pre-defined values
   //  otherwise.  Now calculate the forces.
   //  force_base is negative because the force is opposite in direction to all
   //  the vectors we know.
   force_base = -aero_drag_param_ptr->dynamic_pressure * area;

   switch (coef_method) {

   case AeroDragEnum::Calc_coef:
      force_n = force_base * drag_coef_norm;


      // if plate is full-on (alpha = 90 degrees)
      if (Numerical::compare_exact(sin_alpha,1)) {
         Vector3::scale (normal, force_n, force);
         force_t = 0.0;
      }
      else {
         force_t = force_base * drag_coef_tang;
         Vector3::scale (tangent, force_t, vec_force_t);
         Vector3::scale (normal, force_n, vec_force_n);
         Vector3::sum (vec_force_t, vec_force_n, force);
      }
      break;

   case AeroDragEnum::Specular:
      // For the Specular case the atmospheric particles "bounce" from the
      // the plates after a collision creating a force that is normal to the
      // area of impact. The specular drag force is proportional to the product
      // of the plate are and the square of the relative velocity normal to
      // the plate area.
      // Velocity normal = V*sin(alpha), therefore,
      // Vnormal^2 = V^2*sin^2(alpha) */

      force_n = force_base * drag_coef_spec * sin_alpha * sin_alpha;
      // The force is applied in the normal direction
      Vector3::scale (normal, force_n, force);
      break;


   case AeroDragEnum::Diffuse:
      // For the Diffuce case the atmospheric particles "stick" to the plates
      // after a collision, creating a force that is parallel to the relative
      // velocity vector of a body. The diffuse drag force is proportional to
      // the product of the plate area projected in a direction normal to the
      // relative velocity and the square of the relative velocity.
      // Plate area projected in the direction normal to the relative velocity =
      // area * sin(alpha)

      // force_t is not the force normal to the plate in this case; it
      // is the force parallel to the velocity.
      force_t = force_base * drag_coef_diff * sin_alpha;

      // The force is applied in the direction of the velocity
      Vector3::scale (rel_vel_struct_hat, force_t, force);

      break;

   case AeroDragEnum::Mixed:

      // The calculations for the Mixed case have been updated from previous
      // releases.  The formulation is more intuitive, easier to read, and
      // produces the exact same output as before.

      // Drag is a mixture of specular (along plate normal) and diffuse (along
      // relative velocity vector) forces.  These are summed to produce the
      // total drag force.


      force_n = epsilon * force_base * drag_coef_spec *
                sin_alpha * sin_alpha;
      force_t = (1.0 - epsilon) * force_base *
                drag_coef_diff * sin_alpha;

      // calculate the force on the plate
      Vector3::scale (normal, force_n, vec_force_n);
      Vector3::scale (rel_vel_struct_hat, force_t, vec_force_t);
      Vector3::sum (vec_force_t, vec_force_n, force);

      break;

   default:
      MessageHandler::fail (
         __FILE__, __LINE__, AerodynamicsMessages::runtime_error,
         "The choice for calculating the aerodynamic drag in the "
         "flat plate model for aerodynamics was invalid. Please supply "
         "a valid choice.\n");
   }

   double cpres[3];

   Vector3::diff (center_pressure, center_grav, cpres);
   Vector3::initialize (torque);
   Vector3::cross_incr (cpres, force, torque);
   return;
   // empty for now

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
