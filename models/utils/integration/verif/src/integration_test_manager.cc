/*******************************************************************************

Purpose:
  (Define methods for the class IntegrationTest.)

Reference:
  (((TBS)))

Library dependencies:
  ((integration_test.cc)
   (integration_test_messages.cc)
   (new_orientation.cc)
   (random.cc)
   (environment/time/src/time_manager.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item_demangle.cc)
   (utils/quaternion/src/quat_to_mat.cc))

 
*******************************************************************************/


// Model includes
#include "../include/integration_test.hh"
#include "../include/integration_test_manager.hh"
#include "../include/integration_test_messages.hh"

// JEOD includes
#include "environment/time/include/time_manager.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/integration/gauss_jackson/include/gauss_jackson_integrator_constructor.hh"
#include "utils/integration/lsode/include/lsode_integrator_constructor.hh"
#include "utils/sim_interface/include/jeod_trick_integrator.hh"
#include "utils/sim_interface/include/simulation_interface.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrator_constructor.hh"
#include "er7_utils/integration/core/include/integrator_constructor_factory.hh"

// System includes
#include <cmath>
#include <cstddef>
#include <typeinfo>
#include <sys/select.h>

//! Namespace jeod
namespace jeod {

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES (TrickIntegrator)
JEOD_DECLARE_ATTRIBUTES (IntegrationTest)
JEOD_DECLARE_ATTRIBUTES (std::string)


/**
 * Maximum number of template items under test.
 */
const unsigned int IntegrationTestManager::template_items_size = 6;


/*
 Purpose: (Default constructor.)
*/
IntegrationTestManager::IntegrationTestManager (void) :
   generate_report(true),
   debug(false),
   regression_test(false),
   base_case(0),
   num_tests(1),
   total_tests(0),
   cycle_count(0),
   nskip(0),
   report_fid(1),
   end_time(-1),
   sim_integrator(nullptr),
   integ_option(er7_utils::Integration::Unspecified),
   integ_option_int(0),
   test_items(nullptr),
   time_scale(1),
   deprecated_rotation_integration(false),
   initialized(false),
   integ_constructor(nullptr),
   integ_interface(nullptr)
   // time_integrator(NULL)
{
   JEOD_REGISTER_CLASS (IntegrationTest);

   template_items = JEOD_ALLOC_CLASS_POINTER_ARRAY (
                       template_items_size, IntegrationTest);
   for (unsigned int ii = 0; ii < template_items_size; ++ii) {
      template_items[ii] = nullptr;
   }
}


/*
Purpose: (Destructor.)
*/
IntegrationTestManager::~IntegrationTestManager ( // Return: N/A (destructor)
   void)
{
   if (JEOD_IS_ALLOCATED (integ_interface)) {
      JEOD_DELETE_OBJECT (integ_interface);
      integ_interface = nullptr;
   }

   if (integ_group != nullptr) {
      JEOD_DELETE_OBJECT (integ_group);
      integ_group = nullptr;
   }

   if (test_items != nullptr) {
      if (num_tests > 1) {
         for (unsigned int ii = 0; ii < total_tests; ++ii) {
            IntegrationTest * test = test_items[ii];
            if (test != nullptr) {
               test_items[ii] = nullptr;
               JEOD_DELETE_OBJECT (test);
            }
         }
      }
      JEOD_DELETE_ARRAY (test_items);
      test_items = nullptr;
   }

   if (template_items != nullptr) {
      JEOD_DELETE_ARRAY (template_items);
      template_items = nullptr;
   }

   if (state_integrator_id != nullptr) {
      JEOD_DELETE_OBJECT (state_integrator_id);
      state_integrator_id = nullptr;
   }

   er7_utils::Er7UtilsDeletable::delete_instance (integ_constructor);
}


/*
Purpose:
  (Initialize the test items.)
*/
void
IntegrationTestManager::initialize (   // Return: -- Void
   TimeManager & time_manager,  // Inout:  -- Time manager
   double sim_dt)               // In:     -- Sim. dynamic step size
{
   unsigned int num_template_items;


   // New style setup. The Trick INTEGRATOR is a part of the integrator I/F.
   // Construct the integrator interface and get the simulation's integrator
   // object from the sim interface.
   integ_interface = JeodSimulationInterface::create_integrator_interface();
   sim_integrator = integ_interface->get_integrator();

   // Count the number of supplied template items.
   num_template_items = template_items_size;
   for (unsigned int ii = 0; ii < template_items_size; ++ii) {
      if (template_items[ii] == nullptr) {
         num_template_items = ii;
         break;
      }
   }

   // Sanity check: Number of template items must be positive.
   if (num_template_items == 0) {
      MessageHandler::fail (__FILE__, __LINE__,
                            IntegrationTestMessages::invalid_data,
                            "No template items specified for testing.");
   }

   // For regression testing,
   // - Make each test article have a fixed orientation
   // - Sanity check that each article will be tested once.
   if (regression_test) {
      for (unsigned int ii = 0; ii < num_template_items; ++ii) {
         template_items[ii]->set_deprecated_rotation_integration (
            deprecated_rotation_integration);
         template_items[ii]->configure (false, 0);
      }
      if (num_tests != 1) {
         MessageHandler::warn (__FILE__, __LINE__,
                               IntegrationTestMessages::invalid_data,
                               "Number of tests reset to 1.");
         num_tests = 1;
      }
   }

   // Non-regression tests:
   // - Set each test article's random case # to this object's base case.
   // - Sanity check that each article will be tested at least once.
   else {
      for (unsigned int ii = 0; ii < num_template_items; ++ii) {
         template_items[ii]->set_deprecated_rotation_integration (
            deprecated_rotation_integration);
         template_items[ii]->configure (true, base_case);
      }
      if (num_tests == 0) {
         MessageHandler::warn (__FILE__, __LINE__,
                               IntegrationTestMessages::invalid_data,
                               "Number of tests reset to 1.");
         num_tests = 1;
      }
   }


   // Create the integrator constructor if not set in the input file.
   if (integ_constructor == nullptr) {
      if ((integ_option_int >= 122) && (integ_option_int <= 134)) {
         GaussJacksonIntegratorConstructor gj_integ_constructor;
         GaussJacksonConfig gj_config =
            GaussJacksonConfig::default_configuration();
         nskip = gj_config.final_order = integ_option_int - 120;
         gj_integ_constructor.configure (gj_config);
         integ_constructor = gj_integ_constructor.create_copy ();
      }
      else if ((integ_option_int >=140) && (integ_option_int <150)) {
         LsodeIntegratorConstructor lsode_integ_constructor;
         lsode_integ_constructor.data_interface.set_abs_tol( 0 , 0.0);
         lsode_integ_constructor.data_interface.set_rel_tol( 0 , 2.3E-16);
         for (unsigned int ii=integ_option_int; ii<=144 && ii>140; ii--) {
           lsode_integ_constructor.data_interface.rel_tolerance_error_control_vec[0] *= 10.0;
         }
         if (integ_option_int == 145) {
           lsode_integ_constructor.data_interface.abs_tolerance_error_control_vec[0] = 1.0E-9;
         }
         if (integ_option_int == 146) {
           lsode_integ_constructor.data_interface.integration_method =
                                               LsodeControlDataInterface::ImplicitBackDiffStiff;
         }
         if (integ_option_int == 147) {
           lsode_integ_constructor.data_interface.corrector_method =
                                          LsodeControlDataInterface::NewtonIterInternalJac;
         }
         if (integ_option_int == 148) {
           lsode_integ_constructor.data_interface.corrector_method =
                                          LsodeControlDataInterface::JacobiNewtonInternalJac;
         }
         if (integ_option_int == 149) {
           lsode_integ_constructor.data_interface.max_order = 5;
         }


         integ_constructor = lsode_integ_constructor.create_copy();
      }
      else {
         integ_constructor = er7_utils::IntegratorConstructorFactory::create (
                                integ_option);
      }
   }

   // Create and initialize the integration group.
   integ_group = JEOD_ALLOC_CLASS_OBJECT (
                    JeodIntegrationGroup,
                    (*this,
                     *integ_constructor,
                     *integ_interface,
                     time_manager.get_jeod_integration_time()));
   integ_group->initialize_group();

   // Set the time scale based on the time manager's time scale.
   time_scale = time_manager.dyn_time.scale_factor;


   // Create the test items array, setting the contents to NULL pointers.
   total_tests = num_template_items * num_tests;
   test_items  = JEOD_ALLOC_CLASS_POINTER_ARRAY (total_tests, IntegrationTest);
   for (unsigned int ii = 0; ii < total_tests; ++ii) {
      test_items[ii] = nullptr;
   }


   // Populate the test_case array based on the template_items,
   // initializing each test item.

   // Multiple test mode: Use the template items as templates.
   if (num_tests > 1) {
      for (unsigned int iitem = 0; iitem < num_template_items; ++iitem) {
         IntegrationTest * template_item = template_items[iitem];
         for (unsigned int itest = 0; itest < num_tests; ++itest) {
            IntegrationTest * item = template_item->replicate (itest);
            item->set_deprecated_rotation_integration (
               deprecated_rotation_integration);
            item->initialize (integ_option, integ_constructor, sim_dt);
            test_items[iitem*num_tests + itest] = item;
            // item->validate_integrability (integ_constructor);
            if (item->get_active()) {
               integ_group->add_integrable_object (*item);
            }
         }
      }
   }

   // Single case: Use the template items themselves so as to enable logging.
   else {
      for (unsigned int iitem = 0; iitem < num_template_items; ++iitem) {
         IntegrationTest * item = template_items[iitem];
         item->initialize (integ_option, integ_constructor, sim_dt);
         test_items[iitem] = item;
         if (item->get_active()) {
            integ_group->add_integrable_object (*item);
         }
      }
   }


   // Report the created integrators for debugging mode.
   if (debug) {
      state_integrator_id =
         JEOD_ALLOC_CLASS_OBJECT (
            std::string,
            (NamedItem::demangle (test_items[0]->integrator_id())));

      fprintf (stderr,
               "IntegrationTest debug:\n"
               "  %s created state integrator %s\n\n",
               NamedItem::demangle (typeid(*integ_constructor)).c_str(),
               state_integrator_id->c_str());
   }

   // At this point the simulation should run to completion.
   initialized = true;
}


/*
Purpose:
  (Reset the integrators.)
 */
void
IntegrationTestManager::reset_integrators (  // Return: -- Void
   const TimeManager & time_manager)         // In:     -- Time manager
{

   if (debug) {
      fprintf (stderr,
               "\nIntegrationTest debug:\n"
               "  Resetting integrators at sim time = %g, dyn time = %g\n\n",
               time_manager.simtime, time_manager.dyn_time.seconds);
   }

   time_scale = time_manager.dyn_time.scale_factor;
   integ_group->reset_integrators();
}


/*
Purpose: Compute derivatives.
*/
void
IntegrationTestManager::compute_derivatives (  // Return: -- Void
   void)
{
   for (unsigned int ii = 0; ii < total_tests; ++ii) {
      if (test_items[ii]->get_active()) {
         test_items[ii]->compute_derivatives ();
      }
   }
}


/*
Purpose:
  (Integrate the integrated states.)
 */
int
IntegrationTestManager::integrate ( // Return: -- Integration step number
   double sim_begtime,          // In:     s  Sim time to integrate from
   TimeManager & time_manager)  // Inout:  -- Time manager
{
   unsigned int step_number;    // -- Next integration intermediate step

   // Integrate.
   step_number = integ_group->integrate_group_from (sim_begtime);

   // Report on integration status.
   if (debug) {
      fprintf (stderr,
               "IntegrationTest debug:\n"
               "  Integration cycle step number = %u\n"
               "  Propagating state and time from sim time %g\n"
               "  Time manager advanced to sim time %g, dyn time %g\n"
               "  Integration cycle %s complete.\n\n",
               step_number,
               sim_begtime,
               time_manager.simtime,
               time_manager.dyn_time.seconds,
               ((step_number == 0) ? "is" : "is not"));
   }

   if (step_number == 0) {
      ++cycle_count;
   }

   return step_number;
}


/*
Purpose: Propagate true state and compute errors.
*/
void
IntegrationTestManager::propagate (   // Return: -- Void
   double dyn_time)                   // In:     s  Dynamic time
{
   double test_time;                  // s  Test item time
   for (unsigned int ii = 0; ii < total_tests; ++ii) {
      if (test_items[ii]->get_active()) {
         test_time = test_items[ii]->get_time_scale() * dyn_time;
         test_items[ii]->propagate (test_time, cycle_count > nskip);
      }
   }
}


/*
Purpose:
  (Generate the end of sim run report.)
*/
void
IntegrationTestManager::shutdown ( // Return: -- Void
   double sim_time,                // In:     s  Simulation time
   double dyn_time)                // In:     s  Dynamic time
{
   double test_time;               // s  Test item time
   FILE * report = nullptr;

   if (report_fid == 1) {
      report = stdout;
   }
   else {
      report = fdopen (report_fid, "w");
      if (report == nullptr) {
         MessageHandler::warn (__FILE__, __LINE__,
                               IntegrationTestMessages::internal_error,
                               "Unable to open report file to file desc %d.",
                               report_fid);
      }
   }

   // Never initialized!
   if (! initialized) {
      MessageHandler::warn (__FILE__, __LINE__,
                            IntegrationTestMessages::internal_error,
                            "Report not generated "
                            "due to initialization error.");

   }

   // Premature termination.
   else if (sim_time < end_time - 1.5) {
      MessageHandler::warn (__FILE__, __LINE__,
                            IntegrationTestMessages::internal_error,
                            "Report not generated "
                            "due to early termination at t=%g", sim_time);
   }

   // Shutdown each test item, but only if a report is to be generated.
   else if (generate_report && (report != nullptr)) {
      for (unsigned int ii = 0; ii < total_tests; ++ii) {
         if (test_items[ii]->get_active()) {
            test_time = test_items[ii]->get_time_scale() * dyn_time;
            test_items[ii]->shutdown (sim_time, test_time, report);
         }
      }
      fflush (report);
   }

   // Close the report.
   if ((report_fid != 1) && (report != nullptr)) {
      fclose (report);
   }
}
} // End JEOD namespace
