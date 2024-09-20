################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_shadow_cooling/
####################################################################################

#Auto-generated from the data file with this header:
#/* Logging rate */


trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_radiation_rec.py", "rb").read(), "Log_data/log_radiation_rec.py", 'exec'))
log_radiation_rec( LOG_CYCLE )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################
#define LOG_CYCLE 1.0

#/* Default data generated from S_define. */
#include "S_default.dat"

#/* Parameters to data record. */
#include "Log_data/radiation_rec.d"



#/******************************************************************************/
#/* Set up the time object:                                                    */
#/******************************************************************************/
exec(compile(open( "Modified_data/date_and_time.py", "rb").read(), "Modified_data/date_and_time.py", 'exec'))


#/******************************************************************************/
#/* Set up the vehicle object:                                                 */
#/******************************************************************************/

vehicle.dyn_body.set_name("vehicle")
vehicle.dyn_body.integ_frame_name = "Earth.inertial"
vehicle.dyn_body.composite_body.state.trans.position  = [ 0.0, 0.0, 0.0]
vehicle.dyn_body.structure.state.trans.position  = [  364.8, 854.2, 370.4]
vehicle.dyn_body.structure.state.rot.T_parent_this[0]  = [ 1, 0, 0]
vehicle.dyn_body.structure.state.rot.T_parent_this[1]  = [ 0, 1, 0]
vehicle.dyn_body.structure.state.rot.T_parent_this[2]  = [ 0, 0, 1]



#/******************************************************************************/
#/* Set up the radiation object                                                */
#/******************************************************************************/
exec(compile(open( "Modified_data/radiation_surface_a.py", "rb").read(), "Modified_data/radiation_surface_a.py", 'exec'))
exec(compile(open( "Modified_data/third_bodies.py", "rb").read(), "Modified_data/third_bodies.py", 'exec'))


trick.sim_services.exec_set_terminate_time(1000)
