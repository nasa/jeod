################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T52_OE_VER/ 
####################################################################################

#Auto-generated from the data file with this header:
#/*
#   file purpose: Input file used for ATV TGM model verification unit test
#   test:         Test 13
#   tester:       Scott Piggott
#*/


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################

#############  LOG DATA  #################

#############  ASSIGNMENTS, CALLS, and CONTROL  #################

#/*================================================*/
#/* Load default data files specified in S_define. */
#/*================================================*/
exec(compile(open( "Modified_data/default_mods.py", "rb").read(), "Modified_data/default_mods.py", 'exec'))
set_default_mods(orb_elem_test)

orb_elem_test.orb_elem_ver.mu = 3.986006401119749e+14

orb_elem_test.orb_elem_ver.position  = [ 4671696.48, -1482158.85, -4620191.58]
orb_elem_test.orb_elem_ver.velocity  = [ 4617.404657, 5390.140889, 2952.194537]

read =1
trick.add_read(read, """

orb_elem_test.orb_elem.semiparam     = 6.725408246468065e+06 
orb_elem_test.orb_elem.e_mag         = 0.0018722 
orb_elem_test.orb_elem.inclination   = 0.90392993061862
orb_elem_test.orb_elem.mean_anom     = 3.77944644161017
orb_elem_test.orb_elem.arg_periapsis = 1.44472543533681
orb_elem_test.orb_elem.long_asc_node = 0.52888659963307
orb_elem_test.orb_elem_ver.to_cartesian = 1 
""")


trick.sim_services.exec_set_terminate_time(2)
