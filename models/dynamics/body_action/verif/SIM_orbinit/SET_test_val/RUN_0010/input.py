## Long test name: RUN_ISS_trans_Orbit_inertial_tbody_set10
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
options["TARGET_TRANS"] = "trans_Orbit_inertial_body_set10"
options["TARGET_REF"] = "inertial"

exec(compile(open( "Modified_data/single_vehicle_run.py", "rb").read(), "Modified_data/single_vehicle_run.py", 'exec'))
set_single_vehicle_run(jeod_sys, earth, target, options)
