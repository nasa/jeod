################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_02/ 
####################################################################################

#Auto-generated from the data file with this header:
#/*---------------------------------------------
# * Default data generated from S_define.
# *---------------------------------------------*/


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_sv_force_torque_rec.py", "rb").read(), "Log_data/log_sv_force_torque_rec.py", 'exec'))
log_sv_force_torque_rec( LOG_CYCLE , "iss" , "sv_dyn" )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################
#include "S_default.dat"


#/*---------------------------------------------
# * Set up the simulation executive parameters.
# *---------------------------------------------*/






#/*---------------------------------------------
# * Set up general parameters to record.
# *---------------------------------------------*/
#define sv_dyn sv_dyn
#define iss iss
#define LOG_CYCLE 1.0
#include "Log_data/sv_force_torque_rec.d"


#/*---------------------------------------------
# * Set up the dynamics manager.
# *---------------------------------------------*/
#/* Control epheris update. */
dynamics.manager.deriv_ephem_update = True

#/* Setup general numerical integration parameters. */
exec(compile(open( "Modified_data/state/integrator.py", "rb").read(), "Modified_data/state/integrator.py", 'exec'))
set_state_integrator(dynamics)


#/*---------------------------------------------
# * Set up simulation date and time.
# *---------------------------------------------*/
exec(compile(open( "Modified_data/date_n_time/11Nov2007.py", "rb").read(), "Modified_data/date_n_time/11Nov2007.py", 'exec'))
set_date_n_time_11Nov2007(jeod_time)


#/*---------------------------------------------
# * Set up planetary bodies.
# *---------------------------------------------*/


#/*---------------------------------------------
# * Configure the vehicle.
# *---------------------------------------------*/
#/*
# * Configure the general test vehicle.
# */

#/* Set the vehicle name. */
sv_dyn.body.set_name("iss")

#/* Configure vehicle integration information. */
sv_dyn.body.integ_frame_name = "Earth.inertial"
sv_dyn.body.translational_dynamics = True
sv_dyn.body.rotational_dynamics = True

#/* Configure the dervied reference frames. */
sv_dyn.pfix.reference_name     = "Earth"
sv_dyn.lvlh.reference_name     = "Earth"
sv_dyn.orb_elem.reference_name = "Earth"


#/* Set up the gravity controls for the Earth. */
sv_dyn.earth_grav_ctrl.source_name     = "Earth"
sv_dyn.earth_grav_ctrl.active          = True
sv_dyn.earth_grav_ctrl.spherical       = True
sv_dyn.earth_grav_ctrl.degree          = 20
sv_dyn.earth_grav_ctrl.order           = 20
sv_dyn.earth_grav_ctrl.gradient        = True
sv_dyn.earth_grav_ctrl.gradient_degree = 0
sv_dyn.earth_grav_ctrl.gradient_order  = 0

sv_dyn.body.grav_interaction.add_control(sv_dyn.earth_grav_ctrl)

#/* Set up the gravity controls for the Sun. */
sv_dyn.sun_grav_ctrl.source_name   = "Sun"
sv_dyn.sun_grav_ctrl.active        = True
sv_dyn.sun_grav_ctrl.spherical     = True
sv_dyn.sun_grav_ctrl.gradient      = False

sv_dyn.body.grav_interaction.add_control(sv_dyn.sun_grav_ctrl)

#/* Set up the gravity controls for the Moon. */
sv_dyn.moon_grav_ctrl.source_name  = "Moon"
sv_dyn.moon_grav_ctrl.active       = True
sv_dyn.moon_grav_ctrl.spherical    = True
sv_dyn.moon_grav_ctrl.gradient     = False

sv_dyn.body.grav_interaction.add_control(sv_dyn.moon_grav_ctrl)


#/* Configure gravity gradient torque. */
sv_dyn.grav_torque.active = True


#/* Configuration vehicle mass properties. */
exec(compile(open( "Modified_data/mass/iss.py", "rb").read(), "Modified_data/mass/iss.py", 'exec'))
set_mass_iss(sv_dyn)

#CALL
dynamics.manager.add_body_action(sv_dyn.mass_init)


#/* Configuration vehicle initial state. */
#//#include "Modified_data/state/iss_highly_elliptic.d"
exec(compile(open( "Modified_data/state/iss_typical.py", "rb").read(), "Modified_data/state/iss_typical.py", 'exec'))
set_state_iss_typical(sv_dyn)

#CALL
dynamics.manager.add_body_action(sv_dyn.trans_init)


#CALL
dynamics.manager.add_body_action(sv_dyn.lvlh_init)



#/*Set up simulation termination time (8 hours). */

trick.sim_services.exec_set_terminate_time(10800)
