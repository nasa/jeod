## Long test name: RUN_PAD_39A_full_NedState_ned_tstruct
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

options["TARGET_NAME"] = "PAD_39A"
options["TARGET_FULL"] = "full_NedState_ned_struct"
options["TARGET_ELLIPTICAL_NED"] = "TARGET_ELLIPTICAL_NED"

exec(compile(open( "Modified_data/single_vehicle_run.py", "rb").read(), "Modified_data/single_vehicle_run.py", 'exec'))
set_single_vehicle_run(jeod_sys, earth, target, options)
