################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T17_OE_VER/ 
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

orb_elem_test.orb_elem_ver.position  = [ -7378145, 0.0, 0.0 ]
orb_elem_test.orb_elem_ver.velocity  = [ 0.0,8.051676749069058e+03 , 0.0]

read = 1
trick.add_read(read, """
orb_elem_test.orb_elem_ver.position  = [ 1.106721750000000e+07, -1.355343248684257e-09, 0.0 ]
orb_elem_test.orb_elem_ver.velocity  = [ 0.0, -5.367784499379371e+03, 0.0 ]
""")


trick.sim_services.exec_set_terminate_time(2)
