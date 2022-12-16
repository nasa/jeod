# This is an input file for the surface model verification
#
# Andrew Spencer
# August, 2010
#
#
# NOTE: THE DOCUMENTATION REFERS TO MASS BODY C AS A DIRECT
# CHILD OF MASS BODY A. IN THE CODE, THIS IS ACTUALLY MASS BODY
# D. MASS BODY C ITSELF IS NEVER USED IN THIS PARTICULAR
# DEMONSTRATION. MASS BODY D IN CODE = MASS BODY C IN DOCUMENTATION

# Bring in the functions that we wrote to set up the logging
# for this run.
exec(compile(open("Log_data/log_verif_rec.py", "rb").read(), "Log_data/log_verif_rec.py", 'exec'))
exec(compile(open("Modified_data/mass.py", "rb").read(), "Modified_data/mass.py", 'exec'))

articulation.offset_b = [-2.0381,   -4.09991,    3.59179]
articulation.offset_c = [ 1.40503,  -2.20043,   -2.63571]
articulation.offset_d = [ 3.4757,   -0.339964,  -1.27395]

articulation.T_pstr_cstr_b = [
[0.5738961762045672, -0.6152633149134586, 0.5404574287207816],
[0.6713942538549126, 0.7313744132614041, 0.119671314512468],
[-0.468906104490421, 0.2941811022962542, 0.8328172333852003]]

articulation.T_pstr_cstr_c = [
[0.1200730155375398, 0.1674994268654244, 0.9785327858275759],
[-0.9674416511940138, -0.2014627819906493, 0.1531972552221103],
[0.2227983897489432, -0.9650682304751009, 0.137855678348538]]

articulation.T_pstr_cstr_d = [
[0.8775360833103077, 0.2816997479818719, 0.3880408154760973],
[-0.3435153756221815, 0.9339299286459263, 0.0988538066569347],
[-0.3345558386869913, -0.2200457687990501, 0.9163254064108879]]

articulation.flat_plate_1.local_position = [0.00575529,   -2.31225,    2.12175]
articulation.flat_plate_1.area = 1.0;
articulation.flat_plate_1.local_normal = [-0.760258,   0.282587,  -0.584938]
articulation.flat_plate_1.mass_body_name = "mass_bodya";

articulation.flat_plate_2.local_position = [-1.70562,    3.03695,  -0.958726]
articulation.flat_plate_2.area = 1.0;
articulation.flat_plate_2.local_normal = [ 0.69449,  -0.396341,   0.600498]
articulation.flat_plate_2.mass_body_name = "mass_bodya";

articulation.flat_plate_3.local_position = [-2.73297,   -2.12864,   -3.22221]
articulation.flat_plate_3.area = 1.0;
articulation.flat_plate_3.local_normal = [-0.273795,   0.820658,   0.501555]
articulation.flat_plate_3.mass_body_name = "mass_bodyb";

articulation.flat_plate_4.local_position = [ 2.29363,   -3.30972,    3.85143]
articulation.flat_plate_4.area = 1.0;
articulation.flat_plate_4.local_normal = [ 0.046571,   -0.84212,   0.537275]
articulation.flat_plate_4.mass_body_name = "mass_bodyb";

articulation.flat_plate_5.local_position = [ -1.3239,   -1.59037,   0.596073]
articulation.flat_plate_5.area = 1.0;
articulation.flat_plate_5.local_normal = [ 0.2905,    0.71079,   0.640615]
articulation.flat_plate_5.mass_body_name = "mass_bodyc";

articulation.flat_plate_6.local_position = [ 1.09338,   0.528991,   0.548487]
articulation.flat_plate_6.area = 1.0;
articulation.flat_plate_6.local_normal = [-0.0100222,  -0.542243,   0.840162]
articulation.flat_plate_6.mass_body_name = "mass_bodyc";

articulation.flat_plate_7.local_position = [-0.694371,   -3.48133,    -0.4691]
articulation.flat_plate_7.area = 1.0;
articulation.flat_plate_7.local_normal = [ 0.959905,   0.192204,    0.20406]
articulation.flat_plate_7.mass_body_name = "mass_bodyd";

articulation.flat_plate_8.local_position = [ -2.93046,    1.31073,    0.22227]
articulation.flat_plate_8.area = 1.0;
articulation.flat_plate_8.local_normal = [ 0.401064,   0.648818,   -0.64667]
articulation.flat_plate_8.mass_body_name = "mass_bodyd";

articulation.surface.struct_body_name = "mass_bodya"
articulation.surface.articulation_active = True

trick.exec_set_terminate_time(10.0)
