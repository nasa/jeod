################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_radiation/ 
####################################################################################


trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4;

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1000.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_orbit_rec.py", "rb").read(), "Log_data/log_orbit_rec.py", 'exec'))
log_orbit_rec( LOG_CYCLE )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################
#define LOG_CYCLE 1000.0 

#/* Default data generated from S_define. */
#include "S_default.dat"

#/* Parameters to data record. */
#include "Log_data/orbit_rec.d"


#FIXME:  sys.exec.in.trap_sigfpe  = Yes
#FIXME:  sys.exec.in.pre_init_checkpoint  = No
#FIXME:  sys.exec.in.post_init_checkpoint = No
#FIXME:  sys.exec.in.end_checkpoint       = No
#FIXME:  sys.exec.in.reduced_checkpoint   = No   
#FIXME:  sys.exec.sim_com.monitor_on      = No   
#FIXME:  sys.exec.rt_log.group[0].record  = No

#// 1024Hz time tic necessary for simtime over 100 days
#FIXME:  sys.exec.in.time_tic = 0.0009765625


#/******************************************************************************/
#/* Set up the time object:                                                    */
#/******************************************************************************/
exec(compile(open( "Modified_data/date_and_time.py", "rb").read(), "Modified_data/date_and_time.py", 'exec'))


#/******************************************************************************/
#/* Set up the manager object:                                                 */
#/******************************************************************************/
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
#FIXME:  dynamics.integ.first_step_deriv = Yes
dynamics.dyn_manager.deriv_ephem_update = False


#/******************************************************************************/
#/* Set up the vehicle object:                                                 */
#/******************************************************************************/
exec(compile(open( "Modified_data/vehicle_baseline.py", "rb").read(), "Modified_data/vehicle_baseline.py", 'exec'))



#/******************************************************************************/
#/* Set up the radiation object                                                */
#/******************************************************************************/
exec(compile(open( "Modified_data/radiation_surface.py", "rb").read(), "Modified_data/radiation_surface.py", 'exec'))
exec(compile(open( "Modified_data/third_bodies.py", "rb").read(), "Modified_data/third_bodies.py", 'exec'))

#//turn off moon
radiation.rad_pressure.set_third_body_inactive( "Moon")


trick.sim_services.exec_set_terminate_time(2000000)
#trick.sim_services.exec_set_terminate_time(63000000)
