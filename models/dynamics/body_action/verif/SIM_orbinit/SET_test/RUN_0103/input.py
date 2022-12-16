## Long test name: RUN_STS_114_trans_Orbit_inertial_tbody_set03
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

options["TARGET_NAME"] = "STS_114"
options["TARGET_TRANS"] = "trans_Orbit_inertial_body_set03"
options["TARGET_REF"] = "inertial"

exec(compile(open( "Modified_data/single_vehicle_run.py", "rb").read(), "Modified_data/single_vehicle_run.py", 'exec'))
set_single_vehicle_run(jeod_sys, earth, target, options)
