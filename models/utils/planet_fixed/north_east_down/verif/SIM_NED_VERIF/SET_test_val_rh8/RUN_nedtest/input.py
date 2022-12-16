################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_nedtest/
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#   Purpose:   Input file used for JEOD NorthEastDown model unit test
#
#   Test:      Verify all methods of NorthEastDown work correctly
#
#*******************************************************************************/


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 0.10

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_ned_verif.py", "rb").read(), "Log_data/log_ned_verif.py", 'exec'))
log_ned_verif(LOG_CYCLE  )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################

#// Set pointers between test planet and test grav_source
earth.planet.grav_source = earth.gravity_source


#/* Test conversion from cartesian to spherical and elliptical coordinates, as
#   well as setting of translational (pos/vel) states based on conversion */
read = 1.0
trick.add_read(read, """
earth.cartesian_pos  = [ 6778.1363e3, 0.0, 0.0]
earth.update_from_cart ()
# earth.ned_frame.update_from_cart (earth.cartesian_pos)
""")



#/ Test setting of NED translational states directly
read = 2.0
trick.add_read(read, """
earth.cartesian_pos  = [ 6778.1363e3, 0.0, 0.0]
earth.cartesian_vel  = [ 0.0, 100.0, -750.0]
earth.set_ned_trans_states ()
# earth.ned_frame.set_ned_trans_states (earth.cartesian_pos, earth.cartesian_vel)
""")



#/* Test conversion from spherical to cartesian and elliptical coordinates, as
#   well as setting of frame's orientation states based on conversion */
read = 3.0
trick.add_read(read, """
earth.spherical_pos.altitude = 1000.0
earth.spherical_pos.latitude = 3.1416
earth.spherical_pos.longitude = 1.0
earth.ned_frame.update_from_spher (earth.spherical_pos)

earth.ned_frame.altlatlong_type = trick.NorthEastDown.spherical
earth.ned_frame.build_ned_orientation ()
""")



#// Reset enum defining the spherical/elliptical basis for N-E-D definition
read = 3.5
trick.add_read(read, """
earth.ned_frame.altlatlong_type = trick.NorthEastDown.undefined
""")


#/* Test conversion from elliptical to cartesian and spherical coordinates, as
#   well as setting of frame's orientation states based on conversion */
read = 4.0
trick.add_read(read, """
earth.elliptical_pos.altitude = 500.0
earth.elliptical_pos.latitude = 1.0
earth.elliptical_pos.longitude = 3.1416
earth.ned_frame.update_from_ellip (earth.elliptical_pos)

earth.ned_frame.altlatlong_type = trick.NorthEastDown.elliptical
earth.ned_frame.build_ned_orientation ()
""")



#// End simulation

trick.sim_services.exec_set_terminate_time(5)
