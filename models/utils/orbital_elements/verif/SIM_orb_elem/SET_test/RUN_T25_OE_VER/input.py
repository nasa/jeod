################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T25_OE_VER/ 
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

orb_elem_test.orb_elem.semiparam     = 7378145 
orb_elem_test.orb_elem.e_mag         = 3.56898414435138e-17 
orb_elem_test.orb_elem.inclination   = 1.570796326794897
orb_elem_test.orb_elem.mean_anom     = 0.0
orb_elem_test.orb_elem.arg_periapsis = 0.0 
orb_elem_test.orb_elem.long_asc_node = 0.0 
orb_elem_test.orb_elem_ver.to_cartesian = 1 


trick.sim_services.exec_set_terminate_time(1)
