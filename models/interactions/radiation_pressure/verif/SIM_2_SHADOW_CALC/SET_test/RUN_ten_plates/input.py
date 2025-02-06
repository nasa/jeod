################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_ten_plates/
####################################################################################

#Auto-generated from the data file with this header:
#/* Logging rate */


trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_radiation_rec_v2.py", "rb").read(), "Log_data/log_radiation_rec_v2.py", 'exec'))
log_radiation_rec_v2( LOG_CYCLE )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################
#define LOG_CYCLE 1.0

#/* Default data generated from S_define. */
#include "S_default.dat"

#/* Parameters to data record. */
#include "Log_data/radiation_rec_v2.d"



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
vehicle.dyn_body.structure.state.trans.position = [ -0.3648654*1.0E6, -0.85422932*1.0E6, -0.3703588*1.0E6]



vehicle.dyn_body.structure.state.rot.T_parent_this[0]  = [ 1, 0, 0]
vehicle.dyn_body.structure.state.rot.T_parent_this[1]  = [ 0, 1, 0]
vehicle.dyn_body.structure.state.rot.T_parent_this[2]  = [ 0, 0, 1]



#/******************************************************************************/
#/* Set up the radiation object                                                */
#/******************************************************************************/
exec(compile(open( "Modified_data/radiation_surface_v2.py", "rb").read(), "Modified_data/radiation_surface_v2.py", 'exec'))
exec(compile(open( "Modified_data/third_bodies.py", "rb").read(), "Modified_data/third_bodies.py", 'exec'))

trick.sim_services.exec_set_terminate_time(1000)
