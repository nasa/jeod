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
/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/include/spherical_harmonics_tidal_effects.hh
 * Define the class SphericalHarmonicsTidalEffects, which is the base class for
 * solid-body and ocean tidal effects. SphericalHarmonicsTidalEffects inherits
 * directly from the SphericalHarmonicsDeltaCoeffs class.
 */

/*******************************************************************************

Purpose:
  ()

References:
  (((none)))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((../src/spherical_harmonics_tidal_effects.cc))


*******************************************************************************/


#ifndef JEOD_SPHERICAL_HARMONICS_TIDAL_EFFECTS_HH
#define JEOD_SPHERICAL_HARMONICS_TIDAL_EFFECTS_HH


// System includes

// JEOD includes
#include "environment/planet/include/class_declarations.hh"
#include "utils/ref_frames/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "spherical_harmonics_delta_coeffs.hh"
#include "class_declarations.hh"


//! Namespace jeod
namespace jeod {

class BaseDynManager;


/**
 * Models tidal effects as a delta on top of a gravity model.
 */
class SphericalHarmonicsTidalEffects : public SphericalHarmonicsDeltaCoeffs {

 JEOD_MAKE_SIM_INTERFACES(SphericalHarmonicsTidalEffects)


 // Member data
 public:

   /**
    * Copy of polar motion coefficient xp (from polar motion class).
    */
   double xp; //!< trick_units(--)

   /**
    * Copy of polar motion coefficient yp (from polar motion class).
    */
   double yp; //!< trick_units(--)

   /**
    * The love number. Only used for a first order tidal effect model.
    */
   double k2; //!< trick_units(--)

   /**
    * A matrix of love numbers. Used for higher order (not first-order)
    * tidal effects.
    */
   double** Knm; //!< trick_units(--)

   /**
    * The number of tidal bodies named in tidal_bodies.
    */
   unsigned int num_tidal_bodies; //!< trick_units(count)

protected:

   /**
    * The tidal bodies. Filled out at initialization.
    * Length after init is num_tidal_bodies.
    */
   Planet** tidal_bodies; //!< trick_units(--)

   /**
    * Pointers to the tidal_bodies inertial reference frames.
    */
   RefFrame** tidal_bodies_inertial; //!< trick_units(--)

   /**
    * The planet fixed reference frame of the subject body.
    */
   RefFrame* pfix; //!< trick_units(--)


 // Member functions
 public:

   // Constructor & Destructor
   SphericalHarmonicsTidalEffects ();
   ~SphericalHarmonicsTidalEffects () override;

   void initialize (
      SphericalHarmonicsDeltaCoeffsInit & var_init,
      BaseDynManager& dyn_manager) override;

   void update (SphericalHarmonicsGravityControls & controls) override;

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
