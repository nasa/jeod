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

articulation.offset_b = [-0.126141,    4.21103,    2.79516]
articulation.offset_c = [ 2.69991,    3.53459,    4.99746]
articulation.offset_d = [-1.67924,  -0.878345,    1.04473]

articulation.T_pstr_cstr_b = [
[0.997369704807009, 0.06997143703733257, 0.01891216360204106],
[-0.07007880132551818, 0.9975285522158546, 0.005074358966393988],
[-0.01851036298829154, -0.006386353660100277, 0.99980827209469]]

articulation.T_pstr_cstr_c = [
[-0.07082185013447745, 0.9972707676557767, 0.02086340151527186],
[0.9728231355161533, 0.06443288212731174, 0.2224040258297676],
[0.2204527444783792, 0.03604746426623952, -0.9747313310712469]]

articulation.T_pstr_cstr_d = [
[0.2629583935566351, 0.4849501785352546, -0.8340720637910962],
[-0.2381525060753064, -0.8051199041282633, -0.5431991566879935],
[-0.9349525480515922, 0.3414751298632461, -0.09622093627020228]]

articulation.flat_plate_1.local_position = [-1.49968,   0.670615,    1.06301]
articulation.flat_plate_1.area = 1.0
articulation.flat_plate_1.local_normal = [-0.496347,  -0.260678,  -0.828062]
articulation.flat_plate_1.mass_body_name = "mass_bodya"

articulation.flat_plate_2.local_position = [2.28029,   0.508347,    4.97695]
articulation.flat_plate_2.area = 1.0
articulation.flat_plate_2.local_normal = [ 0.12748,   0.820292,  -0.557557]
articulation.flat_plate_2.mass_body_name = "mass_bodya"

articulation.flat_plate_3.local_position = [ 3.8862,   -4.52567,    2.66886]
articulation.flat_plate_3.area = 1.0
articulation.flat_plate_3.local_normal = [ -0.247273,   -0.23111,  -0.940981]
articulation.flat_plate_3.mass_body_name = "mass_bodyb"

articulation.flat_plate_4.local_position = [3.64855,   -3.53627,   -3.90689]
articulation.flat_plate_4.area = 1.0
articulation.flat_plate_4.local_normal = [ -0.138757,   0.861377,  -0.488648]
articulation.flat_plate_4.mass_body_name = "mass_bodyb"

articulation.flat_plate_5.local_position = [-0.639442,    -2.7477,   -2.42615]
articulation.flat_plate_5.area = 1.0
articulation.flat_plate_5.local_normal = [0.81173,  -0.130319,   0.569308]
articulation.flat_plate_5.mass_body_name = "mass_bodyc"

articulation.flat_plate_6.local_position = [-3.61362,   0.551767,  -0.587464]
articulation.flat_plate_6.area = 1.0
articulation.flat_plate_6.local_normal = [ 0.798565,  -0.347707,  -0.491318]
articulation.flat_plate_6.mass_body_name = "mass_bodyc"

articulation.flat_plate_7.local_position = [-0.431173,   -1.83218,   -1.84599]
articulation.flat_plate_7.area = 1.0
articulation.flat_plate_7.local_normal = [-0.706014,  -0.123783,   0.697296]
articulation.flat_plate_7.mass_body_name = "mass_bodyd"

articulation.flat_plate_8.local_position = [1.14477,    -3.2097,     2.4224]
articulation.flat_plate_8.area = 1.0
articulation.flat_plate_8.local_normal = [  0.31147,  -0.909147,  -0.276475]
articulation.flat_plate_8.mass_body_name = "mass_bodyd"

articulation.surface.struct_body_name = "mass_bodya"
articulation.surface.articulation_active = True

trick.exec_set_terminate_time(10.0)

