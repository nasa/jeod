################################################################################
#
# Purpose:
#   (Regression test run against all five test scenarios using
#    the ABM4 integrator.)
#
#
################################################################################


# Load the common input handler
exec(compile(open("Modified_data/run_common.py", "rb").read(), "Modified_data/run_common.py", 'exec'))

trick.sim_services.exec_set_trap_sigfpe(1)

# Set up the run options.
options = {
  "TEST_CASES":list(range(0,5)),       # Use all five test scenarios
  "LOG_DATA":1}                        # Use Trick data logging

test.switch_integ_to_abm4()

# Use the common handler to set up the run.
set_run_common (options)
