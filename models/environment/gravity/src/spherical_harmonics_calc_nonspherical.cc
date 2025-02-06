/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/spherical_harmonics_calc_nonspherical.cc
 * Define SphericalHarmonicsGravityControl calc_nonspherical method, which
 * computes non-spherical gravitational acceleration of a gravitational body
 * on a given position.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((spherical_harmonics_calc_nonspherical.o)
   (spherical_harmonics_delta_controls.o)
   (spherical_harmonics_gravity_source.o)
   (environment/planet/planet.o))


*******************************************************************************/


// System includes
#include <cmath>

// JEOD includes
#include "environment/planet/include/planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/math/include/matrix3x3.hh"

// Model includes
#include "../include/gravity_messages.hh"
#include "../include/spherical_harmonics_gravity_controls.hh"
#include "../include/spherical_harmonics_delta_controls.hh"
#include "../include/spherical_harmonics_gravity_source.hh"

//! Namespace jeod
namespace jeod {

/**
 * Compute the gravitational acceleration at a given position toward a
 * gravitational body assuming the body has a non-spherical mass distribution.
 * \param[in] posn Point of interest, inrtl coords\n Units: M
 * \param[out] body_grav_accel Accel for given grav body\n Units: M/s2
 * \param[out] dgdx Gradient for given grav body\n Units: 1/s2
 * \param[out] Pot Potential
 */
void
SphericalHarmonicsGravityControls::calc_nonspherical (
   const double posn[3],
   double body_grav_accel[3],
   double dgdx[3][3],
   double  Pot[1])

{

   // Compute position vector magnitude (distance to the attractor)
   double r_mag = Vector3::vmag (posn);

   // Check if radial distance is less than equatorial radius
   if ((r_mag < harmonics_source->radius) & !min_radius_warn) {
      min_radius_warn = true;
      MessageHandler::warn (
         __FILE__, __LINE__, GravityMessages::domain_error,
         "Radial distance %g is less than the equatorial radius %g of %s.",
         r_mag, harmonics_source->radius, harmonics_source->name.c_str() );
   }


   // Store Cnm[2][...] array locally to avoid data writes to gravity body
   double local_Cnm[3];
   local_Cnm[0] = harmonics_source->Cnm[2][0];
   local_Cnm[1] = harmonics_source->Cnm[2][1];
   local_Cnm[2] = harmonics_source->Cnm[2][2];

   // Compute acceleration due to non-spherical gravity
   // (from code on pages 43-46 of Gottlieb's 1993 paper)

   // Compute gravity coefficients changes from variational effects
   double temp_C20;
   unsigned int n_deltacoeffs = var_effects.size();

   if (n_deltacoeffs > 0) {
      // Sum up the gravity coefficients changes in this body's "delta-bin"
      update_deltacoeffs ();
      sum_deltacoeffs ();

      temp_C20 = local_Cnm[0];
      local_Cnm[0] += total_dC20;

      // Correct permanent tide if already included in C20 coefficient
      if (!harmonics_source->tide_free) {
         local_Cnm[0] += harmonics_source->tide_free_delta;
      }
   }

   // Convert to planet-fixed.
   double posn_pf[3];
   Vector3::transform (harmonics_source->pfix->state.rot.T_parent_this, posn,
      posn_pf);

   // Define terms (page 33 of Gottlieb 1993)
   double r_mag_inv = 1.0 / r_mag;
   double X_div_r = posn_pf[0] * r_mag_inv;
   double Y_div_r = posn_pf[1] * r_mag_inv;
   double Z_div_r = posn_pf[2] * r_mag_inv;
   double Epilson = Z_div_r;

   double rad_div_r  = harmonics_source->radius * r_mag_inv;
   double rad_div_r_nth = rad_div_r;
   double mu_div_r  = harmonics_source->mu * r_mag_inv;
   double mu_div_rsq = mu_div_r * r_mag_inv;

   // Compute magnitude of projection on the Earth equatorial plane
   double rho_sq = 0.0;       // x^2+y^2
   if ((posn_pf[0] < -GSL_SQRT_DBL_MIN) || (posn_pf[0] > GSL_SQRT_DBL_MIN)) {
      rho_sq += posn_pf[0] * posn_pf[0];
   }
   if ((posn_pf[1] < -GSL_SQRT_DBL_MIN) || (posn_pf[1] > GSL_SQRT_DBL_MIN)) {
      rho_sq += posn_pf[1] * posn_pf[1];
   }

   // Modification to Gottlieb's paper: Equations 3-18 underflow near poles;
   // calculate cos^n(phi) separately from C_tilde and S_tilde.
   double rho = sqrt (rho_sq);
   double cos_phi = rho * r_mag_inv;
   double cos_phi_nth = cos_phi;

   // Set up first values to enable recursive calculation of normalized
   // coefficients with modification for underflow near poles
   double cos_mlambda[degree + 1];
   double sin_mlambda[degree + 1];
   cos_mlambda[0]    = 1.0;
   sin_mlambda[0]    = 0.0;
   if (rho_sq > 0.0) {
      cos_mlambda[1] = posn_pf[0] / rho;
      sin_mlambda[1] = posn_pf[1] / rho;
   }
   else {
      cos_mlambda[1] = 1.0;
      sin_mlambda[1] = 0.0;
   }

   // Initialize sums for perturbing gravity only.
   // (Full gravity would initialize the first three to 1.0 and Suml to 2.0.)
   double Sumv = 0.0;
   double Sumgam = 0.0;
   double Sumgam_grad = 0.0;
   double Suml = 0.0;

   double Sumh        = 0.0;
   double Sumh_grad   = 0.0;
   double Sumj        = 0.0;
   double Sumk        = 0.0;
   double Summ        = 0.0;
   double Sumn        = 0.0;
   double Sumo        = 0.0;
   double Sump        = 0.0;
   double Sumq        = 0.0;
   double Sumr        = 0.0;
   double Sums        = 0.0;
   double Sumt        = 0.0;

   double Sumh_N      = 0.0;
   double Sumj_N      = 0.0;
   double Sumk_N      = 0.0;
   double Suml_N      = 0.0;
   double Summ_N      = 0.0;
   double Sumn_N      = 0.0;
   double Sumo_N      = 0.0;
   double Sump_N      = 0.0;
   double Sumq_N      = 0.0;
   double Sumr_N      = 0.0;
   double Sums_N      = 0.0;
   double Sumt_N      = 0.0;
   double Sumv_N      = 0.0;

   double Sumh_grad_N = 0.0;
   double Sumgam_N    = 0.0;
   double Sumgam_grad_N = 0.0;

   double C_tilde[degree + 1];
   double S_tilde[degree + 1];

   double Lambda      = 0.0;

   double * P_ii;
   double * P_iim1;
   double * P_iim2;
   double * C_ii;
   double * S_ii;
   double * zeta_ii;
   double * xi_ii;
   double * eta_ii;
   double * upsilon_ii;

   double C_iijj          = 0.0;
   double S_iijj          = 0.0;

   double jj_x_Piijj      = 0.0;
   double B_tilde         = 0.0;
   double B_tilde_m1      = 0.0;
   double A_tilde_m1      = 0.0;
   double Piijj_x_Btilde  = 0.0;
   double zetaiijj_x_Piijjp1 = 0.0;

   double dbl_iip1        = 0.0;
   double dbl_jj          = 0.0;
   double dbl_jjp1        = 0.0;
   double dbl_jjm1        = 0.0;

   bool ii_grad_deg_nonzero;
   bool grad_order_nonzero;
   bool jj_lt_grad_order;


   // Initialize first 2 terms of normalized coefficients (bottom p 33)
   C_tilde[0] = 1.0;
   C_tilde[1] = X_div_r; // equation (3-18)
   S_tilde[0] = 0.0;
   S_tilde[1] = Y_div_r; // equation (3-18)

   Pnm[1][0] = sqrt (3.0) * Epilson;


   for (unsigned int ii = 2; ii <= degree; ++ii) {

      if (ii <= gradient_degree && gradient_degree > 0) {
         ii_grad_deg_nonzero = true;
      }
      else {
         ii_grad_deg_nonzero = false;
      }

      // Protect against writes into harmonics body Cnm array
      if (ii == 2) {
         C_ii = &(local_Cnm[0]);
      }
      else {
         C_ii = &(harmonics_source->Cnm[ii][0]);
      }

      P_ii = &(Pnm[ii][0]);
      S_ii = &(harmonics_source->Snm[ii][0]);
      xi_ii = &(harmonics_source->xi[ii][0]);
      eta_ii = &(harmonics_source->eta[ii][0]);
      zeta_ii = &(harmonics_source->zeta[ii][0]);
      upsilon_ii = &(harmonics_source->upsilon[ii][0]);

      P_iim1 = &(Pnm[ii-1][0]);
      P_iim2 = &(Pnm[ii-2][0]);

      rad_div_r_nth = rad_div_r_nth * rad_div_r;

      // Protect for underflow
      if (rad_div_r_nth < 1.0E-299) {
         rad_div_r_nth = 0.0;
      }

      // P(n,0) term, equation (7-14)
      P_ii[0] = harmonics_source->alpha[ii] * Epilson * P_iim1[0] - harmonics_source->beta[ii] * P_iim2[0];

      // P(n,n-1) term, equation (7-16)
      P_ii[ii-1] = Epilson * harmonics_source->nrdiag[ii];

      // P(n,1) term, equation (7-12)
      P_ii[1] = xi_ii[1] * Epilson * P_iim1[1] - eta_ii[1] * P_iim2[1];


      dbl_iip1 = harmonics_source->int_to_double[ii+1];

      Sumv_N   = P_ii[0] * C_ii[0];
      Sumh_N   = P_ii[1] * C_ii[0] * zeta_ii[0];
      Sumgam_N = Sumv_N * dbl_iip1;

      for (unsigned int jj = 2; jj <= (ii - 2); ++jj) {
         // Equation (7-12)
         P_ii[jj] = xi_ii[jj] * Epilson * P_iim1[jj] - eta_ii[jj] * P_iim2[jj];
      }

      if (ii_grad_deg_nonzero) {
         Sumh_grad_N   = P_ii[1] * C_ii[0] * zeta_ii[0];
         Sumgam_grad_N = Sumv_N * dbl_iip1;
         Summ_N        = P_ii[2] * C_ii[0] * upsilon_ii[0];
         Sump_N        = Sumh_grad_N * dbl_iip1;
         Suml_N        = Sumgam_grad_N * (dbl_iip1 + 1.0);
      }

      if (order > 0) {

         if (gradient_order > 0) {
            grad_order_nonzero = true;
         }
         else {
            grad_order_nonzero = false;
         }

         Sumj_N = 0.0;
         Sumk_N = 0.0;

         if (ii_grad_deg_nonzero && grad_order_nonzero) {
            Sumn_N = 0.0;
            Sumo_N = 0.0;
            Sumq_N = 0.0;
            Sumr_N = 0.0;
            Sums_N = 0.0;
            Sumt_N = 0.0;
         }

         if (cos_phi_nth > GSL_SQRT_DBL_MIN) {
            cos_phi_nth *= cos_phi;
         }
         else {
            cos_phi_nth = 0.0;
         }
         cos_mlambda[ii] = cos_mlambda[1] * cos_mlambda[ii-1] - sin_mlambda[1] * sin_mlambda[ii-1];
         sin_mlambda[ii] = sin_mlambda[1] * cos_mlambda[ii-1] + cos_mlambda[1] * sin_mlambda[ii-1];

         // Equation (3-18), modified for underflow
         C_tilde[ii] = cos_phi_nth * cos_mlambda[ii];
         S_tilde[ii] = cos_phi_nth * sin_mlambda[ii];


         for (unsigned int jj = 1; (jj <= order) && (jj <= ii); ++jj) {

            if (jj <= gradient_order) {
               jj_lt_grad_order = true;
            }
            else {
               jj_lt_grad_order = false;
            }

            dbl_jj     = harmonics_source->int_to_double[jj];
            dbl_jjp1   = harmonics_source->int_to_double[jj+1];
            dbl_jjm1 = harmonics_source->int_to_double[jj-1];

            C_iijj   = C_ii[jj];
            S_iijj   = S_ii[jj];

            jj_x_Piijj  = dbl_jj * P_ii[jj];
            B_tilde = C_iijj * C_tilde[jj] + S_iijj * S_tilde[jj];

            // equation (3-9)
            B_tilde_m1 = C_iijj * C_tilde[jj-1] + S_iijj * S_tilde[jj-1];
            A_tilde_m1 = C_iijj * S_tilde[jj-1] - S_iijj * C_tilde[jj-1];
            Piijj_x_Btilde = P_ii[jj] * B_tilde;
            Sumv_N = Sumv_N + Piijj_x_Btilde;

            if (jj < ii) {
               zetaiijj_x_Piijjp1 = zeta_ii[jj] * P_ii[jj+1];
               Sumh_N  = Sumh_N + zetaiijj_x_Piijjp1 * B_tilde;
               if (ii_grad_deg_nonzero && grad_order_nonzero && jj_lt_grad_order) {
                  Sumh_grad_N = Sumh_grad_N + zetaiijj_x_Piijjp1 * B_tilde;
                  Sump_N      = Sump_N + (dbl_jj + dbl_iip1) * zetaiijj_x_Piijjp1 * B_tilde;
                  Sumq_N      = Sumq_N + dbl_jj * zetaiijj_x_Piijjp1 * B_tilde_m1;
                  Sumr_N      = Sumr_N - dbl_jj * zetaiijj_x_Piijjp1 * A_tilde_m1;
               }
            }

            Sumj_N   = Sumj_N + jj_x_Piijj * B_tilde_m1;
            Sumk_N   = Sumk_N - jj_x_Piijj * A_tilde_m1;
            Sumgam_N = Sumgam_N + (dbl_jj + dbl_iip1) * Piijj_x_Btilde;

            if (ii_grad_deg_nonzero && grad_order_nonzero && jj_lt_grad_order) {
               Sumgam_grad_N = Sumgam_grad_N + (dbl_jj + dbl_iip1) * Piijj_x_Btilde;
               Suml_N        = Suml_N + (dbl_jj + dbl_iip1) * (dbl_jjp1 + dbl_iip1) * Piijj_x_Btilde;
               Summ_N        = Summ_N + P_ii[jj+2] * B_tilde * upsilon_ii[jj];
               Sums_N        = Sums_N + (dbl_jj + dbl_iip1) * jj_x_Piijj * B_tilde_m1;
               Sumt_N        = Sumt_N - (dbl_jj + dbl_iip1) * jj_x_Piijj * A_tilde_m1;
            }

            if (jj >= 2) {
               if (ii_grad_deg_nonzero && grad_order_nonzero && jj_lt_grad_order) {
                  Sumn_N = Sumn_N + dbl_jjm1 * jj_x_Piijj *
                           (C_iijj * C_tilde[jj-2] + S_iijj * S_tilde[jj-2]);
                  Sumo_N = Sumo_N + dbl_jjm1 * jj_x_Piijj *
                           (C_iijj * S_tilde[jj-2] - S_iijj * C_tilde[jj-2]);
               }
            }

         } // next m

         Sumj += rad_div_r_nth * Sumj_N;
         Sumk += rad_div_r_nth * Sumk_N;

         if (ii_grad_deg_nonzero && grad_order_nonzero) {
            Sumn += rad_div_r_nth * Sumn_N;
            Sumo += rad_div_r_nth * Sumo_N;
            Sumq += rad_div_r_nth * Sumq_N;
            Sumr += rad_div_r_nth * Sumr_N;
            Sums += rad_div_r_nth * Sums_N;
            Sumt += rad_div_r_nth * Sumt_N;
         }

      }  // end if order>0

      Sumv   += rad_div_r_nth * Sumv_N;
      Sumh   += rad_div_r_nth * Sumh_N;
      Sumgam += rad_div_r_nth * Sumgam_N;

      if (ii_grad_deg_nonzero) {
         Sumh_grad   += rad_div_r_nth * Sumh_grad_N;
         Sumgam_grad += rad_div_r_nth * Sumgam_grad_N;
         Suml        += rad_div_r_nth * Suml_N;
         Summ        += rad_div_r_nth * Summ_N;
         Sump        += rad_div_r_nth * Sump_N;
      }

   } // next n

   *Pot   = mu_div_r * Sumv; // gravitational potential
   Lambda = Sumgam + Epilson * Sumh;

   // Equation (4-13)
   body_grav_accel[0] = -mu_div_rsq * (Lambda * X_div_r - Sumj);
   body_grav_accel[1] = -mu_div_rsq * (Lambda * Y_div_r - Sumk);
   body_grav_accel[2] = -mu_div_rsq * (Lambda * Z_div_r - Sumh);

   // Convert back to inertial (overwrites position vector)
   Vector3::transform_transpose (harmonics_source->pfix->state.rot.T_parent_this,
                                 body_grav_accel);

   // Remove delta coefficient for tide effect
   if (n_deltacoeffs > 0) {
      local_Cnm[0] = temp_C20;
   }


   // Compute gravity gradient
   if (gradient && (gradient_degree > 0)) {

      double dgdx_pf[3][3]; // Gravity gradient in planet-fixed coords

      Lambda = Sumgam_grad + Epilson * Sumh_grad;
      double Gg = -(Summ * Epilson + Sump + Sumh_grad);
      double Ff =  Suml + Lambda + Epilson * (Sump + Sumh_grad - Gg);
      double D1 =  Epilson * Sumq + Sums;
      double D2 =  Epilson * Sumr + Sumt;

      double mu_div_r3 = mu_div_rsq * r_mag_inv;
      dgdx_pf[0][0] = mu_div_r3 * ((Ff * X_div_r - 2.0 * D1) * X_div_r - Lambda + Sumn);
      dgdx_pf[1][1] = mu_div_r3 * ((Ff * Y_div_r - 2.0 * D2) * Y_div_r - Lambda - Sumn);
      dgdx_pf[2][2] = mu_div_r3 * ((Ff * Z_div_r + 2.0 * Gg) * Z_div_r - Lambda + Summ);
      dgdx_pf[0][1] = mu_div_r3 * ((Ff * Y_div_r - D2) * X_div_r - D1 * Y_div_r - Sumo);
      dgdx_pf[1][0] = dgdx_pf[0][1];
      dgdx_pf[0][2] = mu_div_r3 * ((Ff * X_div_r - D1) * Z_div_r + Gg * X_div_r + Sumq);
      dgdx_pf[2][0] = dgdx_pf[0][2];
      dgdx_pf[1][2] = mu_div_r3 * ((Ff * Y_div_r - D2) * Z_div_r + Gg * Y_div_r + Sumr);
      dgdx_pf[2][1] = dgdx_pf[1][2];

      // Transform dgdx_pf gradient matrix to inertial coordinates
      // (use similarity transformation)
      Matrix3x3::transpose_transform_matrix (
         harmonics_source->pfix->state.rot.T_parent_this,
         dgdx_pf, dgdx);
   }
   else {
      Matrix3x3:: initialize (dgdx);
   }

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
