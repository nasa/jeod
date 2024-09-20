################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_compare/
####################################################################################


trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_radiation_rec.py", "rb").read(), "Log_data/log_radiation_rec.py", 'exec'))
log_radiation_rec( LOG_CYCLE )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################
#define LOG_CYCLE 1.0

#include "S_default.dat"
#include "Log_data/radiation_rec.d"


#/******************************************************************************/
#/* Set up the time object:                                                    */
#/******************************************************************************/
exec(compile(open( "Modified_data/date_and_time.py", "rb").read(), "Modified_data/date_and_time.py", 'exec'))


#/******************************************************************************/
#/* Set up the vehicle object:                                                 */
#/******************************************************************************/
exec(compile(open( "Modified_data/vehicle_baseline.py", "rb").read(), "Modified_data/vehicle_baseline.py", 'exec'))


#/******************************************************************************/
#/* Set up the radiation object (the one with the surface model surface)       */
#/******************************************************************************/
exec(compile(open( "Modified_data/radiation_surface.py", "rb").read(), "Modified_data/radiation_surface.py", 'exec'))
exec(compile(open( "Modified_data/third_bodies.py", "rb").read(), "Modified_data/third_bodies.py", 'exec'))




#/******************************************************************************/
#/* Set up the simple_inside radiation object                                  */
#/* This sphere fits inside the cube                                           */
#/******************************************************************************/
radiation_simple_inside.rad_surface.cx_area = 3.14
radiation_simple_inside.rad_surface.albedo = 0.5
radiation_simple_inside.rad_surface.diffuse = 0.5
radiation_simple_inside.rad_surface.temperature = 270.0
radiation_simple_inside.rad_surface.thermal.active = True
radiation_simple_inside.rad_surface.thermal.thermal_power_dump = 0.0
radiation_simple_inside.rad_surface.thermal.emissivity = 0.5
radiation_simple_inside.rad_surface.thermal.heat_capacity = 628.0
radiation_simple_inside.rad_surface.set_name("simple_inside")


#/******************************************************************************/
#/* Set up the simple_cx_area radiation object                                 */
#/* This has the same cross-sectional area as each face of the cube            */
#/******************************************************************************/
radiation_simple_cx_area.rad_surface.cx_area = 4
radiation_simple_cx_area.rad_surface.albedo = 0.5
radiation_simple_cx_area.rad_surface.diffuse = 0.5
radiation_simple_cx_area.rad_surface.temperature = 270.0
radiation_simple_cx_area.rad_surface.thermal.active = True
radiation_simple_cx_area.rad_surface.thermal.thermal_power_dump = 0.0
radiation_simple_cx_area.rad_surface.thermal.emissivity = 0.5
radiation_simple_cx_area.rad_surface.thermal.heat_capacity = 800.0
radiation_simple_cx_area.rad_surface.set_name("simple_cx_area")


#/******************************************************************************/
#/* Set up the simple_surf_area radiation object                               */
#/* This has the same surface area as the cube                                 */
#/******************************************************************************/
radiation_simple_surf_area.rad_surface.cx_area = 6
radiation_simple_surf_area.rad_surface.albedo = 0.5
radiation_simple_surf_area.rad_surface.diffuse = 0.5
radiation_simple_surf_area.rad_surface.temperature = 270.0
radiation_simple_surf_area.rad_surface.thermal.active = True
radiation_simple_surf_area.rad_surface.thermal.thermal_power_dump = 0.0
radiation_simple_surf_area.rad_surface.thermal.emissivity = 0.5
radiation_simple_surf_area.rad_surface.thermal.heat_capacity = 1200.0
radiation_simple_surf_area.rad_surface.set_name("simple_surf_area")

#/******************************************************************************/
#/* Set up the simple_outside radiation object                                 */
#/******************************************************************************/
radiation_simple_outside.rad_surface.cx_area = 9.42
radiation_simple_outside.rad_surface.albedo = 0.5
radiation_simple_outside.rad_surface.diffuse = 0.5
radiation_simple_outside.rad_surface.temperature = 270.0
radiation_simple_outside.rad_surface.thermal.active = True
radiation_simple_outside.rad_surface.thermal.thermal_power_dump = 0.0
radiation_simple_outside.rad_surface.thermal.emissivity = 0.5
radiation_simple_outside.rad_surface.thermal.heat_capacity = 1884.0
radiation_simple_outside.rad_surface.set_name("simple_outside")






trick.sim_services.exec_set_terminate_time(10000)
