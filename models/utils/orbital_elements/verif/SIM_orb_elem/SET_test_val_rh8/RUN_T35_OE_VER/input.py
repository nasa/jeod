################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T35_OE_VER/
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

orb_elem_test.orb_elem.semiparam     = 8853774
orb_elem_test.orb_elem.e_mag         = 0.2000000000000001
orb_elem_test.orb_elem.inclination   = 1.570796326794897
orb_elem_test.orb_elem.mean_anom     = 6.283185307179586
orb_elem_test.orb_elem.arg_periapsis = 2.249640726028908e-32
orb_elem_test.orb_elem.long_asc_node = 1.570796326794897
orb_elem_test.orb_elem_ver.to_cartesian = True

read = 1
trick.add_read(read, """
orb_elem_test.orb_elem.semiparam     = 8853774
orb_elem_test.orb_elem.e_mag         = 0.2000000000000001
orb_elem_test.orb_elem.inclination   = 1.570796326794897
orb_elem_test.orb_elem.mean_anom     = 3.141592653589793
orb_elem_test.orb_elem.arg_periapsis = 6.283185307179586
orb_elem_test.orb_elem.long_asc_node = 1.570796326794897
orb_elem_test.orb_elem_ver.to_cartesian = True
""")

trick.sim_services.exec_set_terminate_time(2)
