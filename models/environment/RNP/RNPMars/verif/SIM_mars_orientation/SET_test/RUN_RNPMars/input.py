################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_RNPMars/
####################################################################################

#*****************************************************************************/


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 86400.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_RNPMars_rec.py", "rb").read(), "Log_data/log_RNPMars_rec.py", 'exec'))
log_RNPMars( LOG_CYCLE)

#############  ASSIGNMENTS, CALLS, and CONTROL  #################

# * Set up the simulation executive parameters.
# *---------------------------------------------*/
exec(compile(open( "Modified_data/exec_setup.py", "rb").read(), "Modified_data/exec_setup.py", 'exec'))
set_trick_exec_setup()

# You have to have an integrator, whether you need one or not
dynamics.manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.manager.gravity_off = 1

#/*---------------------------------------------
# * Set up planetary bodies.
# *---------------------------------------------*/
#/* Configure the dynamics manager to operate in multi planet mode */
dynamics.manager_init.mode= trick.DynManagerInit.EphemerisMode_Ephemerides
env.de4xx.set_model_number(421)

#/* Control ephemeris update. */
#dynamics.manager.deriv_ephem_update = True

#/* Turn off Mars RNP polar motion. */
mars.rnp.enable_polar = False;

#/* Set up time structure */
exec(compile(open( "Modified_data/epoch.py", "rb").read(), "Modified_data/epoch.py", 'exec'))
set_Time_epoch(jeod_time)

#/*Set up simulation termination time (1 day). */

trick.sim_services.exec_set_terminate_time(86400*365)
