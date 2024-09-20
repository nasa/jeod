################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T46_OE_VER/
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
exec(compile(open( "Modified_data/default_mods_multi.py", "rb").read(), "Modified_data/default_mods_multi.py", 'exec'))
set_default_mods_multi(orb_elem_test)

exec(compile(open( "Modified_data/orb_par_transh_in.py", "rb").read(), "Modified_data/orb_par_transh_in.py", 'exec'))
set_orb_par_transh_in(orb_elem_test)

orb_elem_test.orb_elem_multi_ver.multi_use = True


trick.sim_services.exec_set_terminate_time(5000)
