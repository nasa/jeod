################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_full/
####################################################################################

#Auto-generated from the data file with this header:
#/*****************************************************************************
#                    Run Full: Full Model Comparison
#******************************************************************************
#
#Description:
#The purpose of this test is to verify that the output of the NASA simulation
#MET atmosphere model.
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth = Non-spherical Earth GEM-T1 8x8
#         Planetary ephemeris = On
#         Third body perturbations = On
#      Gravity Gradient Torque: On
#      Atmosphere Model: On
#         F10.7 = 128.8
#         Geomagnetic Index = 15.7
#      Aerodynamic Drag Model: On
#         Coefficient of drag = 0.02
#
#   Simulation Duration - 28800 sec (8 hrs)
#
#   Data Collection Rate - 60 sec.
#
#
#*****************************************************************************/


trick.sim_services.exec_set_trap_sigfpe(1)

#---------------------------------------------
# Set up general parameters to record.
#---------------------------------------------
LOG_CYCLE = 60.0

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


#---------------------------------------------
# Set up the simulation executive parameters.
#---------------------------------------------






#---------------------------------------------
# Set up the dynamics manager.
#---------------------------------------------

# Control epheris update.
dynamics.manager.deriv_ephem_update = True

# Setup general numerical integration parameters.
exec(compile(open( "Modified_data/state/integrator.py", "rb").read(), "Modified_data/state/integrator.py", 'exec'))
set_state_integrator(dynamics)


#---------------------------------------------
# Set up simulation date and time.
#---------------------------------------------
exec(compile(open( "Modified_data/date_n_time/11Nov2007.py", "rb").read(), "Modified_data/date_n_time/11Nov2007.py", 'exec'))
set_date_n_time_11Nov2007(jeod_time)


#---------------------------------------------
# Set up planetary bodies.
#---------------------------------------------

# Configure Earth planetary parameters.
exec(compile(open( "Modified_data/Earth/params.py", "rb").read(), "Modified_data/Earth/params.py", 'exec'))
set_Earth_params(earth)

# Turn off polar motion.
earth.rnp.enable_polar = 0

# Configure atmosphere.
exec(compile(open( "Modified_data/Earth/soflx_mean.py", "rb").read(), "Modified_data/Earth/soflx_mean.py", 'exec'))
set_Earth_soflx_mean(earth)
exec(compile(open( "Modified_data/Earth/uniform_wind.py", "rb").read(), "Modified_data/Earth/uniform_wind.py", 'exec'))
set_Earth_uniform_wind(earth)


#---------------------------------------------
# Configure the vehicle.
#---------------------------------------------

# Set the vehicle name.
sv_dyn.body.set_name("iss")

# Configure vehicle integration information.
sv_dyn.body.integ_frame_name = "Earth.inertial"
sv_dyn.body.translational_dynamics = True
sv_dyn.body.rotational_dynamics = True

# Configure the dervied reference frames.
sv_dyn.pfix.reference_name     = "Earth"
sv_dyn.lvlh.reference_name     = "Earth"
sv_dyn.orb_elem.reference_name = "Earth"

# Configure gravitational effects of the planets.
exec(compile(open( "Modified_data/vehicle/grav_controls.py", "rb").read(), "Modified_data/vehicle/grav_controls.py", 'exec'))
set_vehicle_grav_controls(sv_dyn)

# Make the Earth gravity GEMT1 8x8.
sv_dyn.earth_grav_ctrl.spherical = 0
sv_dyn.earth_grav_ctrl.degree    = 8
sv_dyn.earth_grav_ctrl.order     = 8

# Configure other planetary gravity.
sv_dyn.sun_grav_ctrl.active  = 1
sv_dyn.moon_grav_ctrl.active = 1

# Configure gravity gradient torque.
sv_dyn.grav_torque.active = 1

# Configure the atmosphere.
sv_dyn.atmos.active = 1

# Configure aerodynamics.
exec(compile(open( "Modified_data/aero/drag.py", "rb").read(), "Modified_data/aero/drag.py", 'exec'))
set_aero_drag(sv_dyn)

# Configure aerodynamics (~ISS Aerodynamic Parameters).
# This is the value for Cd = 2.0 and Area = 1400.0
sv_dyn.aero_drag.ballistic_drag.option = trick.DefaultAero.DRAG_OPT_CD
sv_dyn.aero_drag.ballistic_drag.Cd = 2.0
sv_dyn.aero_drag.ballistic_drag.area = 1400.0

# Configuration vehicle mass properties.
exec(compile(open( "Modified_data/mass/iss.py", "rb").read(), "Modified_data/mass/iss.py", 'exec'))
set_mass_iss(sv_dyn)

dynamics.manager.add_body_action(sv_dyn.mass_init)


# Configuration vehicle initial state.
exec(compile(open( "Modified_data/state/iss_highly_elliptic.py", "rb").read(), "Modified_data/state/iss_highly_elliptic.py", 'exec'))
set_state_iss_highly_elliptic(sv_dyn)
dynamics.manager.add_body_action(sv_dyn.trans_init)
dynamics.manager.add_body_action(sv_dyn.lvlh_init)


# Set up simulation termination time (8 hours).
trick.sim_services.exec_set_terminate_time(28800)
