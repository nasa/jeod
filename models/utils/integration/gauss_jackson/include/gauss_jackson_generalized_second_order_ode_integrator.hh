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
 * @addtogroup GaussJackson
 * @{
 *
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_generalized_second_order_ode_integrator.hh
 * Defines the class GaussJacksonGeneralizedDerivSecondOrderODEIntegrator,
 * which integrates a generalized derivative second order ODE using
 * Gauss-Jackson.
 */


/*
Purpose: ()
Library dependencies:
  ((gauss_jackson_generalized_second_order_ode_integrator.o))
*/


#ifndef JEOD_GAUSS_JACKSON_GENERALIZED_SECOND_ORDER_STATE_INTEGRATOR_HH
#define JEOD_GAUSS_JACKSON_GENERALIZED_SECOND_ORDER_STATE_INTEGRATOR_HH

// Local includes
#include "gauss_jackson_first_order_ode_integrator.hh"
#include "gauss_jackson_simple_second_order_ode_integrator.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"




//! Namespace jeod 
namespace jeod {

/**
 * Integrates a generalized derivative second order ODE using Gauss-Jackson.
 * Generalized position is integrated via a simple second order Gauss-Jackson
 * integrator. Generalized velocity is integrated via a first order summed
 * Adams integrator.
 */
class GaussJacksonGeneralizedDerivSecondOrderODEIntegrator :
   public er7_utils::SecondOrderODEIntegrator {

JEOD_MAKE_SIM_INTERFACES(GaussJacksonGeneralizedDerivSecondOrderODEIntegrator)

public:

   /**
    * Default constructor.
    */
   GaussJacksonGeneralizedDerivSecondOrderODEIntegrator ()
   :
      er7_utils::Er7UtilsDeletable (),
      SecondOrderODEIntegrator (),
      vel_integrator (),
      pos_integrator (),
      posdot (NULL),
      posdotdot (NULL)
   {}


   /**
    * Non-default constructor.
    * @param  priming_constructor  Integrator constructor for the technique
    *                              used during priming.
    * @param  controls             The Gauss-Jackson integration controls that
    *                              drives this state integrator.
    * @param  position_size        Generalized position vector size.
    * @param  velocity_size        Generalized velocity vector size.
    * @param  deriv_funs           Position vector time deriv functions.
    * @param  priming_controls     Integration controls used during priming.
    */
   GaussJacksonGeneralizedDerivSecondOrderODEIntegrator (
      const er7_utils::IntegratorConstructor & priming_constructor,
      GaussJacksonIntegrationControls & controls,
      unsigned int position_size,
      unsigned int velocity_size,
      const er7_utils::GeneralizedPositionDerivativeFunctions & deriv_funs,
      er7_utils::IntegrationControls & priming_controls);


   /**
    * Copy constructor.
    */
   GaussJacksonGeneralizedDerivSecondOrderODEIntegrator (
      const GaussJacksonGeneralizedDerivSecondOrderODEIntegrator & src);


   /**
    * Destructor.
    */
   ~GaussJacksonGeneralizedDerivSecondOrderODEIntegrator ();


   /**
    * Assignment operator.
    */
   GaussJacksonGeneralizedDerivSecondOrderODEIntegrator & operator= (
      GaussJacksonGeneralizedDerivSecondOrderODEIntegrator src)
   {
      swap (src);
      return *this;
   }


   /**
    * Non-throwing swap.
    */
   void swap (
      GaussJacksonGeneralizedDerivSecondOrderODEIntegrator & other);

   using SecondOrderODEIntegrator::swap;


   /**
    * Replicate this.
    */
   virtual er7_utils::SecondOrderODEIntegrator* create_copy() const;


   /**
    * Reset the integrator.
    */
   void reset_integrator ()
   {
      vel_integrator.reset_integrator ();
      pos_integrator.reset_integrator ();
   }


   /**
    * Integrate state.
    */
   er7_utils::IntegratorResult integrate (
      double dyn_dt,
      unsigned int target_stage,
      double const * ER7_UTILS_RESTRICT acc,
      double * ER7_UTILS_RESTRICT vel,
      double * ER7_UTILS_RESTRICT pos)
   {
      compute_posdot (pos, vel, posdot);
      compute_posdotdot (pos, vel, acc, posdotdot);

      vel_integrator.integrate (dyn_dt, target_stage, acc, vel);
      return pos_integrator.integrate (
                dyn_dt, target_stage, posdotdot, posdot, pos);
   }


private:

   /**
    * Integrator for the generalized velocity.
    */
   GaussJacksonFirstOrderODEIntegrator vel_integrator; //!< trick_units(--)

   /**
    * Integrator for the generalized position.
    */
   GaussJacksonSimpleSecondOrderODEIntegrator pos_integrator; //!< trick_units(--)

   /**
    * Generalized position time derivative.
    */
   double* posdot; //!< trick_units(--)

   /**
    * Generalized position second time derivative.
    */
   double* posdotdot; //!< trick_units(--)
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
