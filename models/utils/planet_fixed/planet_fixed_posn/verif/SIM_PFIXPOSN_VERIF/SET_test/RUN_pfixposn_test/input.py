################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_pfixposn_test/
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#   Purpose:   Input file used for JEOD PlanetFixedPosition model unit test
#
#   Test:      Verify all methods of PlanetFixedPosition work correctly
#
#*******************************************************************************/


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 0.10

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_pfixposn_verif.py", "rb").read(), "Log_data/log_pfixposn_verif.py", 'exec'))
log_pfixposn_verif(LOG_CYCLE)

#############  ASSIGNMENTS, CALLS, and CONTROL  #################

#// Set pointers between test planet and test grav_source
earth.planet.grav_source = earth.gravity_source


#// Test conversion from cartesian to spherical and elliptical coordinates
read = 1.0
trick.add_read(read, """
earth.cartesian_pos  = [ 6778.1363e3, 0.0, 0.0]
earth.update_from_cart ()
# earth.pfix_pos.update_from_cart (earth.cartesian_pos)
""")


#// Test conversion from spherical to cartesian and elliptical coordinates
read = 2.0
trick.add_read(read, """
earth.spherical_pos.altitude = 1000.0
earth.spherical_pos.latitude = 3.1416
earth.spherical_pos.longitude = 1.0
earth.pfix_pos.update_from_spher (earth.spherical_pos)
""")


#// Test conversion from elliptical to cartesian and spherical coordinates
read = 3.0
trick.add_read(read, """
earth.elliptical_pos.altitude = 500.0
earth.elliptical_pos.latitude = 1.0
earth.elliptical_pos.longitude = 3.1416
earth.pfix_pos.update_from_ellip (earth.elliptical_pos)
""")



#// Set up simulation termination time

trick.sim_services.exec_set_terminate_time(5)
