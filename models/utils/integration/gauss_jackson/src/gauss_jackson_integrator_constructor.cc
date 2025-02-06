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
 * @file models/utils/integration/gauss_jackson/src/gauss_jackson_integrator_constructor.cc
 * Defines member functions for the class GaussJacksonIntegratorConstructor.
 */


/*
Purpose: ()
*/


// Model includes
#include "../include/gauss_jackson_integrator_constructor.hh"
#include "../include/gauss_jackson_first_order_ode_integrator.hh"
#include "../include/gauss_jackson_simple_second_order_ode_integrator.hh"
#include "../include/gauss_jackson_generalized_second_order_ode_integrator.hh"

// Integration includes
#include "er7_utils/integration/core/include/integrator_constructor_factory.hh"
#include "er7_utils/integration/core/include/integrator_constructor_utils.hh"
#include "er7_utils/integration/core/include/integration_controls.hh"
#include "er7_utils/integration/core/include/integration_messages.hh"
#include "er7_utils/integration/core/include/integrator_constructor_factory.hh"

// Interface includes
#include "er7_utils/interface/include/alloc.hh"
#include "er7_utils/interface/include/message_handler.hh"

// System includes
#include <algorithm>



//! Namespace jeod 
namespace jeod {

/**
 * Cast the provided integration controls to a GaussJacksonIntegrationControls.
 * @param  controls  Generic controls to be cast.
 * @return GaussJacksonIntegrationControls pointer, guaranteed to be non-null.
 */
static GaussJacksonIntegrationControls *
cast_to_gj_controls (
   er7_utils::IntegrationControls & controls)
{
   GaussJacksonIntegrationControls * gj_controls =
      dynamic_cast<GaussJacksonIntegrationControls*> (&controls);
   if (gj_controls == nullptr) {
      er7_utils::MessageHandler::fail (
         __FILE__, __LINE__,
         er7_utils::IntegrationMessages::invalid_request,
         "Unable to convert IntegrationControls at %p "
         "to a GaussJacksonIntegrationControls.",
         &controls);
   }
   return gj_controls;
}


// Named constructor; create an GaussJacksonIntegratorConstructor.
er7_utils::IntegratorConstructor*
GaussJacksonIntegratorConstructor::create_constructor ()
{
   return er7_utils::alloc::allocate_object<
             GaussJacksonIntegratorConstructor> ();
}


// Default constructor.
GaussJacksonIntegratorConstructor::GaussJacksonIntegratorConstructor ()
:
   er7_utils::Er7UtilsDeletable (),
   er7_utils::IntegratorConstructor (),
   priming_constructor (nullptr),
   config ()
{
}


// Copy constructor.
GaussJacksonIntegratorConstructor::GaussJacksonIntegratorConstructor (
   const GaussJacksonIntegratorConstructor & src)
:
   er7_utils::Er7UtilsDeletable (),
   er7_utils::IntegratorConstructor (src),
   priming_constructor (nullptr),
   config (src.config)
{
   if (src.priming_constructor != nullptr) {
      priming_constructor = src.priming_constructor->create_copy ();
   }
}


// Destructor.
GaussJacksonIntegratorConstructor::~GaussJacksonIntegratorConstructor ()
{
   er7_utils::alloc::delete_object (priming_constructor);
}


// Non-throwing swap.
void
GaussJacksonIntegratorConstructor::swap (
   GaussJacksonIntegratorConstructor & src)
{
   std::swap (priming_constructor, src.priming_constructor);
   std::swap (config, src.config);
}


// Configure.
void
GaussJacksonIntegratorConstructor::configure (
   const GaussJacksonConfig & config_in,
   er7_utils::Integration::Technique priming_technique)
{
   config = config_in;
   if (priming_technique != er7_utils::Integration::Unspecified) {
      config.priming_technique = priming_technique;
   }
   config = GaussJacksonConfig::validate_configuration (config);

   priming_constructor =
      er7_utils::IntegratorConstructorFactory::create (
         config.priming_technique);
}


// Configure.
void
GaussJacksonIntegratorConstructor::configure (
   const GaussJacksonConfig & config_in,
   const er7_utils::IntegratorConstructor & priming_cotr_in)
{
   config = GaussJacksonConfig::validate_configuration (config_in);
   priming_constructor = priming_cotr_in.create_copy();
}


// Create a duplicate of the constructor.
er7_utils::IntegratorConstructor *
GaussJacksonIntegratorConstructor::create_copy ()
const
{
   return er7_utils::alloc::replicate_object (*this);
}


// Create an GaussJackson integration controls.
er7_utils::IntegrationControls *
GaussJacksonIntegratorConstructor::create_integration_controls ()
const
{
   // Configure this object with the standard config if not already configured.
   if (priming_constructor == nullptr) {
      (const_cast<GaussJacksonIntegratorConstructor*>(this))->configure (GaussJacksonConfig::standard_configuration ());
   }

   return er7_utils::alloc::allocate_object<
                GaussJacksonIntegrationControls,
                const er7_utils::IntegratorConstructor &,
                const GaussJacksonConfig &> (
             *priming_constructor, config); 
}


// Create an GaussJackson state integrator for a first order ODE.
er7_utils::FirstOrderODEIntegrator *
GaussJacksonIntegratorConstructor::create_first_order_ode_integrator (
   unsigned int size,
   er7_utils::IntegrationControls & controls)
const
{
   GaussJacksonIntegrationControls * gj_controls =
      cast_to_gj_controls (controls);

   return er7_utils::alloc::allocate_object<
                GaussJacksonFirstOrderODEIntegrator,
                const er7_utils::IntegratorConstructor &,
                GaussJacksonIntegrationControls &,
                unsigned int,
                er7_utils::IntegrationControls &> (
             *priming_constructor, *gj_controls,
             size, gj_controls->get_priming_controls());
}


// Create an GaussJackson state integrator for a second order ODE.
er7_utils::SecondOrderODEIntegrator *
GaussJacksonIntegratorConstructor::create_second_order_ode_integrator (
   unsigned int size,
   er7_utils::IntegrationControls & controls)
const
{
   GaussJacksonIntegrationControls * gj_controls =
      cast_to_gj_controls (controls);

   return er7_utils::alloc::allocate_object<
                GaussJacksonSimpleSecondOrderODEIntegrator,
                const er7_utils::IntegratorConstructor &,
                GaussJacksonIntegrationControls &,
                unsigned int,
                er7_utils::IntegrationControls &> (
             *priming_constructor, *gj_controls,
             size, gj_controls->get_priming_controls());
}


// Create an GaussJackson state integrator for a second order ODE.
er7_utils::SecondOrderODEIntegrator *
GaussJacksonIntegratorConstructor::
create_generalized_deriv_second_order_ode_integrator (
   unsigned int position_size,
   unsigned int velocity_size,
   const er7_utils::GeneralizedPositionDerivativeFunctions & deriv_funs,
   er7_utils::IntegrationControls & controls)
const
{
   GaussJacksonIntegrationControls * gj_controls =
      cast_to_gj_controls (controls);

   return er7_utils::alloc::allocate_object<
                GaussJacksonGeneralizedDerivSecondOrderODEIntegrator,
                const er7_utils::IntegratorConstructor &,
                GaussJacksonIntegrationControls &,
                unsigned int,
                unsigned int,
                const er7_utils::GeneralizedPositionDerivativeFunctions &,
                er7_utils::IntegrationControls &> (
             *priming_constructor, *gj_controls,
             position_size, velocity_size, deriv_funs,
             gj_controls->get_priming_controls());
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
