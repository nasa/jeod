################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T57_OE_VER/ 
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

orb_elem_test.orb_elem_ver.mu = 1.0

orb_elem_test.orb_elem_ver.position  = [ 1.29903810567666, .75, 0]
orb_elem_test.orb_elem_ver.velocity  = [ -0.35355339059327, 0.61237243569579, 0.70710678118655 ]

read =1
trick.add_read(read, """

orb_elem_test.orb_elem.semiparam     = 2.25 
orb_elem_test.orb_elem.e_mag         = .5 
orb_elem_test.orb_elem.inclination   = 0.78539816339745
orb_elem_test.orb_elem.mean_anom     = 0.0
orb_elem_test.orb_elem.arg_periapsis = 0.0
orb_elem_test.orb_elem.long_asc_node = 0.52359877559830
orb_elem_test.orb_elem_ver.to_cartesian = 1 
""")


trick.sim_services.exec_set_terminate_time(2)
