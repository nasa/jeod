################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_10A/
####################################################################################

#Auto-generated from the data file with this header:
#/*****************************************************************************
#   Run #10A: Rot State Comparison: Gravity Gradient, Circular, 0 Rate
#******************************************************************************
#
#Description:
#The purpose of this test is to verify the numerical integration routines and
#math model for the propagation of the rotational state of an orbital vehicle.
#The orbital vehicle is a special ISS 'cylinder-like' configuration with a mass
#of 1000kg, 12 meters long and 1 meter in radius.  The inertia matrix in the
#structural reference fram has center of mass at 6.0,0.0,0.0 meters.  The
#inertia matrix is diagonal with Ixx = 500.0 , Iyy = 12250.0 , Izz = 12250.0
#(kgM^2).  The initial ISS rotational state is at a special attitude.  Attitude
#is set wrt the LVLH frame.  In in-plane displacement (Pitch) is 5 degrees, the
#out-or-plane displacement is 1 degree.  The analytic solution for this
#'cylinder' is that the amplitude of the gravity gradient libration will be a
#max and min of 5 degrees in plane with a period of 3257.94 seconds and an out
#of plane libration max and min of 1 degrees with a period of 2821.46 seconds.
#
#References:
#   Spacecraft Attitude Dynamics, Peter C. Hughes, Dover Edition 2004,
#   pages (232 - 353)
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth = Spherical
#         Planetary ephemeris = Off
#         Third body perturbations = Off
#      Gravity Gradient Torque: On
#      Atmosphere Model: On
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
exec(compile(open( "Log_data/log_earth_RNP_rec.py", "rb").read(), "Log_data/log_earth_RNP_rec.py", 'exec'))
log_earth_RNP_rec( LOG_CYCLE )
exec(compile(open( "Log_data/log_earth_time_rec.py", "rb").read(), "Log_data/log_earth_time_rec.py", 'exec'))
log_earth_time_rec( LOG_CYCLE )
exec(compile(open( "Log_data/log_planetary_ephem_rec.py", "rb").read(), "Log_data/log_planetary_ephem_rec.py", 'exec'))
log_planetary_ephem_rec( LOG_CYCLE )
exec(compile(open( "Log_data/log_sv_aero_rec.py", "rb").read(), "Log_data/log_sv_aero_rec.py", 'exec'))
log_sv_aero_rec( LOG_CYCLE , "iss" , "sv_dyn" )
exec(compile(open( "Log_data/log_sv_atmos_rec.py", "rb").read(), "Log_data/log_sv_atmos_rec.py", 'exec'))
log_sv_atmos_rec( LOG_CYCLE , "iss" , "sv_dyn" )
exec(compile(open( "Log_data/log_sv_grav_rec.py", "rb").read(), "Log_data/log_sv_grav_rec.py", 'exec'))
log_sv_grav_rec( LOG_CYCLE , "iss" , "sv_dyn" )
exec(compile(open( "Log_data/log_sv_orbelem_rec.py", "rb").read(), "Log_data/log_sv_orbelem_rec.py", 'exec'))
log_sv_orbelem_rec( LOG_CYCLE , "iss" , "sv_dyn" )
exec(compile(open( "Log_data/log_sv_state_rec.py", "rb").read(), "Log_data/log_sv_state_rec.py", 'exec'))
log_sv_state_rec( LOG_CYCLE , "iss" , "sv_dyn" )
exec(compile(open( "Log_data/log_sv_rot_state_rec.py", "rb").read(), "Log_data/log_sv_rot_state_rec.py", 'exec'))
log_sv_rot_state_rec( LOG_CYCLE , "iss" , "sv_dyn" )
exec(compile(open( "Log_data/log_sv_force_torque_rec.py", "rb").read(), "Log_data/log_sv_force_torque_rec.py", 'exec'))
log_sv_force_torque_rec( LOG_CYCLE , "iss" , "sv_dyn" )
exec(compile(open( "Log_data/log_Earth_RNP_ascii_rec.py", "rb").read(), "Log_data/log_Earth_RNP_ascii_rec.py", 'exec'))
log_Earth_RNP_ascii_rec( LOG_CYCLE )
exec(compile(open( "Log_data/log_sv_state_ascii_rec.py", "rb").read(), "Log_data/log_sv_state_ascii_rec.py", 'exec'))
log_sv_state_ascii_rec( LOG_CYCLE , "iss" , "sv_dyn" )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################


#/*---------------------------------------------
# * Default data generated from S_define.
# *---------------------------------------------*/
#include "S_default.dat"


#/*---------------------------------------------
# * Set up the simulation executive parameters.
# *---------------------------------------------*/
exec(compile(open( "Modified_data/trick/no_debug.py", "rb").read(), "Modified_data/trick/no_debug.py", 'exec'))
set_trick_no_debug()
exec(compile(open( "Modified_data/trick/exec_setup.py", "rb").read(), "Modified_data/trick/exec_setup.py", 'exec'))
set_trick_exec_setup()
#FIXME:  sys.exec.in.trap_sigfpe = Yes


#/*---------------------------------------------
# * Set up general parameters to record.
# *---------------------------------------------*/
#define sv_dyn sv_dyn
#define iss iss
#define LOG_CYCLE 600.0
#//#include "Log_data/earth_RNP_rec.d"
#//#include "Log_data/earth_time_rec.d"
#//#include "Log_data/planetary_ephem_rec.d"
#//#include "Log_data/sv_aero_rec.d"
#//#include "Log_data/sv_atmos_rec.d"
#//#include "Log_data/sv_grav_rec.d"
#//#include "Log_data/sv_orbelem_rec.d"
#include "Log_data/sv_state_rec.d"
#include "Log_data/sv_rot_state_rec.d"
#//#include "Log_data/sv_force_torque_rec.d"

#/* Set up special ASCII recording group. */
#//#include "Log_data/Earth_RNP_ascii_rec.d"
#//#include "Log_data/sv_state_ascii_rec.d"


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
sv_dyn.lvlh_euler.sequence     = trick.Orientation.Pitch_Yaw_Roll

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
sv_dyn.grav_torque.active = 1

#/* Configure the atmosphere. */
sv_dyn.atmos.active = 1

#/* Configure aerodynamics. */
exec(compile(open( "Modified_data/aero/no_drag.py", "rb").read(), "Modified_data/aero/no_drag.py", 'exec'))
set_aero_no_drag(sv_dyn)

#/* Configuration vehicle mass properties. */
exec(compile(open( "Modified_data/mass/cylinder.py", "rb").read(), "Modified_data/mass/cylinder.py", 'exec'))
set_mass_cylinder(sv_dyn)

#CALL
dynamics.manager.add_body_action(sv_dyn.mass_init)


#/* Configuration vehicle initial state. */
exec(compile(open( "Modified_data/state/iss_typical.py", "rb").read(), "Modified_data/state/iss_typical.py", 'exec'))
set_state_iss_typical(sv_dyn)


#CALL
dynamics.manager.add_body_action(sv_dyn.trans_init)


sv_dyn.lvlh_init.orientation.euler_sequence      = trick.Orientation.Roll_Pitch_Yaw
sv_dyn.lvlh_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0, 85.0, 1.0])

#CALL
dynamics.manager.add_body_action(sv_dyn.lvlh_init)



trick.add_read(60000,  """
jeod_time.manager.dyn_time.scale_factor = -1.0
""")

trick.sim_services.exec_set_terminate_time(120000)
