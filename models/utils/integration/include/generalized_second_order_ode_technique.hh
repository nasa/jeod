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
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 *
 * @file models/utils/integration/include/generalized_second_order_ode_technique.hh
 * Define the static class GeneralizedSecondOrderODETechnique.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((generalized_second_order_ode_technique.o))

 

*******************************************************************************/


#ifndef JEOD_GENERALIZED_SECOND_ORDER_ODE_TECHNIQUE_HH
#define JEOD_GENERALIZED_SECOND_ORDER_ODE_TECHNIQUE_HH

// System includes

// ER7 utilities includes
#include "er7_utils/integration/core/include/integration_technique.hh"


/**
 * Namespace er7_utils contains the state integration models used by JEOD.
 */
namespace er7_utils {
   class IntegratorConstructor;
}


//! Namespace jeod
namespace jeod {

/**
 * Enumerates the integration schemes, generalized Cartesian or Lie group
 * integration, and provides simple utilities that work with this.
 */
class GeneralizedSecondOrderODETechnique {

   // This is a static class. It has no constructors, no destructor,
   // and no assignment operator.

public:

   /**
    * Enumerates the types of second order ODE solvers that can be used
    * to integrate a generalized second order ODE.
    */
   enum TechniqueType {
      Unspecified, /**< No technique specified (an error). */
      Cartesian,   /**< Integrate using a generalized derivative scheme.
                        The integrator treats generalized position as if it
                        lives in some Cartesian space. */
      LieGroup     /**< Integrate using a generalized step scheme.
                        The integrator treats generalized position as if it
                        lives in some Lie group. */
   };

   /**
    * Test whether an integration method provides an integrator for the
    * specified technique.
    * @param[in]  generator  Integrator constructor for the integration technique.
    * @param[in]  technique  Technique to be queried.
    * @return True if the constructor can create an integrator for the
    *    specified technique, false otherwise.
    */
   static bool is_provided_by (
      const er7_utils::IntegratorConstructor & generator,
      TechniqueType technique);

   /**
    * Validate the specified technique with respect to the integration
    * method. Possible outcomes are:
    * - Failure if the generator doesn't provide either of the generalized
    *   second order ODE integrators.
    * - Switch to plan B if the generator doesn't provide the requested
    *   integrator but does provide the alternate technique.
    * - Nothing happens if the generator does provide the requested integrator.
    *
    * @param[in]  generator  Integrator constructor for the integration technique.
    * @param[in]  technique  Technique to be queried.
    * @param[in]  file       Typically __FILE__
    * @param[in]  line       Typically __LINE__
    * @param[in]  requester  Something to identify the caller.
    * @param[in]  name       The name of the object associated with the caller.
    *
    * @return Input technique if supported, alternate if not.
    *    The function does not return is neither of the options is supported.
    */
   static TechniqueType validate_technique (
      const er7_utils::IntegratorConstructor & generator,
      TechniqueType technique,
      const char * file,
      unsigned int line,
      const char * requester,
      const char * name);


private:

   /**
    * Not implemented.
    */
   GeneralizedSecondOrderODETechnique ();

   /**
    * Not implemented.
    */
   GeneralizedSecondOrderODETechnique (
      const GeneralizedSecondOrderODETechnique &);

   /**
    * Not implemented.
    */
   ~GeneralizedSecondOrderODETechnique ();

   /**
    * Not implemented.
    */
   GeneralizedSecondOrderODETechnique & operator= (
      const GeneralizedSecondOrderODETechnique &);

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
