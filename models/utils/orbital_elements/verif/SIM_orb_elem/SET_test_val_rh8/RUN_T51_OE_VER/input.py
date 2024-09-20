################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_T51_OE_VER/
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
orb_elem_test.orb_elem.e_mag         = 0.0
orb_elem_test.orb_elem.inclination   = 3.14159265358979
orb_elem_test.orb_elem.mean_anom     = 4.71238898038469
orb_elem_test.orb_elem.arg_periapsis = 0.0
orb_elem_test.orb_elem.long_asc_node = 0.0
orb_elem_test.orb_elem_ver.to_cartesian = True

read = 1
trick.add_read(read, """
orb_elem_test.orb_elem_ver.to_cartesian = False
""")

read = 2
trick.add_read(read, """
orb_elem_test.orb_elem.semiparam     = 7378145
orb_elem_test.orb_elem.e_mag         = .001
orb_elem_test.orb_elem.inclination   = 0.0
orb_elem_test.orb_elem.mean_anom     = 0.78539816339745
orb_elem_test.orb_elem.arg_periapsis = 0.0
orb_elem_test.orb_elem.long_asc_node = 0.0
orb_elem_test.orb_elem_ver.to_cartesian = True
""")

read = 3
trick.add_read(read, """
orb_elem_test.orb_elem_ver.to_cartesian = False
""")

read =4
trick.add_read(read, """
orb_elem_test.orb_elem.semiparam     = 7378145
orb_elem_test.orb_elem.e_mag         = .8
orb_elem_test.orb_elem.inclination   = 0.0
orb_elem_test.orb_elem.mean_anom     = 3.66519142918809
orb_elem_test.orb_elem.arg_periapsis = 0.0
orb_elem_test.orb_elem.long_asc_node = 0.0
orb_elem_test.orb_elem_ver.to_cartesian = True
""")

read = 5
trick.add_read(read, """
orb_elem_test.orb_elem_ver.to_cartesian = False
""")

read = 6
trick.add_read(read, """
orb_elem_test.orb_elem.semiparam     = 7378145
orb_elem_test.orb_elem.e_mag         = 0.0
orb_elem_test.orb_elem.inclination   = 0.78539816339745
orb_elem_test.orb_elem.mean_anom     = 0.0
orb_elem_test.orb_elem.arg_periapsis = 0.0
orb_elem_test.orb_elem.long_asc_node = 3.66519142918809
orb_elem_test.orb_elem_ver.to_cartesian = True
""")

read = 7
trick.add_read(read, """
orb_elem_test.orb_elem_ver.to_cartesian = False
""")

read = 8
trick.add_read(read, """
orb_elem_test.orb_elem.semiparam     = 7378145
orb_elem_test.orb_elem.e_mag         = 0.01
orb_elem_test.orb_elem.inclination   = 0.78539816339745
orb_elem_test.orb_elem.mean_anom     = 0.0
orb_elem_test.orb_elem.arg_periapsis = 0.0
orb_elem_test.orb_elem.long_asc_node = 3.66519142918809
orb_elem_test.orb_elem_ver.to_cartesian = True
""")

read = 9
trick.add_read(read, """
orb_elem_test.orb_elem_ver.to_cartesian = False
""")

read = 10
trick.add_read(read, """
orb_elem_test.orb_elem.semiparam     = 7378145
orb_elem_test.orb_elem.e_mag         = 2.5
orb_elem_test.orb_elem.inclination   = 0.78539816339745
orb_elem_test.orb_elem.mean_anom     = -0.78539816339745
orb_elem_test.orb_elem.arg_periapsis = 0.0
orb_elem_test.orb_elem.long_asc_node = 0.0
orb_elem_test.orb_elem_ver.to_cartesian = True
""")

read = 11
trick.add_read(read, """
orb_elem_test.orb_elem_ver.to_cartesian = False
""")

read = 12
trick.add_read(read, """
orb_elem_test.orb_elem.semiparam     = 7378145
orb_elem_test.orb_elem.e_mag         = 2.5
orb_elem_test.orb_elem.inclination   = 0.78539816339745
orb_elem_test.orb_elem.mean_anom     = -3.66519142918809
orb_elem_test.orb_elem.arg_periapsis = 0.0
orb_elem_test.orb_elem.long_asc_node = 0.0
orb_elem_test.orb_elem_ver.to_cartesian = True
""")

read = 13
trick.add_read(read, """
orb_elem_test.orb_elem_ver.to_cartesian = False
""")

read = 14
trick.add_read(read, """
orb_elem_test.orb_elem.semiparam     = 7378145
orb_elem_test.orb_elem.e_mag         = 1
orb_elem_test.orb_elem.inclination   = 0.78539816339745
orb_elem_test.orb_elem.mean_anom     = -0.78539816339745
orb_elem_test.orb_elem.arg_periapsis = 0.0
orb_elem_test.orb_elem.long_asc_node = 0.0
orb_elem_test.orb_elem_ver.to_cartesian = True
""")

read = 15
trick.add_read(read, """
orb_elem_test.orb_elem_ver.to_cartesian = False
""")


trick.sim_services.exec_set_terminate_time(16)
