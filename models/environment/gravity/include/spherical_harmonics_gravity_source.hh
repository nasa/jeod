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
/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/include/spherical_harmonics_gravity_source.hh
 * Define the spherical harmonics implementation of a gravity body.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((spherical_harmonics_gravity_source.o))


*******************************************************************************/


#ifndef JEOD_SPHERICAL_HARMONICS_GRAVITY_BODY_HH
#define JEOD_SPHERICAL_HARMONICS_GRAVITY_BODY_HH


// System includes
#include <vector>

// JEOD includes
#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "gravity_source.hh"


//! Namespace jeod
namespace jeod {

class BaseDynManager;
class EphemerisRefFrame;

/**
 * Models the gravity for a specific planet using spherical harmonics.
 */
class SphericalHarmonicsGravitySource : public GravitySource {

 JEOD_MAKE_SIM_INTERFACES(SphericalHarmonicsGravitySource)

 // Member data
 public:
   /**
    * Spherical harmonics distance scale, typically the planet's mean equatorial
    * radius.
    */
   double radius; //!< trick_units(m)

   /**
    * The degree of the spherical harmonics gravity coefficients.
    */
   unsigned int degree; //!< trick_units(--)

   /**
    * The order of the spherical harmonics gravity coefficients.
    */
   unsigned int order; //!< trick_units(--)

   /**
    * Normalized real (cosine) spherical harmonic coefficients.
    */
   double ** Cnm; //!< trick_units(--)

   /**
    * Normalized imaginary (sine) spherical harmonic coefficients.
    */
   double ** Snm; //!< trick_units(--)

   /**
    * Is C20 coefficient free of the permanent tide effect?
    */
   bool tide_free; //!< trick_units(--)

   /**
    * Number to be added to C20 to remove the permanent tide
    */
   double tide_free_delta; //!< trick_units(--)

   /**
    * (Planet radius/vehicle distance)^n
    */
   double * a_by_rad; //!< trick_units(--)
   /**
    * Gottlieb coefficient alpha
    */
   double * alpha; //!< trick_units(--)
   /**
    * Gottlieb coefficient beta
    */
   double * beta; //!< trick_units(--)
   /**
    * Gottlieb coefficient xi
    */
   double ** xi; //!< trick_units(--)
   /**
    * Gottlieb coefficient eta
    */
   double ** eta; //!< trick_units(--)
   /**
    * Gottlieb coefficient zeta
    */
   double ** zeta; //!< trick_units(--)
   /**
    * Gottlieb coefficient upsilon
    */
   double ** upsilon; //!< trick_units(--)
   /**
    * Gottlieb coefficient nrdiag
    */
   double * nrdiag; //!< trick_units(--)
   /**
    * 0 to degree+1 cast as doubles
    */
   double * int_to_double; //!< trick_units(--)

   /**
    * List of all gravity coefficient altering effects such as
    * solid-body tides
    */
   JeodPointerVector<SphericalHarmonicsDeltaCoeffs>::type delta_coeffs; //!< trick_io(**)


 // Make the copy constructor and assignment operator private
 // (and unimplemented) to avoid erroneous copies
 private:

   /**
    * Not implemented.
    */
   SphericalHarmonicsGravitySource (
      const SphericalHarmonicsGravitySource &);

   /**
    * Not implemented.
    */
   SphericalHarmonicsGravitySource & operator= (
      const SphericalHarmonicsGravitySource &);

 public:

   // Default constructor
   SphericalHarmonicsGravitySource ();

   // Destructor
   virtual ~SphericalHarmonicsGravitySource ();


   // Overloaded methods:

   // Initialize the spherical harmonics model
   virtual void initialize_body (void);


   // Find the index number for a given set of delta-coeffs;
   // Returns -1 if coeffs are not in the delta-coeffs vector.
   int find_deltacoeff (
      const SphericalHarmonicsDeltaCoeffs & delta_coeff) const;


   void add_deltacoeff (
      SphericalHarmonicsDeltaCoeffsInit & var_init,
      BaseDynManager & dyn_manager,
      SphericalHarmonicsDeltaCoeffs & var_effect);

};


} // End JEOD namespace

#ifdef TRICK_VER
#include "spherical_harmonics_delta_coeffs.hh"
#endif


#endif

/**
 * @}
 * @}
 * @}
 */
