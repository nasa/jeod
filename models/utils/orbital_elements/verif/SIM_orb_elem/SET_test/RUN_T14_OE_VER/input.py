################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T14_OE_VER/ 
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

orb_elem_test.orb_elem_ver.position  = [ 5.21713636207762e+06, 5.21713636207762e+06, 0.0 ]
orb_elem_test.orb_elem_ver.velocity  = [ -4.02583837453453e+03,4.02583837453453e+03 , 5.69339522918879e+03 ]

read = 1
trick.add_read(read, """
orb_elem_test.orb_elem_ver.position  = [ -7.82570454311643e+06, -7.82570454311643e+06, 0.0 ]
orb_elem_test.orb_elem_ver.velocity  = [ 2.68389224968969e+03, -2.68389224968969e+03, -3.79559681945919e+03 ]
""")


trick.sim_services.exec_set_terminate_time(2)
