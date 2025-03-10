################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_1/
####################################################################################

#Auto-generated from the data file with this header:
#/*****************************************************************************
#                      Run #1: Simulation Constants
#******************************************************************************
#
#Description:
#The purpose of this test is to verify the numerical constants associated with
#the orbital dynamics models used in the simulation.
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth = Spherical
#         Planetary ephemeris = Off
#         Third body perturbations = Off
#      Gravity Gradient Torque: Off
#      Atmosphere Model: Off
#         F10.7 = 128.8
#         Geomagnetic Index = 15.7
#      Aerodynamic Drag Model: Off
#         Coefficient of drag = N/A
#
#   Simulation Duration - 120000 sec
#
#   Data Collection Rate - 600 sec.
#
#
#*****************************************************************************/


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 600.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_sv_state_rec.py", "rb").read(), "Log_data/log_sv_state_rec.py", 'exec'))
log_sv_state_rec( LOG_CYCLE , "iss" , "sv_dyn" )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################


#/*---------------------------------------------
# * Default data generated from S_define.
# *---------------------------------------------*/
#include "S_default.dat"


#/*---------------------------------------------
# * Set up the simulation executive parameters.
# *---------------------------------------------*/







#/*---------------------------------------------
# * Set up general parameters to record.
# *---------------------------------------------*/
#define sv_dyn sv_dyn
#define iss iss
#define LOG_CYCLE 600.0
#include "Log_data/sv_state_rec.d"


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
#/*
# * Configure Earth planetary parameters.
# */
exec(compile(open( "Modified_data/Earth/params.py", "rb").read(), "Modified_data/Earth/params.py", 'exec'))
set_Earth_params(earth)

#/* Turn off polar motion. */
earth.rnp.enable_polar = 0

#/* Configure atmosphere. */
exec(compile(open( "Modified_data/Earth/soflx_mean.py", "rb").read(), "Modified_data/Earth/soflx_mean.py", 'exec'))
set_Earth_soflx_mean(earth)
exec(compile(open( "Modified_data/Earth/uniform_wind.py", "rb").read(), "Modified_data/Earth/uniform_wind.py", 'exec'))
set_Earth_uniform_wind(earth)


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

#/* Configure gravitational affects of the planets. */
exec(compile(open( "Modified_data/vehicle/grav_controls.py", "rb").read(), "Modified_data/vehicle/grav_controls.py", 'exec'))
set_vehicle_grav_controls(sv_dyn)

#/* Make the Earth gravity spherical. */
sv_dyn.earth_grav_ctrl.spherical = 1

#/* Configure planetary ephemeris model. */
#/* Turn off the Sun and Moon gravity effects. */
sv_dyn.sun_grav_ctrl.active = 0
sv_dyn.moon_grav_ctrl.active = 0

#/* Configure gravity gradient torque. */
sv_dyn.grav_torque.active = False

#/* Configure the atmosphere. */
sv_dyn.atmos.active = 0

#/* Configure aerodynamics. */
exec(compile(open( "Modified_data/aero/no_drag.py", "rb").read(), "Modified_data/aero/no_drag.py", 'exec'))
set_aero_no_drag(sv_dyn)

#/* Configuration vehicle mass properties. */
exec(compile(open( "Modified_data/mass/iss.py", "rb").read(), "Modified_data/mass/iss.py", 'exec'))
set_mass_iss(sv_dyn)

#CALL
dynamics.manager.add_body_action(sv_dyn.mass_init)


#/* Configuration vehicle initial state. */
exec(compile(open( "Modified_data/state/iss_typical.py", "rb").read(), "Modified_data/state/iss_typical.py", 'exec'))
set_state_iss_typical(sv_dyn)


#CALL
dynamics.manager.add_body_action(sv_dyn.trans_init)



#CALL
dynamics.manager.add_body_action(sv_dyn.lvlh_init)



trick.add_read(60000,  """
jeod_time.manager.dyn_time.scale_factor = -1.0
""")

trick.sim_services.exec_set_terminate_time(120000)
