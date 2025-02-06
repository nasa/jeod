/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/gravity_controls.cc
 * Define member functions for the GravityControls class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((gravity_controls.cc)
   (gravity_source.cc)
   (gravity_manager.cc)
   (gravity_messages.cc)
   (utils/message/src/message_handler.cc)
   (utils/ref_frames/src/ref_frame.cc))


*******************************************************************************/

// System includes
#include <algorithm>
#include <cmath>
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame.hh"

// Model includes
#include "../include/gravity_controls.hh"
#include "../include/gravity_source.hh"
#include "../include/gravity_integ_frame.hh"
#include "../include/gravity_interaction.hh"
#include "../include/gravity_manager.hh"
#include "../include/gravity_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * The speed of light squared, in m^2/s^2.
 */
static constexpr double speed_of_light_sq = 89875517873681764.0;



/**
 * GravityControls constructor.
 */
GravityControls::GravityControls ()
:
   active(false),
   spherical(false),
   gradient(false),
   perturbing_only(false),
   battin_method(false),
   relativistic(false),
   body(nullptr),
   grav_manager(nullptr),
   subscribed_to_inertial(false),
   subscribed_to_pfix(false),
   skip_spherical(false)
{
    Vector3::initialize(grav_accel);
    Matrix3x3::initialize(grav_grad);

    grav_pot = 0.0;

    grav_accel_magsq = 0.0;
}

/**
 * GravityControls destructor.
 */
GravityControls::~GravityControls ()
{
   // Empty.
   // Note: The planet_name is set externally.
   // This is a simple POD class; deleting the memory is the responsibility
   // of the external agent that set the planet_name.
}


/**
 * Initialize this GravityControl.
 * \param[in] grav_man Ref to Gravity Manager
 */
void
GravityControls::initialize_control (
   GravityManager & grav_man)
{

   // Check if initializing directly or via name
   if( body != nullptr ) // directly
   {
       source_name = body->name;
   }
   else if( !source_name.empty() ) // by name
   {
       body = grav_man.find_grav_source (source_name);
       if (body == nullptr) {
           MessageHandler::error (
                   __FILE__, __LINE__, GravityMessages::missing_entry,
                   "Unable to find planet named '%s'",
                   source_name.c_str() );
           return;
       }
   }
   else
   {
       MessageHandler::error (
               __FILE__, __LINE__, GravityMessages::invalid_object,
               "GravityControl has a null source AND empty planet name");
       return;
   }

   // The grav_source should have an associated inertial frame.
   if (body->inertial == nullptr) {
      MessageHandler::error (
         __FILE__, __LINE__, GravityMessages::missing_entry,
         "Gravity source named '%s' has not been fully initialized.",
         source_name.c_str() );
      return;
   }

   // If everything is ok, set the base gravity body and gravity manager pointers.
   grav_manager = &grav_man;
}


/**
 * Reset subscriptions for this GravityControl.
 * \param[in] dyn_manager Ptr to dynamics manager
 */
void
GravityControls::reset_control (
   BaseDynManager& dyn_manager)
{
   if( body == nullptr || body->inertial == nullptr )
   {
       MessageHandler::inform (
               __FILE__, __LINE__, GravityMessages::null_pointer,
               "'%s' has no inertial frame yet, cannot reset subscription.",
               source_name.c_str() );
       subscribed_to_inertial = false;
       return;
   }
   bool subscribe_to_pfix; // Planet-fixed subscription needed


   // Make the inertial subscription consistent with expectations;
   // inertial subscription is needed if the control is on.
   if (active) {
      if (!subscribed_to_inertial) {
         dyn_manager.subscribe_to_frame (*(body->inertial));
         subscribed_to_inertial = true;
      }
      // No else: Subscription status is already consistent.
   }
   else {
      if (subscribed_to_inertial) {
         dyn_manager.unsubscribe_to_frame (*(body->inertial));
         subscribed_to_inertial = false;
      }
      // No else: Subscription status is already consistent.
   }

   // Determine what the planet-fixed subscription should be;
   // need planet-fixed if the control is on and is non-spherical.
   subscribe_to_pfix = active && !spherical;

   // Make the planet-fixed subscription consistent with expectations.
   if (subscribe_to_pfix) {
      if (!subscribed_to_pfix) {
         dyn_manager.subscribe_to_frame (*(body->pfix));
         subscribed_to_pfix = true;
      }
      // No else: Subscription status is already consistent.
   }
   else {
      if (subscribed_to_pfix) {
         dyn_manager.unsubscribe_to_frame (*(body->pfix));
         subscribed_to_pfix = false;
      }
      // No else: Subscription status is already consistent.
   }

   return;
}


/**
 * Compute the gravitation at a given position toward a gravity body.
 * \param[in] integ_pos Point of interest, integ coords\n Units: M
 * \param[in] integ_frame_idx Integ frame index
 * \param[out] body_grav_accel Accel for given grav body\n Units: M/s2
 * \param[out] dgdx Gradient for given grav body\n Units: 1/s2
 * \param[out] Pot Potential
 */
void
GravityControls::gravitation (
   const double integ_pos[3],
   unsigned int integ_frame_idx,
   double body_grav_accel[3],
   double dgdx[3][3],
   double Pot[1])
{
   double posn[3];              // M    Vehicle inertial position wrt planet
   GravityIntegFrame & grav_source_frame = // --  Grav frame for this integ frame
                                         body->frames[integ_frame_idx];
   const RefFrame & integ_frame = // --   Integration frame
                                  *(grav_source_frame.ref_frame);

   // Compute position of integ. frame origin wrt the planet center.
   integ_frame.compute_position_from (
      *(body->inertial), grav_source_frame.pos);

   // Compute position of the vehicle CoM wrt the planet center.
   Vector3::sum (grav_source_frame.pos, integ_pos, posn);

   // Compute contributions from non-spherical gravity if requested.
   if (! spherical) {
      calc_nonspherical (integ_pos, posn, grav_source_frame,
                         body_grav_accel, dgdx, Pot[0]);
   }

    // Spherical only: Initialize gradient, accel, and potential to zero.
   else {
      Matrix3x3:: initialize (dgdx);
      Vector3::initialize (body_grav_accel);
      Pot[0] = 0.0;
   }

   // Compute spherical gravity contributions if needed.
   if (! perturbing_only && ! skip_spherical) {
      calc_spherical (
         integ_pos, posn, grav_source_frame,
         body_grav_accel, dgdx, Pot[0]);
   }
}


/**
 * Compute the gravitation at a given position toward a gravity body.
 * \param[in] point_of_interest  Point of interest, as a reference frame.
 * \param[in] integ_frame_idx  Integ frame index.
 * \param[out] body_grav_accel  Accel for given grav body.
 * \param[out] dgdx  Gradient for given grav body.
 * \param[out] pot  Specific gravitational potential for given grev body.
 */
void
GravityControls::gravitation (
   const RefFrame& point_of_interest,
   unsigned int integ_frame_idx,
   double body_grav_accel[3],
   double dgdx[3][3],
   double& pot)
{
   double rel_pos[3];              // M    Vehicle inertial position wrt planet
   GravityIntegFrame & grav_source_frame = // --  Grav frame for this integ frame
                                         body->frames[integ_frame_idx];
   const RefFrame & integ_frame = // --   Integration frame
                                  *(grav_source_frame.ref_frame);
   RefFrameState grav_source_state;

   // Compute state of the planet center wrt integ. frame origin.
   body->inertial->compute_relative_state (integ_frame, grav_source_state);

   // Compute position of the vehicle CoM wrt the planet center.
   Vector3::diff (
      point_of_interest.state.trans.position, grav_source_state.trans.position,
      rel_pos);

   // Compute contributions from non-spherical gravity if requested.
   if (! spherical) {
       calc_nonspherical (
          point_of_interest.state.trans.position, rel_pos, grav_source_frame,
          body_grav_accel, dgdx, pot);
   }

    // Spherical only: Initialize gradient, accel, and potential to zero.
   else {
      Matrix3x3:: initialize (dgdx);
      Vector3::initialize (body_grav_accel);
      pot = 0.0;
   }

   // Compute relativistic gravitational acceleration correction if requested.
   // The relativistic contributions to gravity gradient and potential are
   // not computed.
   if (relativistic)
   {
      double rel_vel[3];
      double relativistic_accel[3];
      Vector3::diff (
         point_of_interest.state.trans.velocity, grav_source_state.trans.velocity,
         rel_vel);
      calc_relativistic (
         point_of_interest, rel_pos, rel_vel,
         relativistic_accel);
      Vector3::incr (relativistic_accel, body_grav_accel);
   }

   // Compute spherical gravity contributions if needed.
   if (! perturbing_only && ! skip_spherical) {

      // Compute state of integ. frame origin wrt the planet center.
      Vector3::negate (grav_source_state.trans.position, grav_source_frame.pos);

      // Calculate spherical gravity, in the integration frame.
      calc_spherical (
         point_of_interest.state.trans.position, rel_pos, grav_source_frame,
         body_grav_accel, dgdx, pot);
   }
}


void
GravityControls::calc_spherical (
   const double integ_pos[3],
   const double posn[3],
   const GravityIntegFrame & grav_source_frame,
   double body_grav_accel[3],
   double dgdx[3][3],
   double&  pot)
{
   double mu_local = body->mu;  // M3/s2  Locally cache mu for current grav body

   // Compute terms leading to spherical gravity acceleration.
   double acc_local[3];
   double r_sq = Vector3::vmagsq (posn);
   double r_mag = std::sqrt(r_sq);
   double r_3rd = r_sq * r_mag;

   // Compute 3rd body acceleration offset.
   if (grav_source_frame.is_third_body) {
      double rho_sq = Vector3::vmagsq (grav_source_frame.pos);
      double rho_3rd = rho_sq * std::sqrt(rho_sq);
      double r_dot_rho = Vector3::dot (integ_pos, grav_source_frame.pos);

      // Issue: Does the 3rd body effect also apply to potential?
      // pot += mu_local * r_dot_rho / rho_3rd;

      // Battin's method: See writeup.
      if (battin_method) {
         double q = (Vector3::vmagsq(integ_pos) + 2.0 * r_dot_rho) / rho_sq;

         // Use Battin's method only where it yields better results.
         // Note: updated for v4.0 based on additional experimental analysis.
         if (q > -0.38) {
            q = q * (3.0 + q * (3.0 + q));
            q = q / (1.0 + std::sqrt(1.0 + q));
            Vector3::scale (
               -mu_local / (rho_3rd * (1.0 + q)),
               Vector3::scale_decr (
                  grav_source_frame.pos, q,
                  Vector3::copy (integ_pos, acc_local)));
         }

         // Use the original method where Battin's method yields poor results.
         else {
            Vector3::scale (posn, -mu_local/r_3rd, acc_local);
            Vector3::scale_incr (
               grav_source_frame.pos, mu_local/rho_3rd, acc_local);
         }
      }

      // Original method: a = -(mu/d^3) * \vec d + (mu/\rho^3) * \vec{\rho}.
      else {
         Vector3::scale (posn, -mu_local/r_3rd, acc_local);
         Vector3::scale_incr (
            grav_source_frame.pos, mu_local/rho_3rd, acc_local);
      }
   }

   // Non-third body: a = -mu/r^3 * \vec r.
   else {
      Vector3::scale (posn, -mu_local / r_3rd, acc_local);
   }

   Vector3::incr (acc_local, body_grav_accel);

   // Compute inertial frame specific potential energy.
   pot += mu_local / r_mag;

   // Compute spherical gravity gradient (Gottlieb p.23)
   if (gradient) {
      double r_5th = r_3rd * r_sq;   // M^5
      double mu_div_r5th = mu_local / r_5th;
      dgdx[0][0] += mu_div_r5th * (3.0 * posn[0] * posn[0] - r_sq);
      dgdx[0][1] += mu_div_r5th *  3.0 * posn[0] * posn[1];
      dgdx[0][2] += mu_div_r5th *  3.0 * posn[0] * posn[2];
      dgdx[1][1] += mu_div_r5th * (3.0 * posn[1] * posn[1] - r_sq);
      dgdx[1][2] += mu_div_r5th *  3.0 * posn[1] * posn[2];
      dgdx[2][2] += mu_div_r5th * (3.0 * posn[2] * posn[2] - r_sq);
      dgdx[1][0] = dgdx[0][1];
      dgdx[2][0] = dgdx[0][2];
      dgdx[2][1] = dgdx[1][2];
   }
}


/**
 *
 * \details
 * Implements equation 27 (Folkner) / equation 4 (Genova) to compute a
 * parameteric post-Newtonian correction to gravitation. The referenced equation
 * is of the form
 * @f{align*}{
 * \begin{split}
 *   \boldsymbol a_{\text{A,pm-pm}} =&
 *   \phantom{+} \sum_{B\ne A}
 *     \frac{GM_B}{r^3_{AB}}(\boldsymbol r_B - \boldsymbol r_A)
 *     \bigl(1+\frac {s_1} {c^2}\bigr) \\
 *   & + \frac 1 {c^2} \sum_{B\ne A}
 *       \frac{GM_B}{r^3_{AB}}(\boldsymbol v_A - \boldsymbol v_B)
 *       \Bigl(
 *         \bigl( \boldsymbol r_A - \boldsymbol r_B \bigr) \cdot
 *         \bigl( (2+2\gamma)\boldsymbol v_A - (1+2\gamma)\boldsymbol v_B \bigr)
 *       \Bigr) \\
 *   & + \frac {3+4\gamma} {2c^2}
 *       \sum_{B\ne A} \frac{GM_B}{r_{AB}} \boldsymbol a_B
 * \end{split}
 * @f}
 * where @f$ \boldsymbol a_B @f$ is the net acceleration of gravitating body *B*
 * toward the other gravitating bodies (typically taken to be the Newtonian
 * gravitational acceleration) and
 * @f{equation*}{
 *  \begin{split}
 *  s_1 =
 *    & - 2(\beta+\gamma) \sum_{C\ne A}\frac{GM_C}{r_{AC}}
 *      - (2\beta-1) \sum_{C\ne A}\frac{GM_C}{r_{BC}} \\
 *    & + \gamma v_a^2
 *      + (1+\gamma) v_b^2
 *      - 2(1+\gamma)\boldsymbol v_A \cdot \boldsymbol v_B \\
 *    & - \frac32
 *        \left(
 *          \frac{(\boldsymbol r_A - \boldsymbol r_B) \cdot \boldsymbol v_B}{r_B}
 *         \right)^2 \\
 *    & + \frac12(\boldsymbol r_B - \boldsymbol r_A)\cdot \boldsymbol a_B
 *  \end{split}
 * @f}
 * In terms of the referenced equation, the intent of this function is to compute
 * the relativistic portion of the acceleration of body *A* toward body *B*.
 * With this, the expression this function computes is
 * @f{equation*}{
 * \begin{split}
 *   \boldsymbol \Delta a_{\text{A,B}} =
 *   \frac 1 c^2 \frac {GM_B}{r_{AB}} \Biggl\{ &
 *     \phantom{+} \frac{\boldsymbol r_B - \boldsymbol r_A}{r^2_{AB}} s_1 \\
 *   & + \frac{\boldsymbol v_A - \boldsymbol v_B}{r^2_{AB}}
 *       \Bigl(
 *         \bigl( \boldsymbol r_A - \boldsymbol r_B \bigr) \cdot
 *         \bigl( (2+2\gamma)\boldsymbol v_A - (1+2\gamma)\boldsymbol v_B \bigr)
 *       \Bigr) \\
 *   & + \frac {3+4\gamma}{2} \boldsymbol a_B
 *   \Biggr\}
 * \end{split}
 * @f}
 * Note that the common factor @f$ \frac 1 c^2 \frac {GM_B}{r_{AB}} @f$
 * is unitless and thus each of the three terms in the braced expression
 * has units of acceleration.
 *
 * The referenced equations explicitly involve the parameterized post-Newtonian
 * (PPN) factors @f$ \beta @f$ and @f$ \gamma @f$. This function hardcodes both
 * of those factors as one, consistent with general relativity.
 * For example, the factor @f$ (3+4\gamma)/2 @f$ becomes 3.5 in the code.
 * Magic numbers that involve @f$ \beta @f$ or @f$ \gamma @f$, including a
 * factor of one (e.g., @f$ 2 \beta - 1 @f$), are noted in comments.
 *
 * @par References:
 *  - William M. Folkner, et al., *Planetary and Lunar Ephemerides DE430 and
 *    DE431*, IPN Progress Report 42-196, 15 February 2014.\n
 *    ftp://naif.jpl.nasa.gov/pub/naif/generic_kernels/spk/planets/de430_and_de431.pdf
 *  - Antonio Genova, et al., *Solar system expansion and strong equivalence
 *    principle as seen by the NASA MESSENGER mission*,
 *    Nature Communications 9:289, 18 January 2018,
 *    DOI: 10.1038/s41467-017-02558-1.\n
 *    https://www.nature.com/articles/s41467-017-02558-1
 */
void
GravityControls::calc_relativistic (
   const RefFrame& point_of_interest,
   const double rel_pos[3],
   const double rel_vel[3],
   double perturbing_accel[3])
{
   typedef std::vector<GravitySource*>::const_iterator BodyIterator;
   double rsq_AB = Vector3::vmagsq(rel_pos);
   double r_AB_mag = std::sqrt(rsq_AB);

   double body_accel[3] = {0.0, 0.0, 0.0};
   double body_potential = 0.0;
   double point_potential = 0.0;

   // Compute
   // - The gravitational acceleration and the specific gravitational potential
   //   energy at the location of the gravitational body associated with this
   //   control with respect to the the other bodies in the simulation, and
   // - The specific gravitational potential energy at the point of interest
   //   with respect to the the other bodies in the simulation.
   const RefFrame& body_inertial = *(body->inertial);
   const std::vector<GravitySource*>& all_bodies = grav_manager->get_bodies();
   for (BodyIterator iter = all_bodies.begin(), end = all_bodies.end();
        iter != end;
        ++iter)
   {
      const GravitySource* other_body = *iter;
      const RefFrame& other_inertial = *(other_body->inertial);

      // Add the other body's contribution to the gravitational acceleration
      // and potential at this body's location.
      if (other_body != body) {
         double r_CB[3];
         other_inertial.compute_position_from (body_inertial, r_CB);
         double r_CB_sq = Vector3::vmagsq(r_CB);
         double other_potential = other_body->mu / std::sqrt(r_CB_sq);
         Vector3::scale_incr (r_CB, other_potential/r_CB_sq, body_accel);
         body_potential += other_potential;
      }

      // Add the other body's contribution to the gravitational potential
      // at the point of interest. Note: The point of interest might be
      // that of a propagated planet, which needs to be excluded from the sum.
      double r_CA[3];
      other_inertial.compute_position_from (point_of_interest, r_CA);
      double rsq_CA = Vector3::vmagsq(r_CA);
      if (rsq_CA > 1.0) {
         point_potential += other_body->mu / std::sqrt(rsq_CA);
      }
   }

   // The relativistic point mass interaction (equation 27, Folkner) involves
   // three sums over the gravitating bodies. As noted above, this function
   // calculates the contributions from gravitating body B, sans the Newtonian
   // acceleration.
   // yielding accelerations. The first is parallel to relative position,
   // the second is parallel to relative velocity, and the third is parallel
   // to the Newtonian acceleration of the gravitating body toward the other
   // other gravitating bodies in the simulation.
   double accel1[3];
   double accel2[3];
   double accel3[3];

   // Compute the contribution parallel to relative position.
   double accel1_scale;

   // The first and second terms in the sum leading to accel1 are
   // @f$ -2(\beta+\gamma) \sum_{C\ne A} \frac{\mu_C}{r_{AC}} @f$ and
   // @f$ -(2\beta-1) \sum_{C\ne B} \frac{\mu_C}{r_{BC}} @f$.
   // The scale factors @f$ 2(\beta+\gamma) @f$ and @f$ (2\beta-1) @f$
   // are 4.0 and 1.0, respectively, when @f$ \beta=\gamma=1 @f$.
   // Sans the scale factors, these have already been calculated
   // as point_potential and body_potential, respectively.
   accel1_scale = -(4.0*point_potential + body_potential);

   // The next set of terms in the sum leading to accel1 are
   // @f$ \gamma*v_A^2 + (1+\gamma)*v_B^2 - 2*(1+\gamma)*v_A \cdot v_B @f$.
   // These are calculated as @f$ (1+\gamma)||v_A-v_B||^2 - v_A^2 @f$.
   accel1_scale +=
      2.0*Vector3::vmagsq(rel_vel)  // 1+gamma = 2
      - Vector3::vmagsq(point_of_interest.state.trans.velocity);

   // The next term in the sum leading to accel1 is
   // @f$ -3/2 (((r_A-r_B)\cdot v_B)/(||r_A-r_B||))^2
   // We don't have v_B, but it is calculable from v_A and v_A-v_B.
   double v_B[3];
   Vector3::diff (point_of_interest.state.trans.velocity, rel_vel, v_B);
   double rhat_dot_vb = Vector3::dot(rel_pos, v_B) / r_AB_mag;
   accel1_scale -= 1.5*rhat_dot_vb*rhat_dot_vb;

   // The last term in the sum leading to accel1 is
   // @f$ 1/2 (((r_B-r_A)\cdot a_B) @f$.
   // We have @f$ r_A-r_B @F$ in rel_pos, hence the change in sign.
   accel1_scale -= 0.5*Vector3::dot(rel_pos, body_accel);

   // Finally, calculate the contribution parallel to relative position.
   // The reference equation uses
   // @f$ \frac{\boldsymbol r_B - \boldsymbol r_A}{r^2_{AB}} s_1 @f$ .
   // The @f$ s_1 @f$ in the above is accel1_scale. We once again
   // have @f$ r_A-r_B @F$ in rel_pos, hence the change in sign.
   Vector3::scale (rel_pos, -accel1_scale/rsq_AB, accel1);

   // Compute the contribution parallel to relative velocity.
   double term2_vel[3];
   Vector3::scale (rel_vel, 3.0, term2_vel);
   Vector3::incr (point_of_interest.state.trans.velocity, term2_vel);
   double term2_scale = Vector3::dot(rel_pos, term2_vel) / rsq_AB;
   Vector3::scale (rel_vel, term2_scale, accel2);

   // Compute the contribution parallel to the acceleration of the grav body.
   Vector3::scale (body_accel, 3.5, accel3);

   Vector3::sum (accel1, accel2, accel3, perturbing_accel);
   Vector3::scale (body->mu / r_AB_mag / speed_of_light_sq, perturbing_accel);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
