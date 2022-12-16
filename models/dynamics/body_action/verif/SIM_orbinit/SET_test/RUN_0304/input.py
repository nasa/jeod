## Long test name: RUN_STS_114_trans_Orbit_pfix_tbody_set04
################################################################################
#
# Purpose:
#   Test translation state initialization with options
#   test = trans_init,reference frame = pfix.
#
#

#
# Date:
#   June, 2006
#
################################################################################


trick.sim_services.exec_set_trap_sigfpe(1)

options = {}

options["TARGET_NAME"] = "STS_114"
options["TARGET_TRANS"] = "trans_Orbit_pfix_body_set04"
options["TARGET_REF"] = "pfix"

exec(compile(open( "Modified_data/single_vehicle_run.py", "rb").read(), "Modified_data/single_vehicle_run.py", 'exec'))
set_single_vehicle_run(jeod_sys, earth, target, options)
