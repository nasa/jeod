/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/spherical_harmonics_gravity_source.cc
 * Define member functions for the SphericalHarmonicsGravitySource class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((spherical_harmonics_gravity_source.o)
   (gravity_source.o)
   (spherical_harmonics_delta_coeffs.o)
   (spherical_harmonics_delta_coeffs_init.o)
   (gravity_manager.o)
   (gravity_messages.o)
   (environment/ephemerides/ephem_interface/ephem_ref_frame.o)
   (utils/message/message_handler.o))


*******************************************************************************/


// System includes
#include <cmath>
#include <cstddef>
#include <cstring>
#include <typeinfo>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "utils/math/include/numerical.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/spherical_harmonics_delta_coeffs.hh"
#include "../include/spherical_harmonics_delta_coeffs_init.hh"
#include "../include/gravity_manager.hh"
#include "../include/gravity_messages.hh"
#include "../include/spherical_harmonics_gravity_source.hh"


//! Namespace jeod
namespace jeod {

/**
 * SphericalHarmonicsGravitySource constructor.
 */
SphericalHarmonicsGravitySource::SphericalHarmonicsGravitySource (
   void)
:
   radius(0.0),
   degree(0),
   order(0),
   Cnm(NULL),
   Snm(NULL),
   tide_free(false),
   tide_free_delta(0.0),

   a_by_rad(NULL),
   alpha(NULL),
   beta(NULL),
   xi(NULL),
   eta(NULL),
   zeta(NULL),
   upsilon(NULL),
   nrdiag(NULL),
   int_to_double(NULL)
{
   JEOD_REGISTER_CLASS (SphericalHarmonicsGravitySource);
   JEOD_REGISTER_CLASS (SphericalHarmonicsDeltaCoeffs);
   JEOD_REGISTER_CHECKPOINTABLE (this, delta_coeffs);
   return;
}


/**
 * SphericalHarmonicsGravitySource destructor.
 */
SphericalHarmonicsGravitySource::~SphericalHarmonicsGravitySource (
   void)
{

   JEOD_DEREGISTER_CHECKPOINTABLE (this, delta_coeffs);
   if (xi != NULL) {
      for (unsigned int ii = 0; ii <= degree; ++ii) {
         JEOD_DELETE_ARRAY (xi[ii]);
         JEOD_DELETE_ARRAY (eta[ii]);
         JEOD_DELETE_ARRAY (zeta[ii]);
         JEOD_DELETE_ARRAY (upsilon[ii]);
      }
      JEOD_DELETE_ARRAY (a_by_rad);
      JEOD_DELETE_ARRAY (alpha);
      JEOD_DELETE_ARRAY (beta);
      JEOD_DELETE_ARRAY (xi);
      JEOD_DELETE_ARRAY (eta);
      JEOD_DELETE_ARRAY (zeta);
      JEOD_DELETE_ARRAY (upsilon);
      JEOD_DELETE_ARRAY (nrdiag);
      JEOD_DELETE_ARRAY (int_to_double);
   }

   if (JEOD_IS_ALLOCATED (Snm)) {
      for (unsigned int ii = 0; ii <= degree; ++ii) {
         JEOD_DELETE_ARRAY (Snm[ii]);
         JEOD_DELETE_ARRAY (Cnm[ii]);
      }
      JEOD_DELETE_ARRAY (Snm);
      JEOD_DELETE_ARRAY (Cnm);
   }

   return;
}


/**
 * Initialize Gottlieb gravity coefficients.
 */
void
SphericalHarmonicsGravitySource::initialize_body (
   void)
{

   // If degree > 0 then create and fill Gottlieb coefficient arrays.
   // Otherwise, only spherical gravity can be used.
   if (degree > 0) {
      double num1;
      double den1;
      double num2;
      double den2;
      double ** Pnm;

      Pnm  = JEOD_ALLOC_PRIM_ARRAY (degree + 1, double *);

      a_by_rad      = JEOD_ALLOC_PRIM_ARRAY (degree + 1, double);
      alpha         = JEOD_ALLOC_PRIM_ARRAY (degree + 1, double);
      beta          = JEOD_ALLOC_PRIM_ARRAY (degree + 1, double);
      xi            = JEOD_ALLOC_PRIM_ARRAY (degree + 1, double *);
      eta           = JEOD_ALLOC_PRIM_ARRAY (degree + 1, double *);
      zeta          = JEOD_ALLOC_PRIM_ARRAY (degree + 1, double *);
      upsilon       = JEOD_ALLOC_PRIM_ARRAY (degree + 1, double *);
      nrdiag        = JEOD_ALLOC_PRIM_ARRAY (degree + 1, double);
      int_to_double = JEOD_ALLOC_PRIM_ARRAY (degree + 2, double);

      for (unsigned int ii = 0; ii <= degree; ++ii) {
         Pnm[ii]           = JEOD_ALLOC_PRIM_ARRAY (ii + 3, double);
         xi[ii]            = JEOD_ALLOC_PRIM_ARRAY (ii + 1, double);
         eta[ii]           = JEOD_ALLOC_PRIM_ARRAY (ii + 1, double);
         zeta[ii]          = JEOD_ALLOC_PRIM_ARRAY (ii + 1, double);
         upsilon[ii]       = JEOD_ALLOC_PRIM_ARRAY (ii + 1, double);
         int_to_double[ii] = static_cast<double> (ii);
      }

      int_to_double[degree + 1] = static_cast<double> (degree + 1);


      // In the code below, the equation numbers and page numbers refer to
      // the Gottlieb 1993 paper.

      // Bottom of page 47 and page 48, and see equation (7-8)
      Pnm[0][0] = 1.0;
      Pnm[0][1] = 0.0;
      Pnm[0][2] = 0.0;
      Pnm[1][1] = sqrt (3.0);
      Pnm[1][2] = 0.0;
      Pnm[1][3] = 0.0;

      // Pages 46-47
      for (unsigned int ii = 2; ii <= degree; ++ii) {
         for (unsigned int jj = 0; jj <= (ii - 1); ++jj) {
            // Equation (7-10)
            num1 = (2.0 * int_to_double[ii] - 1.0)
                      * (2.0 * int_to_double[ii] + 1.0);
            den1 = (int_to_double[ii] + int_to_double[jj])
                      * (int_to_double[ii] - int_to_double[jj]);
            xi[ii][jj] = sqrt (num1 / den1);

            // Equation (7-10)
            num2 = (2.0 * int_to_double[ii] + 1.0)
                      * (int_to_double[ii] + int_to_double[jj] - 1.0)
                      * (int_to_double[ii] - int_to_double[jj] - 1.0);
            den2 = (int_to_double[ii] + int_to_double[jj])
                      * (int_to_double[ii] - int_to_double[jj])
                      * (2.0 * int_to_double[ii] - 3.0);
            if (std::fpclassify(num2) == FP_ZERO) {
               eta[ii][jj] = 0.0;
            }
            else {
               eta[ii][jj] = sqrt (num2 / den2);
            }
         }

         for (unsigned int jj = 0; jj <= ii; ++jj) {
            if (ii == jj) {
               zeta[ii][jj] = 0.0;
               upsilon[ii][jj] = 0.0;
            }
            else if (jj == 0) {
               // Equation (7-19)
               zeta[ii][0] = sqrt (int_to_double[ii]
                      * (int_to_double[ii] + 1.0) / 2.0);
               // Equation (7-22)
               upsilon[ii][0] = sqrt (int_to_double[ii]
                      * (int_to_double[ii] - 1.0) * (int_to_double[ii] + 1.0)
                      * (int_to_double[ii] + 2.0) / 2.0);
            }
            else {
               // Equation (7-19)
               zeta[ii][jj] = sqrt ((int_to_double[ii] - int_to_double[jj])
                      * (int_to_double[ii] + int_to_double[jj] + 1.0));
               // Equation (7-22)
               upsilon[ii][jj] = sqrt ((int_to_double[ii] - int_to_double[jj])
                      * (int_to_double[ii] + int_to_double[jj] + 1.0)
                      * (int_to_double[ii] - int_to_double[jj] - 1.0)
                      * (int_to_double[ii] + int_to_double[jj] + 2.0));
            }
         }

         // P(n,n) term, equation (7-8)
         Pnm[ii][ii] = sqrt ((2.0 * int_to_double[ii] + 1.0)
                      / (2.0 * int_to_double[ii])) * Pnm[ii - 1][ii - 1];

         // P(n,n+1) and P(n,n+2) terms, table 1 (p. 14)
         Pnm[ii][ii + 1] = 0.0;
         Pnm[ii][ii + 2] = 0.0;

         // Equation (7-15) and (7-16)
         nrdiag[ii] = sqrt (2.0 * int_to_double[ii] + 1.0) * Pnm[ii - 1][ii - 1];

         // Equation (7-13)
         alpha[ii] = sqrt ((2.0 * int_to_double[ii] + 1.0)
                      * (2.0 * int_to_double[ii] - 1.0)) / int_to_double[ii];
         beta[ii] = sqrt ((2.0 * int_to_double[ii] + 1.0)
                      / (2.0 * int_to_double[ii] - 3.0))
                      * (int_to_double[ii] - 1.0) / int_to_double[ii];
      }


      // Clean up local variable Pnm (needed to init nrdiag above)
      for (unsigned int ii = 0; ii <= degree; ++ii) {
         JEOD_DELETE_ARRAY (Pnm[ii]);
      }
      JEOD_DELETE_ARRAY (Pnm);
   }

   return;
}


/**
 * Find the given variational gravity effect if already exists.
 * @return Index number of delta-coeff; -1 if not found
 * \param[in] delta_coeff delta-coeff to be found
 */
int
SphericalHarmonicsGravitySource::find_deltacoeff (
   const SphericalHarmonicsDeltaCoeffs & delta_coeff)
const
{
   int deltacoeff_idx            = 0;
   unsigned int found_deltacoeff = 0;  // a "flag" for when idx = 0 on purpose
   unsigned int n_deltacoeffs    = delta_coeffs.size();

   for (unsigned int ii = 0; ii < n_deltacoeffs; ++ii) {
      if (std::strcmp (typeid(delta_coeff).name(),
                       typeid(delta_coeffs[ii]).name()) == 0) {
         found_deltacoeff = 1;
         deltacoeff_idx   = ii;
         break;
      }
   }
   if (!found_deltacoeff) {
      deltacoeff_idx = -1;
   }
   return deltacoeff_idx;
}


/**
 * Add a gravitational variation effect (i.e., a delta coeffs) to the
 * vector of effects.
 * \param[in] var_init Effect init structure
 * \param[in] dyn_manager Dynamics manager
 * \param[in] var_effect Delta coeff to be added
 */
void
SphericalHarmonicsGravitySource::add_deltacoeff (
   SphericalHarmonicsDeltaCoeffsInit & var_init,
   BaseDynManager & dyn_manager,
   SphericalHarmonicsDeltaCoeffs & var_effect)
{

   // Variational effect must be unique for each grav body. Check for conflicts.
   if (find_deltacoeff (var_effect) != -1) {
      MessageHandler::fail (
         __FILE__, __LINE__, GravityMessages::duplicate_entry,
         "Duplicate gravitational variation effect '%s'",
         typeid(var_effect).name());
      return;
   }

   // Save the new effect in the delta_coeffs container.
   delta_coeffs.push_back (&var_effect);

   // Populate the grav_source ptr inside the given delta_coeffs
   var_effect.grav_source = this;

   // Send the init structure to the delta-coeff for it to set itself up with
   var_effect.initialize (var_init, dyn_manager);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
