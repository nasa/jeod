################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T53_OE_VER/ 
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

orb_elem_test.orb_elem_ver.position  = [ -4640789.30, -4261640.89, -2384608.32]
orb_elem_test.orb_elem_ver.velocity  = [ 4937.466049, -2370.502176, -5396.909174]

read =1
trick.add_read(read, """

orb_elem_test.orb_elem.semiparam     = 6.731933986532722e+06 
orb_elem_test.orb_elem.e_mag         = .0013342 
orb_elem_test.orb_elem.inclination   = 0.90387233475330
orb_elem_test.orb_elem.mean_anom     = 2.14449390590707
orb_elem_test.orb_elem.arg_periapsis = 1.46217855332383 
orb_elem_test.orb_elem.long_asc_node = 0.44028688001775
orb_elem_test.orb_elem_ver.to_cartesian = 1 
""")


trick.sim_services.exec_set_terminate_time(2)
