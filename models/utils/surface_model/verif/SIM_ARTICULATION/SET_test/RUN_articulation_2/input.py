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

articulation.offset_b = [0.0978881,    1.65429,   -2.12155]
articulation.offset_c = [-1.88698,    1.08009,   -3.95204]
articulation.offset_d = [ 4.42141,    2.94013,  -0.773762]

articulation.T_pstr_cstr_b = [
[0.1908098464799607, -0.6966135920642235, -0.6916076241899547],
[0.04030743089433808, 0.7095182653790713, -0.703533326934959],
[0.9807991198512527, 0.1063641595879615, 0.1634617755138939]]

articulation.T_pstr_cstr_c = [
[-0.7361133641239929, -0.5508618941680473, -0.3933043207385965],
[ -0.4364024092758288, -0.05791070693348288, 0.897886010137543],
[-0.517387719545323, 0.8325848446711333, -0.197768612493578]]

articulation.T_pstr_cstr_d = [
[0.8222190420975497, -0.3626722656735943, 0.4386623696231186],
[0.04304325740965631, 0.808117347462562, 0.5874467045797761],
[-0.5675412978839106, -0.4641284094313281, 0.6800600670198124]]

articulation.flat_plate_1.local_position = [3.39326,    2.47589,   0.948552]
articulation.flat_plate_1.area = 1.0;
articulation.flat_plate_1.local_normal = [ 0.768477,   0.607306,  -0.201548]
articulation.flat_plate_1.mass_body_name = "mass_bodya";

articulation.flat_plate_2.local_position = [ -4.94456, -0.00261188,   -4.32719]
articulation.flat_plate_2.area = 1.0;
articulation.flat_plate_2.local_normal = [0.741323,  -0.585104,  -0.328777]
articulation.flat_plate_2.mass_body_name = "mass_bodya";

articulation.flat_plate_3.local_position = [ -3.6932,    2.92994,   -1.23367]
articulation.flat_plate_3.area = 1.0;
articulation.flat_plate_3.local_normal = [0.338868,  -0.512566,   0.788952]
articulation.flat_plate_3.mass_body_name = "mass_bodyb";

articulation.flat_plate_4.local_position = [ -2.90045,  0.0849147,  -0.604298]
articulation.flat_plate_4.area = 1.0;
articulation.flat_plate_4.local_normal = [ 0.79904,   0.326973,   0.504603]
articulation.flat_plate_4.mass_body_name = "mass_bodyb";

articulation.flat_plate_5.local_position = [ 2.44785,   0.962552,   0.615419]
articulation.flat_plate_5.area = 1.0;
articulation.flat_plate_5.local_normal = [-0.628147,   0.721774,  -0.290642]
articulation.flat_plate_5.mass_body_name = "mass_bodyc";

articulation.flat_plate_6.local_position = [-2.22592,  -0.619661,    2.45179]
articulation.flat_plate_6.area = 1.0;
articulation.flat_plate_6.local_normal = [0.772102,   0.554193,   0.311013]
articulation.flat_plate_6.mass_body_name = "mass_bodyc";

articulation.flat_plate_7.local_position = [-0.528547,  -0.710206,   0.832302]
articulation.flat_plate_7.area = 1.0;
articulation.flat_plate_7.local_normal = [  0.360979,    0.56045,   0.745379]
articulation.flat_plate_7.mass_body_name = "mass_bodyd";

articulation.flat_plate_8.local_position = [4.29469,    3.92779,    2.56689]
articulation.flat_plate_8.area = 1.0;
articulation.flat_plate_8.local_normal = [-0.0567124,  -0.673532,  -0.736979]
articulation.flat_plate_8.mass_body_name = "mass_bodyd";

articulation.surface.struct_body_name = "mass_bodya"
articulation.surface.articulation_active = True

trick.exec_set_terminate_time(10.0)
