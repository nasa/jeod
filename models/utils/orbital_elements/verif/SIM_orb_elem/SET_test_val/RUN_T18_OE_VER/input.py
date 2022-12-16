################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T18_OE_VER/ 
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

orb_elem_test.orb_elem_ver.position  = [ -5.217136362077621e+06, -5.217136362077621e+06, 0.0 ]
orb_elem_test.orb_elem_ver.velocity  = [ -4.025838374534530e+03,4.025838374534531e+03 , -5.693395229188786e+03]

read = 1
trick.add_read(read, """
orb_elem_test.orb_elem_ver.position  = [ 7.825704543116432e+06, 7.825704543116429e+06, 0.0 ]
orb_elem_test.orb_elem_ver.velocity  = [ 2.683892249689686e+03, -2.683892249689687e+03, 3.795596819459191e+03 ]
""")


trick.sim_services.exec_set_terminate_time(2)
