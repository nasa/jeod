## Long test name: RUN_STS_114_trans_NedTransState_tned_cstruct_rot_NedRotState_tned_cstruct
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

options["CHASER_NAME"] = "STS_114"
options["CHASER_TRANS"] = "trans_NedTransState_tned_struct_pad_39a"
options["CHASER_ROT"] = "rot_NedRotState_tned_struct_pad_39a"

exec(compile(open( "Modified_data/double_vehicle_run.py", "rb").read(), "Modified_data/double_vehicle_run.py", 'exec'))
set_double_vehicle_run(jeod_sys, earth, target, options)
