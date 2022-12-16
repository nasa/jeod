## Long test name: RUN_STS_114_trans_NedTransState_tned_cbody
################################################################################
#
# Purpose:
#   Test translation state initialization with options
#   test = trans_init,reference frame = inertial.
#
#

#
# Date:
#   June, 2006
#
################################################################################


trick.sim_services.exec_set_trap_sigfpe(1)

options = {}

options["TARGET_NAME"] = "ISS"
options["CHASER_NAME"] = "STS_114"
options["CHASER_TRANS"] = "trans_NedTransState_tned_body"

exec(compile(open( "Modified_data/double_vehicle_run.py", "rb").read(), "Modified_data/double_vehicle_run.py", 'exec'))
set_double_vehicle_run(jeod_sys, earth, target, options)
