################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_basic_cr/
####################################################################################

#Auto-generated from the data file with this header:
#/******************************************************************************/


trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_radiation_rec.py", "rb").read(), "Log_data/log_radiation_rec.py", 'exec'))
log_radiation_rec( LOG_CYCLE )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################
#/* This input file uses the radiation coefficient for the default surface.    */
#/******************************************************************************/

#/******************************************************************************/
#/* Set up the time object:                                                    */
#/******************************************************************************/
jeod_time.time_manager_init.initializer = "TAI"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

jeod_time.time_tai.calendar_year = 2005
jeod_time.time_tai.calendar_month = 12
jeod_time.time_tai.calendar_day = 31
jeod_time.time_tai.calendar_hour = 23
jeod_time.time_tai.calendar_minute = 59
jeod_time.time_tai.calendar_second = 50.0

jeod_time.time_tai.update_from_name = "Dyn"

jeod_time.time_tt.initialize_from_name = "TAI"
jeod_time.time_tt.update_from_name = "TAI"


#/******************************************************************************/
#/* Set up the vehicle object:                                                 */
#/******************************************************************************/

vehicle.dyn_body.set_name("vehicle")
vehicle.dyn_body.integ_frame_name = "Sun.inertial"
vehicle.dyn_body.composite_body.state.trans.position  = [ 0.0, 0.0, 0.0]
vehicle.dyn_body.structure.state.trans.position  = [  1.5E+11, 0.0, 0.0]
vehicle.dyn_body.structure.state.rot.T_parent_this[0]  = [ 1, 0, 0]
vehicle.dyn_body.structure.state.rot.T_parent_this[1]  = [ 0, 1, 0]
vehicle.dyn_body.structure.state.rot.T_parent_this[2]  = [ 0, 0, 1]

#/******************************************************************************/
#/* Set up the vehicle2 object:                                                 */
#/******************************************************************************/

vehicle2.dyn_body.set_name("vehicle2")
vehicle2.dyn_body.integ_frame_name = "Sun.inertial"
vehicle2.dyn_body.composite_body.state.trans.position  = [ 1.4999999E+11, 0.0, 0.0]
vehicle2.dyn_body.structure.state.trans.position  = [  1.4999999E+11, 0.0, 0.0]
vehicle2.dyn_body.structure.state.rot.T_parent_this[0]  = [ 1, 0, 0]
vehicle2.dyn_body.structure.state.rot.T_parent_this[1]  = [ 0, 1, 0]
vehicle2.dyn_body.structure.state.rot.T_parent_this[2]  = [ 0, 0, 1]

#/******************************************************************************/
#/* Set up the radiation object (the one with the surface model surface)       */
#/******************************************************************************/
exec(compile(open( "Modified_data/radiation_surface.py", "rb").read(), "Modified_data/radiation_surface.py", 'exec'))
exec(compile(open( "Modified_data/third_bodies.py", "rb").read(), "Modified_data/third_bodies.py", 'exec'))
#set_radiation_surface(radiation)




#/******************************************************************************/
#/* Set up the simple radiation object (the one with the default surface)      */
#/******************************************************************************/
radiation_simple.rad_surface.cx_area = 2.0
#//radiation_simple.rad_surface.albedo = 0.5
#//radiation_simple.rad_surface.diffuse = 0.5
radiation_simple.rad_surface.rad_coeff = 1.111
radiation_simple.rad_surface.temperature = 270.0
radiation_simple.rad_surface.thermal.active = True
radiation_simple.rad_surface.thermal.thermal_power_dump = 0.0
radiation_simple.rad_surface.thermal.emissivity = 0.5
radiation_simple.rad_surface.thermal.heat_capacity = 1200.0







trick.sim_services.exec_set_terminate_time(200)
