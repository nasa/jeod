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
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 * @addtogroup GaussJackson
 * @{
 *
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_config.hh
 * Defines the class GaussJacksonConfig, which specifies
 * Gauss-Jackson configuration data.
 */


/*
Purpose: ()
Library dependencies:
  ((../src/gauss_jackson_config.cc))
*/


#ifndef JEOD_GAUSS_JACKSON_CONFIG_HH
#define JEOD_GAUSS_JACKSON_CONFIG_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integration_technique.hh"


//! Namespace jeod 
namespace jeod {

/**
 * Contains Gauss-Jackson configuration data.
 * All member data are public; this is esentially a struct.
 */
class GaussJacksonConfig {

JEOD_MAKE_SIM_INTERFACES(GaussJacksonConfig)

public:

   // Static member functions.

   /**
    * Creates a GaussJacksonConfig with all members set to -1.
    * This otherwise invalid value has a special meaning to the validation
    * function. When encountered, the item is silently replaced with
    * the default for that item.
    */
   static GaussJacksonConfig default_configuration();

   /**
    * Creates a GaussJacksonConfig with all members set to their defaults.
    */
   static GaussJacksonConfig standard_configuration();

   /**
    * Creates a GaussJacksonConfig based on the supplied configuration.
    * Values of -1 are replaced with their defaults.
    * The standard configuration is used if any invalid item is invalid.
    */
   static GaussJacksonConfig validate_configuration (
      const GaussJacksonConfig& config);


   // Member data.

   /**
    * The integration technique to be used to prime the Gauss-Jackson process.
    * Defaults to er7_utils::Integration::Unspecified, the interpretation of
    * which depends on the initial order.
    */
   er7_utils::Integration::Technique priming_technique; //!< trick_units(--)

   /**
    * The order of the Gauss Jackson integrator immediately after priming.
    * This must be an even number and must be 14 or less.
    * Defaults to 4.
    */
   unsigned int initial_order; //!< trick_units(--)

   /**
    * The order of the Gauss Jackson integrator once it's operational.
    * This must be an even number between initial_order and 14, inclusive.
    * Defaults to 12.
    */
   unsigned int final_order; //!< trick_units(--)

   /**
    * The number of time doubling steps involved in the bootstrap operation.
    * Defaults to (final_order - initial_order)/2.
    */
   unsigned int ndoubling_steps; //!< trick_units(--)

   /**
    * Maximum number of correction steps allowed before the integrator is
    * deemed to be not converging.
    * The algorithm is run in predict-only mode if this limit is zero.
    * The corrector is applied but once with the limit is one.
    * A one-time warning is issued if the limit is 2 or more and if the
    * the algorithm would make more corrections were it not for this limit.
    * Defaults to 10.
    */
   unsigned int max_correction_iterations; //!< trick_units(--)

   /**
    * Number that indicates the allowable relative difference for two
    * states to be considered converged.
    * Defaults to 1e-14.
    */
   double relative_tolerance; //!< trick_units(--)

   /**
    * Number that indicates the allowable absolute difference for two
    * states to be considered converged.
    * Defaults to 1e-10.
    */
   double absolute_tolerance; //!< trick_units(--)



   // Note: The implicitly-defined default constructor, copy constructor,
   // destructor, and assignment operator are exactly what the doctor ordered.
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
