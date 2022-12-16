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

# Set up parameters to record
exec(compile(open("Log_data/log_verif_rec.py", "rb").read(), "Log_data/log_verif_rec.py", 'exec'))
exec(compile(open("Modified_data/mass.py", "rb").read(), "Modified_data/mass.py", 'exec'))

articulation.offset_b = [0,0,0]
articulation.offset_c = [0,0,0]
articulation.offset_d = [0,0,0]

articulation.T_pstr_cstr_b = [[1.0, 0.0, 0.0],[0.0, 1.0, 0.0],[0.0,0.0,1.0]]
articulation.T_pstr_cstr_c = [[1.0, 0.0, 0.0],[0.0, 1.0, 0.0],[0.0,0.0,1.0]]
articulation.T_pstr_cstr_d = [[1.0, 0.0, 0.0],[0.0, 1.0, 0.0],[0.0,0.0,1.0]]

articulation.flat_plate_1.local_position = [0.0, 0.0, 0.0]
articulation.flat_plate_1.area = 1.0
articulation.flat_plate_1.local_normal = [1.0, 0.0, 0.0]
articulation.flat_plate_1.mass_body_name = "mass_bodya"

articulation.flat_plate_2.local_position = [0.0, 0.0, 0.0]
articulation.flat_plate_2.area = 1.0
articulation.flat_plate_2.local_normal =  [-1.0, 0.0, 0.0]
articulation.flat_plate_2.mass_body_name = "mass_bodya"

articulation.flat_plate_3.local_position = [0.0, 0.0, 0.0]
articulation.flat_plate_3.area = 1.0
articulation.flat_plate_3.local_normal = [1.0, 0.0, 0.0]
articulation.flat_plate_3.mass_body_name = "mass_bodyb"

articulation.flat_plate_4.local_position = [0.0, 0.0, 0.0]
articulation.flat_plate_4.area = 1.0
articulation.flat_plate_4.local_normal =  [-1.0, 0.0, 0.0]
articulation.flat_plate_4.mass_body_name = "mass_bodyb"

articulation.flat_plate_5.local_position = [0.0, 0.0, 0.0]
articulation.flat_plate_5.area = 1.0
articulation.flat_plate_5.local_normal = [1.0, 0.0, 0.0]
articulation.flat_plate_5.mass_body_name = "mass_bodyc"

articulation.flat_plate_6.local_position = [0.0, 0.0, 0.0]
articulation.flat_plate_6.area = 1.0
articulation.flat_plate_6.local_normal = [-1.0, 0.0, 0.0]
articulation.flat_plate_6.mass_body_name = "mass_bodyc"

articulation.flat_plate_7.local_position = [0.0, 0.0, 0.0]
articulation.flat_plate_7.area = 1.0
articulation.flat_plate_7.local_normal = [1.0, 0.0, 0.0]
articulation.flat_plate_7.mass_body_name = "mass_bodyd"

articulation.flat_plate_8.local_position = [0.0, 0.0, 0.0]
articulation.flat_plate_8.area = 1.0
articulation.flat_plate_8.local_normal =  [-1.0, 0.0, 0.0]
articulation.flat_plate_8.mass_body_name = "mass_bodyd"


articulation.surface.struct_body_name = "mass_bodya"
articulation.surface.articulation_active = True
articulation.rotate_in_time = True

trick.stop(360.0)
