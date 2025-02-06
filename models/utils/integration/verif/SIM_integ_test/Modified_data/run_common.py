################################################################################
#
# Purpose:
#   (Run the simulation per the options dictionary.)
#
#   
#
################################################################################


def set_run_common(options) :
   """
   Run the simulation per the options dictionary.
   The following dictionary entries are used:
     - TEST_CASES, a list of test scenario numbers.
         0 - torque_free_sphere
         1 - torque_free_symtop
         2 - torque_free_symtop_b
         3 - sho_sphere
         4 - orbit
         5 - spring_damper
     - TEST_NAME  - Specifies the test type by name.
     - LOG_DATA   - Boolean (0/1) indicating whether data should be logged.
     - OMEGA_DT   - Characteristic frequency * dynamic time step size
     - CASENO     - Random number index
     - NTESTS     - Number of tests to run
     - NREVS      - Number of revolutions (nominal) / steps (trace enabled)
     - DO_REPORT  - Boolean (0/1) indicating whether final report is desired
     - REPORT_ID  - Report file ID (1=stdout)
     - TRACE      - Boolean (0/1) indicating whether behavior trace is enabled.)
   """

   import sys
   import os

   # Add the JEOD python library and the Log_data directory to the search path
   sys.path.extend (['/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]),
                     '/'.join([os.getcwd(), "Log_data"])])

   # Now import the JEOD logger and the test-specific loggers.
   import jeod_log
   import log_run

   # There are six tests, four rotational tests and two translational tests.
   # NOTE WELL: The order here must agree with that in the run_cases.pl script.
   test_names = [
      "torque_free_sphere",
      "torque_free_symtop",
      "torque_free_symtop_b",
      "sho_sphere",
      "orbit",
      "spring_damper" ]

   # The test_objs maps the test names to test objects.
   test_objs = {
      "torque_free_sphere"   : test.torque_free_sphere,
      "torque_free_symtop"   : test.torque_free_symtop,
      "torque_free_symtop_b" : test.torque_free_symtop_b,
      "sho_sphere"           : test.sho_sphere,
      "orbit"                : test.orbit,
      "spring_damper"        : test.spring_damper }

   # The dynamics rate in the S_define is 1.
   delta_t = 1.0

   # Translate the numeric list options["TEST_CASES"] to a list of names.
   run_tests = []
   for ii in range (0, 6) :
      if ii in options["TEST_CASES"] :
         run_tests.append (test_names[ii])
   if not run_tests :
      raise RuntimeError ("No tests specified")

   # Set default values for regression tests (those in SET_test/RUN_xxx)
   if ("REGRESSION" not in options) or (options["REGRESSION"] == 1) :
      options["REGRESSION"] = 1
      options["OMEGA_DT"] = 1
      options["CASENO"] = 0
      options["NTESTS"] = 1
      options["NREVS"] = 0
      options["DO_REPORT"] = 0
      options["REPORT_ID"] = 1
      options["TRACE"] = 0

   # Final report, logging and behavior trace are off by default.
   if "DO_REPORT" not in options :
      options["DO_REPORT"] = 0
   if "LOG_DATA" not in options :
      options["LOG_DATA"] = 0
   if "TRACE" not in options :
      options["TRACE"] = 0

   # Compute the stop time
   # Nominal case (trace is off):
   # Stop time is the specified number of orbits or 80000 secs
   if options["TRACE"] == 0 :
      if options["NREVS"] == 0 :
         tstop = 80000
      else :
         tstop = 360.0 * options["NREVS"] * delta_t / options["OMEGA_DT"]

   # Behavior trace is on:
   # For NREVS = 0, just stop immediately at t=0.
   # This lets us trace allocations and initializations.
   # For NREVS > 0, use 2 NREVS seconds with a time reversal at NREVS seconds.
   else :
      if options["NREVS"] == 0 :
         tstop = 0
      else :
         tstop = 2 * options["NREVS"]
         trick.add_read (options["NREVS"],
            """
            time.time_manager.dyn_time.scale_factor =
               -time.time_manager.dyn_time.sacle_factor
            """)

   # Set the default log cycle if not specified so we get 401 log entries.
   if "LOG_CYCLE" not in options :
      options["LOG_CYCLE"] = tstop / 400


   # Set up logging if enabled.
   if "LOG_DATA" in options :
      # Create and initialize the logger.
      logger = jeod_log.Logger ()

      # Add items for the active tests to the data recording group.
      log_run.log_data (logger, options["LOG_CYCLE"], run_tests)


   # Copy numeric options to the test manager.
   test.test_manager.regression_test = options["REGRESSION"]
   test.test_manager.base_case       = options["CASENO"]
   test.test_manager.num_tests       = options["NTESTS"]
   test.test_manager.generate_report = options["DO_REPORT"]
   test.test_manager.report_fid      = options["REPORT_ID"]
   test.test_manager.debug           = options["TRACE"]
   test.test_manager.end_time        = tstop

   # Specify the integrator where the integ_option won't do.
   ###   if options["INTEG_OPT"] == 10 :
   ###      test.test_manager.set_integ_constructor (test.beeman)
   ###   elif options["INTEG_OPT"] == 11 :
   ###      test.test_manager.set_integ_constructor (test.midpoint)
   ###   elif options["INTEG_OPT"] == 12 :
   ###      test.test_manager.set_integ_constructor (test.gauss_jackson)

   # Add each active test to the test manager's list of such and
   # Set the omega_dt value (in degrees) for the test.
   wdt = options["OMEGA_DT"]
   for test_no, test_name in enumerate (run_tests) :
      test_object = test_objs[test_name]
      test_object.omega_dt = trick.attach_units ("degree", wdt)
      test.test_manager.set_template_item (test_no, test_object)

   # Make the spring-mass-damper very slightly damped.
   test.spring_damper.lambda_value = 0.01

   # Enable all messages.
   jeod_sys.interface_init.message_suppression_level = 9999

   # Finally, set the simulation end time.
   trick.exec_set_terminate_time (tstop)
