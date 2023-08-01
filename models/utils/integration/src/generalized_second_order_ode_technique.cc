/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 *
 * @file models/utils/integration/src/generalized_second_order_ode_technique.cc
 * Define class GeneralizedSecondOrderODETechnique methods.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((generalized_second_order_ode_technique.cc)
   (integration_messages.cc)
   (utils/message/src/message_handler.cc)
  )

 
******************************************************************************/


// System includes
#include <cstddef>

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrator_constructor.hh"

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Local includes
#include "../include/generalized_second_order_ode_technique.hh"
#include "../include/integration_messages.hh"


//! Namespace jeod
namespace jeod {

// Will the specified integrator create an integrator of the specified type?
bool
GeneralizedSecondOrderODETechnique::is_provided_by (
   const er7_utils::IntegratorConstructor & generator,
   TechniqueType technique)
{
   switch (technique) {
   case Cartesian:
      return generator.provides (
         er7_utils::Integration::GeneralizedDerivSecondOrderODE);

   case LieGroup:
      return generator.provides (
         er7_utils::Integration::GeneralizedStepSecondOrderODE);

   case Unspecified:
   default:
      MessageHandler::error (
         __FILE__, __LINE__, IntegrationMessages::invalid_request,
         "No integration technique was specified\n"
         "for integrating rotational state.\n");
      return false;
   }
}


// What technique should be used? The input technique is viewed as a request
// rather than a mandate.
GeneralizedSecondOrderODETechnique::TechniqueType
GeneralizedSecondOrderODETechnique::validate_technique (
   const er7_utils::IntegratorConstructor & generator,
   TechniqueType technique,
   const char * file,
   unsigned int line,
   const char * requester,
   const char * name)
{
   // Replace a null name with something that won't drop core.
   if (name == nullptr) {
      name = "<unnamed>";
   }

   // A valid technique must have been specified.
   if ((technique != Cartesian) && (technique != LieGroup)) {
      MessageHandler::fail (
         file, line, IntegrationMessages::invalid_request,
         "No integration technique was specified\n"
         "for integrating rotational state.\n"
         "Error occurred in context of %s '%s",
         requester, name);
   }

   // Determine which of the two rotational integration formalisms the
   // user-specified integration technique provides.
   bool provides_cartesian = is_provided_by (generator, Cartesian);
   bool provides_lie_group = is_provided_by (generator, LieGroup);

   // It needs to provide at least one of them.
   if ((! provides_cartesian) && (! provides_lie_group)) {
      MessageHandler::fail (
         file, line, IntegrationMessages::invalid_request,
         "The integration constructor '%s' cannot be used\n"
         "for integrating rotational state.\n"
         "Error occurred in context of %s '%s",
         generator.get_class_name(), requester, name);
   }

   // Switch to Lie group integration if the user requested Cartesian
   // integration but the technique only supplies Lie group integration.
   if ((! provides_cartesian) && (technique == Cartesian)) {
      MessageHandler::inform (
         file, line, IntegrationMessages::unsupported_option,
         "The integration constructor '%s' does not provide\n"
         "a generalized derivative integrator.\n"
         "Switching to Lie group integration for %s '%s'.",
         generator.get_class_name(), requester, name);

      technique = LieGroup;
   }


   // Switch to Cartesian integration if the user requested Lie group
   // integration but the technique only supplies Cartesian integration.
   if ((! provides_lie_group) && (technique == LieGroup)) {
      MessageHandler::inform (
         file, line, IntegrationMessages::unsupported_option,
         "The integration constructor '%s' does not provide\n"
         "a generalized step integrator.\n"
         "Switching to Cartesian integration for %s '%s'.",
         generator.get_class_name(), requester, name);

      technique = Cartesian;
   }

   return technique;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
