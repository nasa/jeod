################################################################################
#
# Purpose: Run the simulation using values provided by environment variables.
#
################################################################################

# Load the common input handler
exec(compile(open("Modified_data/run_common.py", "rb").read(), "Modified_data/run_common.py", 'exec'))
import re

test_ids = [
   int(os.getenv("RUN_TEST_ID_0")),
   int(os.getenv("RUN_TEST_ID_1")),
   int(os.getenv("RUN_TEST_ID_2")),
   int(os.getenv("RUN_TEST_ID_3")),
   int(os.getenv("RUN_TEST_ID_4")),
   int(os.getenv("RUN_TEST_ID_5")) ]

integ_opt = os.getenv("RUN_INTEG_OPT")
match = re.search("^GaussJackson(\d+)$", integ_opt)
if (match) :
   integ_opt = 120 + int(match.group(1))
else :
   integ_opt = eval("trick.Integration." + integ_opt)

# Set up the run options per the environment variables
options = { 
   "TEST_CASES" : test_ids,   # Collected above
   "LOG_DATA"   : int(os.getenv("RUN_LOG_DATA")),
   "INTEG_OPT"  : integ_opt,
   "OMEGA_DT"   : float(os.getenv("RUN_OMEGA_DT")),
   "CASENO"     : int(os.getenv("RUN_CASENO")),
   "NTESTS"     : int(os.getenv("RUN_NTESTS")),
   "NREVS"      : int(os.getenv("RUN_NREVS")),
   "DO_REPORT"  : int(os.getenv("RUN_DO_REPORT")),
   "REPORT_ID"  : int(os.getenv("RUN_REPORT_ID")),
   "TRACE"      : int(os.getenv("RUN_TRACE")),
   "REGRESSION" : 0 }

# This run uses old-style integration
test.test_manager.set_deprecated_rotation_integration()

# Use the common handler to set up the run.
set_run_common (data_record, jeod_sys, test, options)
