/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_third_body.cc
 * Provides the functionality associated with RadiationThirdBodys.
 */


/******************************************************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

CLASS:
    (None)
LIBRARY DEPENDENCY:
    ((radiation_third_body.o)
     (radiation_messages.o)
     (utils/message/message_handler.o))


******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
//#include "utils/ref_frames/include/ref_frame.hh"

// Model includes
#include "../include/radiation_third_body.hh"
#include "../include/radiation_source.hh"
#include "../include/radiation_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Constructor
 */
RadiationThirdBody::RadiationThirdBody ()
   :
   primary_source_ptr(NULL),
   shadow_geometry(Conical),
   force_state_update(true),
   name(),
   active(true),
   update_interval(0.0),
   radius(0.0),
   local_frame_ptr(NULL),

   added_to_model(false),
   initialized(false),
   illum_factor(1.0),
   last_update_time(0.0),
   planet_link(NULL),
   r_plus(0.0),
   r_minus(0.0),
   r_ratio(0.0),
   r_par(0.0),
   r_perp(0.0),
   d_source_to_third(0.0)
{
   Vector3::initialize (output_flux);
   Vector3::initialize (third_to_cg_inrtl);
   Vector3::initialize (source_to_third_inrtl);
   Vector3::initialize (source_to_third_hat_inrtl);
}


/**
 * initializes the third body
 * \param[in] dyn_mgr_ptr pointer to the dynamics manager
 */
void
RadiationThirdBody::initialize (
   DynManager * dyn_mgr_ptr)
{
   if (name.empty()) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "Tried to initialize a Third Body with an empty name.\n"
         "Name must be specified in order to identify the body.\n"
         "Unable to proceed.  Terminating.\n");
      // fail out at this point.  DO NOT PROCEED.
   }

   if (primary_source_ptr == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "Initializing the RadiationThirdBody named (%s) without specifying\n"
         "the Primary Source.  primary_source_ptr is still NULL.\n"
         "This value should be assigned by the RadiationPressure top-level\n"
         "class; this error message should only be seen if the model is\n"
         "being configured using a non-recommended initialization sequence.\n"
         "Check documentation on how to configure this model.\n"
         "Unable to proceed.  Terminating.\n", name.c_str());
      // fail out at this point.  DO NOT PROCEED.
   }

   if (primary_source_ptr->inertial_frame_ptr == NULL) {
      MessageHandler::fail(
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "While initializing the RadiationThirdBody named (%s), it was\n"
         "detected that the source-body's inertial-frame-pointer\n"
         "(inertial_frame_ptr) has not been set.  This pointer must be\n"
         "set before the RadiationThirdBody can be initialized.\n", name.c_str());
      // fail out at this point.  DO NOT PROCEED.
   }

   if (primary_source_ptr->radius <= 0.0) {
      MessageHandler::fail(
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "While initializing the RadiationThirdBody named (%s), it was\n"
         "detected that the source-body's radius has an invalid value.\n"
         "This radius must be positive and must be set\n"
         "before the RadiationThirdBody can be initialized.\n", name.c_str());
      // fail out at this point.  DO NOT PROCEED.
   }

   planet_link = dyn_mgr_ptr->find_planet (name.c_str());
   if (planet_link == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "Tried to initialize a Third Body with a name (%s) that is not\n"
         "recognized in the Dynamics Manager's list of planetary bodies or\n"
         "dynamic bodies.\nUnable to proceed.  Terminating.\n", name.c_str());
      // fail out at this point.  DO NOT PROCEED.
   }

//  TODO STUB:  This section allows for assignment of a RadiationThirdBody
//  tag to a DynBody.  That is not necessary at this time.
//  Replace failure outcome above by providing option to test "name" for being
//  the name of a DynBody:
//   if (planet_link == NULL) {
//      DynBody * dyn_body_link = dyn_mgr_ptr->find_dyn_body (name.c_str());
//      if (dyn_body_link == NULL) {
//         FAIL_OUT (both cases failed)
//      }
//      local_frame_ptr = &(dyn_body_link->composite_body);
//      radius = ...
//      subscribe_to_frame = ...
//   }
//   else {
//       local_frame_ptr = &(planet_link->pfix);
//       radius  = planet_link->r_eq;
//       subscribe_to_frame = planet_link->inertial;
//   }
   local_frame_ptr = &(planet_link->pfix);
   if (local_frame_ptr == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "Could not find a pfix frame associated with planet (%s)\n"
         "Unable to proceed.  Terminating.\n", name.c_str());
      // fail out at this point.  DO NOT PROCEED.
   }


   radius  = planet_link->r_eq;
   if (radius <= 0.0) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "RadiationThirdBody Object idenitified by name (%s) has a radius "
         "of %f on record.\nThis is not a valid value; radius must be "
         "positive.\nUnable to proceed.  Terminating.\n", name.c_str(), radius);
   }

   if (active) {
      dyn_mgr_ptr->subscribe_to_frame (planet_link->inertial);
   }

   r_plus  = radius + primary_source_ptr->radius;
   r_minus = radius - primary_source_ptr->radius;
   r_ratio = radius / primary_source_ptr->radius;

   // All checks passed, all settings valid.  Set initialized flag.
   initialized = true;
}



/**
 * Calculates the effect of shadowing by a third body.
 * NOTE -
 *      This method is intended to be an internal call so should be protected
 *      However, doing so would be a change to the API.  Delay for JEOD4.0.
 */
void
RadiationThirdBody::calculate_shadow (
   void)
{
   // NOTE - this check would not be needed if this method were protected
   //        because it would already be guaranteed by passing the same check
   //        at the entry to process_third_body()
   //        Likewise, the d_source_to_third check below must have been
   //        checked in update_body_state().
   if (!initialized) {
      MessageHandler::error(
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "RadiationThirdBody::calculate_shadow() called without the model "
         "being initialized.\nAborting.\n"
         "Check documentation for required configuration.\n");
      return;
   }

   Vector3::diff (primary_source_ptr->source_to_cg,
                  source_to_third_inrtl,
                  third_to_cg_inrtl);

   // r_par is the component of that vector aligned with the sun-body vector
   // r_perp is the component perpendicular to r_par.
   r_par = Vector3::dot (third_to_cg_inrtl, source_to_third_hat_inrtl);

   // If the two vectors (source-to-third-body, third-body-to-vehicle) have
   // opposing directions, the third-body cannot be casting a shadow on the
   // vehicle:
   if (r_par < 0) {
      illum_factor = 1;
      return;
   }

   // Otherwise, continue with the calculation.
   // The algorithm is explained in greater detail in the documentation

   // Compute the square of the distance between the vehicle and the
   // third-body.
   double r_mag2 = Vector3::vmagsq (third_to_cg_inrtl);
   if (r_mag2 <= 0.0) {
      MessageHandler::error(
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "Numerical error detected; the distance between the third-body "
         "\nand the vehicle has an illegal value (r-squared = %20.16f)\n"
         "for RadiationThirdBody (%s).\n"
         "Putting the vehicle in total shadow and exiting.\n",
          r_mag2, name.c_str());
      illum_factor = 0.0;
      return;
   }

   // Compute the distance off the line connecting the two planetary bodies
   double r_perp2 = r_mag2 - r_par * r_par;
   if (r_perp2 <= 0.0) {
      r_perp = 0.0;
   } else {
      r_perp = sqrt (r_perp2);
   }


   if (shadow_geometry == Cylindrical ||
       shadow_geometry == Cyl) {
      // Cylindrical shadow (this is the easy one)
      // if vehicle is outside the cylindrical radius, it is outside the
      // shadow.  Otherwise it is in full shadow,
      if (r_perp < radius) {
         illum_factor = 0;
      }
      else {
         illum_factor = 1;
      }
      // done, return
   }

   else if  (shadow_geometry == Conical ||
             shadow_geometry == Con ) {
      // Conical shadow
      // NOTE: the "regions" have been ordered based on mathematical complexity
      //       with the more complex regions coming late.  The more complex
      //       computations require some of the same variables that are needed
      //       for earlier computations.  Consequently, a design decision was
      //       made to structure this as
      //
      //        if(...) {... return}
      //        <build complexity>
      //        if(...) { ... return}
      //        etc.
      //
      //       rather than
      //        if(...) {...}
      //        else if(...) {...}
      //        etc.
      //       which would require replicating code in the respective "else"
      //       blocks

      if (d_source_to_third <= 0.0) {
         MessageHandler::error(
            __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
            "The distance between the source-body and the third-body has an\n"
            "illegal value (%20.16f) for RadiationThirdBody (%s).\n"
            "This is a major configuration problem.\n"
            "Deactivating this body.\n", d_source_to_third, name.c_str());
         active = false;
         illum_factor = 1.0;
         return;
      }



      double r_perp_x_d = r_perp * d_source_to_third;
      double radius_x_d = radius * d_source_to_third;

      if (r_perp_x_d >= (r_plus * r_par) + radius_x_d) { // Region B (none):
         illum_factor = 1;
         return;
      }
      if ( r_perp_x_d <= (r_minus * r_par) + radius_x_d) { // Region C (total):
         illum_factor = 0;
         return;
      }

      // Here is the first example of why I'm using if(){return}.  All
      // subsequent calculations will require the ratio of the angular sizes,
      // shadow-body over source, but the previous calculations did not.
      // So doing that now.  ang_ratio is shadow-body over source.
      double ang_ratio_2_a = r_ratio * primary_source_ptr->d_source_to_cg;
      double ang_ratio_2 = ang_ratio_2_a * ang_ratio_2_a / r_mag2;


      if (r_perp_x_d <= -((r_minus * r_par) + radius_x_d)) { // Region D (annular)
         illum_factor =  1 - ang_ratio_2;
         return;
      }



      double ang_ratio = sqrt (ang_ratio_2);
      // Introduce 2 new variables:
      // delta:    fraction of partial eclipse from first contact (0) to
      //           totality (1).  Note the delta here starts with only the
      //           numerator part of the delta in the document; the numerator
      //           is identical for both cases but the denominator differs.
      double delta = radius_x_d - r_perp_x_d +
                     (radius + primary_source_ptr->radius) * r_par;
      if (ang_ratio_2 >= 1) { // ThirdBody has a larger angular size
         illum_factor = 1 - generate_alpha(
                              1/ang_ratio,
                              delta / (2*primary_source_ptr->radius * r_par));
      }
      else {
         illum_factor = 1 - ang_ratio_2 *
                        generate_alpha(
                            ang_ratio,
                            delta / (2 * radius * (d_source_to_third + r_par)));
      }
      // done, return
   }

   else {
      illum_factor = 1;
   }
}

/**
 * generate_alpha( rho_adj, delta: generates an approximate alpha-value given
 * a relative disk size and fraction of maximum possible eclipse
 * \param[in] rho_adj relative disk size
 * \param[in] delta fraction of eclipse achieved.
 */
double
RadiationThirdBody::generate_alpha (
   double rho_adj,
   double delta)
{
   // Coefficients are empirically derived to provide close match
   // to circular geometry.  See documentation for origin of
   // these values.
   double a3 =  0.758656 * rho_adj * rho_adj +
               -0.0637441 * rho_adj -
                1.08955;
   double a2 = -1.2205 * rho_adj * rho_adj +
                0.61815 * rho_adj +
                1.62909;
   double a1 =  0.43723 * rho_adj * rho_adj +
               -0.52921 * rho_adj +
                0.473649;
   double a0 = -0.01242 * rho_adj * rho_adj * rho_adj +
               -0.0036715 * rho_adj * rho_adj +
                0.0150263 * rho_adj +
               -0.0078259;
   return a3 * delta * delta * delta +
          a2 * delta * delta +
          a1 * delta +
          a0;
}

/**
 * convert from the old style framework in which the shadow geometry was
 * owned by the RadiationSource (same for all third bodies) to the new style
 * framework where the geometry is owned by each ThirdBody
 * \param[in] old_shadow_geometry integer representation of the shadow geometry enumeration.
 */
void
RadiationThirdBody::convert_shadow_from_int(
      int old_shadow_geometry)
{

   shadow_geometry = static_cast<ShadowGeometry>(old_shadow_geometry);
   MessageHandler::inform (
     __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
     "Converting from old to new enumeration types for shadow geometry.\n"
     "Enumerated value is %i, check that the enumeration contained in \n"
     "RadiationSource::OldShadowGeometry matches with that in\n"
     "RadiationThirdBody::ShadowGeometry for element %i",
     shadow_geometry,shadow_geometry);
}


/**
 * Primary executable for handling the Third Body effects
 * @return shading fraction
 * \param[in] real_time Current time.\n Units: s
 * \param[in] veh_struc_frame Unused.
 */
double
RadiationThirdBody::process_third_body (
    double real_time,
    RefFrame & veh_struc_frame JEOD_UNUSED )
{
   // NOTE - for error cases, return 1.0 which is the value associated with no
   //        shadowing.
   if (!active) {
      return 1.0;
   }

   if (!initialized) {
      MessageHandler::error(
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "RadiationThirdBody::calculate_shadow() called without the model "
         "being initialized for body (%s).\nAborting procedure.\n"
         "Check documentation for required configuration.\n"
         "Deactivating this RadiationThirdBody as a precautionary measure.\n",
         name.c_str());
      active = false;
      return 1.0;
   }


   // if state-update fails, return an illumination factor of 1.0
   if (!test_for_state_update (real_time)) {
      MessageHandler::error(
         __FILE__, __LINE__, RadiationMessages::operational_setup_error, "\n"
         "The state-update for the RadiationThirdBody (%s) failed.\n"
         "Deactivating this RadiationThirdBody.\n", name.c_str());
      active = false;
      return 1.0;
   }

   calculate_shadow();
   return illum_factor;
}

/**
 * Tests for necessity of updating third body state, and calls
 * appropriate update method (polymorphic) if needed.
 * \param[in] real_time Current time\n Units: s
 */
bool
RadiationThirdBody::test_for_state_update (
     double real_time)
{
   // Always update on the first pass, and thereafter whenever the
   // "update_interval" time has passed since the last update.
   if (force_state_update ||
       std::abs(real_time - last_update_time) > update_interval) {
      if (!update_third_body_state()) {
         return false;
      }
      last_update_time = real_time;
      force_state_update = false;
   }
   return true;
}


/**
 * Updates the state of the Third Body.  Because the ThirdBody may be
 * associated with multiple vehicles, and because its state
 * typically changes much more slowly than the vehicle anyway, it is
 * not always necessary to update the ThirdBodyState.
 */
bool
RadiationThirdBody::update_third_body_state ()
{
   local_frame_ptr->compute_position_from (
      *(primary_source_ptr->inertial_frame_ptr),
      source_to_third_inrtl);

   d_source_to_third = Vector3::vmag (source_to_third_inrtl);

   if (d_source_to_third <= 0.0) {
      MessageHandler::error(
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "The distance between the source-body and the third-body has an\n"
         "illegal value (%20.16f) for RadiationThirdBody (%s).\n"
         "This is a major configuration problem.\n"
         "Deactivating this body.\n", d_source_to_third, name.c_str());
      active = false;
      return false;
   }

   Vector3::scale (source_to_third_inrtl,
                   1 / d_source_to_third,
                   source_to_third_hat_inrtl);
   return true;
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
