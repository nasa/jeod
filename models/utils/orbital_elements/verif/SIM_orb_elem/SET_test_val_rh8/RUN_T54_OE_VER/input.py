################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T54_OE_VER/
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

orb_elem_test.orb_elem_ver.position  = [ -3884577.77, 2621655.56, 4811943.86]
orb_elem_test.orb_elem_ver.velocity  = [ -5941.483887, -4243.634662, -2469.292325]

read =1
trick.add_read(read, """

orb_elem_test.orb_elem.semiparam     = 6.724305769002885e+06
orb_elem_test.orb_elem.e_mag         = .0017794
orb_elem_test.orb_elem.inclination   = 0.90296965046417
orb_elem_test.orb_elem.mean_anom     = 0.90826742287567
orb_elem_test.orb_elem.arg_periapsis = 1.08141665104412
orb_elem_test.orb_elem.long_asc_node = 0.35020537586749
orb_elem_test.orb_elem_ver.to_cartesian = True
""")


trick.sim_services.exec_set_terminate_time(2)
