################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T56_OE_VER/
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

orb_elem_test.orb_elem_ver.mu = 3.986004415e+14

orb_elem_test.orb_elem_ver.position  = [ 6524.834e+03, 6862.875e+03, 6448.296e+03]
orb_elem_test.orb_elem_ver.velocity  = [ 4.901327e+03, 5.533756e+03, -1.976341e+03]

read =1
trick.add_read(read, """

orb_elem_test.orb_elem.semiparam     = 11067.790e+03
orb_elem_test.orb_elem.e_mag         = .83285
orb_elem_test.orb_elem.inclination   = 1.53362081372742
orb_elem_test.orb_elem.mean_anom     = 0.1327277817291182
orb_elem_test.orb_elem.arg_periapsis = 0.93165675471457
orb_elem_test.orb_elem.long_asc_node = 3.97743083236988
orb_elem_test.orb_elem_ver.to_cartesian = True
""")


trick.sim_services.exec_set_terminate_time(2)
