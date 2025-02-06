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
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_integrator_constructor.hh
 * Defines the class GaussJacksonIntegratorConstructor, which constructs
 * integrators that use Gauss-Jackson integration.
 */


/*
Purpose: ()
Library dependencies:
  ((gauss_jackson_integrator_constructor.o))
*/


#ifndef JEOD_GAUSS_JACKSON_INTEGRATOR_CONSTRUCTOR_HH
#define JEOD_GAUSS_JACKSON_INTEGRATOR_CONSTRUCTOR_HH

// Local includes
#include "gauss_jackson_config.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// er7 utils integration includes
#include "er7_utils/integration/core/include/integrator_constructor.hh"

// System includes


//! Namespace jeod 
namespace jeod {

/**
 * Create state and time integrators that propagate using Gauss-Jackson.
 */
class GaussJacksonIntegratorConstructor :
   public er7_utils::IntegratorConstructor {
JEOD_MAKE_SIM_INTERFACES(GaussJacksonIntegratorConstructor)

public:


   // Static member functions.

   /**
    * Named constructor; create an GaussJacksonIntegratorConstructor instance.
    * The caller is responsible for deleting the returned object.
    * @return Newly created GaussJacksonIntegratorConstructor instance.
    */
   static er7_utils::IntegratorConstructor* create_constructor (void);


   // Constructors, destructor, and assignment operator.

   /**
    * GaussJackson default constructor.
    */
   GaussJacksonIntegratorConstructor (void);

   /**
    * GaussJacksonIntegratorConstructor copy constructor.
    */
   GaussJacksonIntegratorConstructor (
      const GaussJacksonIntegratorConstructor & src);

   /**
    * GaussJacksonIntegratorConstructor destructor.
    */
   ~GaussJacksonIntegratorConstructor ();

   /**
    * GaussJacksonIntegratorConstructor assignment operator.
    */
   GaussJacksonIntegratorConstructor & operator= (
      GaussJacksonIntegratorConstructor src)
   {
      swap (src);
      return *this;
   }


   // Member functions.

   /**
    * Configure the Gauss-Jackson integrator constructor.
    */
   void configure (
      const GaussJacksonConfig & config_in,
      er7_utils::Integration::Technique priming_technique =
         er7_utils::Integration::Unspecified);

   /**
    * Configure the Gauss-Jackson integrator constructor.
    */
   void configure (
      const GaussJacksonConfig & config_in,
      const er7_utils::IntegratorConstructor & priming_cotr_in);

   /**
    * Return the class name.
    */
   virtual const char * get_class_name (void) const
   { return "GaussJacksonIntegratorConstructor"; }

   /**
    * GaussJackson does not implement a 2nd order generalized step integrator.
    */
   virtual bool implements (
      er7_utils::Integration::ODEProblemType problem_type)
   const
   {
      return (problem_type !=
              er7_utils::Integration::GeneralizedStepSecondOrderODE);
   }

   /**
    * GaussJackson does not provide a 2nd order generalized step integrator.
    */
   virtual bool provides (
      er7_utils::Integration::ODEProblemType problem_type)
   const
   {
      return (problem_type !=
              er7_utils::Integration::GeneralizedStepSecondOrderODE);
   }

   /**
    * Non-throwing swap.
    * @param[in,out] src  Object with which contents are to be swapped.
    */
   virtual void swap (GaussJacksonIntegratorConstructor & src);

   /**
    * Create a duplicate of the constructor.
    * The caller is responsible for deleting the returned object.
    * @return Duplicated constructor.
    */
   virtual er7_utils::IntegratorConstructor * create_copy (void) const;

   /**
    * Create an integration controls that guides the GaussJackson integration
    * process. The caller is responsible for deleting the created object.
    * @return Integration controls object
    */
   virtual er7_utils::IntegrationControls *
   create_integration_controls (void) const;

   /**
    * Create a GaussJackson state integrator for a first order ODE.
    * The caller is responsible for deleting the created object.
    * @return State integrator
    * @param[in]     size      State size
    * @param[in,out] controls  Integration controls
    */
   virtual er7_utils::FirstOrderODEIntegrator *
   create_first_order_ode_integrator (
      unsigned int size,
      er7_utils::IntegrationControls & controls) const;

   /**
    * Create a GaussJackson state integrator for a simple second order ODE.
    * The caller is responsible for deleting the created object.
    * @return State integrator
    * @param[in]     size      State size
    * @param[in,out] controls  Integration controls
    */
   virtual er7_utils::SecondOrderODEIntegrator *
   create_second_order_ode_integrator (
      unsigned int size,
      er7_utils::IntegrationControls & controls) const;

   /**
    * Create a GaussJackson state integrator for a generalized second order ODE
    * where generalized position is advanced with the use of the
    * position derivative function.
    * The caller is responsible for deleting the created object.
    * @return State integrator
    * @param[in]     position_size  Size of the generalized position
    * @param[in]     velocity_size  Size of the generalized velocity
    * @param[in]     deriv_funs     Position derivative functions container
    * @param[in,out] controls       Integration controls
    */
   virtual er7_utils::SecondOrderODEIntegrator *
   create_generalized_deriv_second_order_ode_integrator (
      unsigned int position_size,
      unsigned int velocity_size,
      const er7_utils::GeneralizedPositionDerivativeFunctions & deriv_funs,
      er7_utils::IntegrationControls & controls) const;

   /**
    * GaussJackson can use a large number of steps per Trick cycle.
    * The magic number 192 is for order=16, ndboubling=6.
    * @return Always returns 192.
    */
   virtual unsigned int get_buffer_size (void) const
   { return 192; }

   /**
    * GaussJackson uses two steps per cycle once primed.
    * @return Always returns 2.
    */
   virtual unsigned int get_transition_table_size (void) const
   { return 2; }

private:

   /**
    * The integrator constructor that creates the priming integrators.
    */
   er7_utils::IntegratorConstructor * priming_constructor; //!< trick_units(--)

   /**
    * Data used to configure the Gauss-Jackson integration process.
    */
   GaussJacksonConfig config; //!< trick_units(--)

};

} // End JEOD namespace


#ifdef TRICK_ICG
#include "gauss_jackson_integration_controls.hh"
#include "gauss_jackson_first_order_ode_integrator.hh"
#include "gauss_jackson_simple_second_order_ode_integrator.hh"
#include "gauss_jackson_generalized_second_order_ode_integrator.hh"
#endif


#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
