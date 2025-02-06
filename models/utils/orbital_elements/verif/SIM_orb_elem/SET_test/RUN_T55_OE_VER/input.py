################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T55_OE_VER/ 
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

orb_elem_test.orb_elem_ver.position  = [ 5657077.34, 3080301.15, 1918782.98]
orb_elem_test.orb_elem_ver.velocity  = [ -3868.050682, 3564.535653, 5633.830366]

read =1
trick.add_read(read, """

orb_elem_test.orb_elem.semiparam     = 6.732527344719575e+06 
orb_elem_test.orb_elem.e_mag         = .0024649 
orb_elem_test.orb_elem.inclination   = 0.90300001919315
orb_elem_test.orb_elem.mean_anom     = 5.48133793918087
orb_elem_test.orb_elem.arg_periapsis = 1.17751936658084
orb_elem_test.orb_elem.long_asc_node = 0.26146236472059
orb_elem_test.orb_elem_ver.to_cartesian = 1 
""")


trick.sim_services.exec_set_terminate_time(2)
