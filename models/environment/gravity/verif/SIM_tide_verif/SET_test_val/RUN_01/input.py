################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_01/
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
#      Gravity Manager: On
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

exec(compile(open( "Log_data/log_tide_sim.py", "rb").read(), "Log_data/log_tide_sim.py", 'exec'))

#/*---------------------------------------------
# * Set up the dynamics manager.
# *---------------------------------------------*/
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager.deriv_ephem_update = True

#/*---------------------------------------------
# * Set up simulation date and time.
# *---------------------------------------------*/
exec(compile(open( "Modified_data/time.py", "rb").read(), "Modified_data/time.py", 'exec'))

#/*---------------------------------------------
# * Set up planetary bodies.
# *---------------------------------------------*/

# Turn off polar motion.
earth.rnp.enable_polar = 0

# Configure atmosphere.
exec(compile(open( "Modified_data/solar_flux_mean.py", "rb").read(), "Modified_data/solar_flux_mean.py", 'exec'))
exec(compile(open( "Modified_data/uniform_wind.py", "rb").read(), "Modified_data/uniform_wind.py", 'exec'))

#/*---------------------------------------------
# * Configure the vehicle.
# *---------------------------------------------*/

# Set the vehicle name.
sv_dyn.dyn_body.set_name("iss")

# Configure vehicle integration information.
sv_dyn.dyn_body.integ_frame_name = "Earth.inertial"
sv_dyn.dyn_body.translational_dynamics = True
sv_dyn.dyn_body.rotational_dynamics = True

# Configure the derived reference frames.
sv_dyn.pfix.reference_name     = "Earth"
sv_dyn.lvlh.reference_name     = "Earth"
sv_dyn.orb_elem.reference_name = "Earth"

# Set up the gravity controls for the Earth.
sv_dyn.earth_grav_ctrl.source_name     = "Earth"
sv_dyn.earth_grav_ctrl.active          = True
sv_dyn.earth_grav_ctrl.spherical       = False
sv_dyn.earth_grav_ctrl.degree          = 8
sv_dyn.earth_grav_ctrl.order           = 8
sv_dyn.earth_grav_ctrl.gradient        = True

# Set up the Earth solid body tides
earth.sb_tide.dC20 = 0.0

sv_dyn.sbtide_ctrl.active = True
sv_dyn.sbtide_ctrl.first_order_only = True
sv_dyn.sbtide_ctrl.grav_effect = earth.sb_tide
sv_dyn.sbtide_ctrl.grav_source   = earth.gravity_source

sv_dyn.earth_grav_ctrl.add_deltacontrol(sv_dyn.sbtide_ctrl)
sv_dyn.dyn_body.grav_interaction.add_control(sv_dyn.earth_grav_ctrl)


# Set up the gravity controls for the Sun.
sv_dyn.sun_grav_ctrl.source_name     = "Sun"
sv_dyn.sun_grav_ctrl.active          = True
sv_dyn.sun_grav_ctrl.spherical       = True
sv_dyn.sun_grav_ctrl.gradient        = True

sv_dyn.dyn_body.grav_interaction.add_control(sv_dyn.sun_grav_ctrl)


# Set up the gravity controls for the Moon.
sv_dyn.moon_grav_ctrl.source_name     = "Moon"
sv_dyn.moon_grav_ctrl.active          = True
sv_dyn.moon_grav_ctrl.spherical       = True
sv_dyn.moon_grav_ctrl.gradient        = True

sv_dyn.dyn_body.grav_interaction.add_control(sv_dyn.moon_grav_ctrl)


# Configure gravity gradient torque.
sv_dyn.grav_torque.active = True

# Configure the atmosphere.
sv_dyn.atmos_state.active = True

# Configure aerodynamics (~ISS Aerodynamic Parameters).
# This is the value for Cd = 2.0 and Area = 1400.0
sv_dyn.aero_drag.active = True
sv_dyn.aero_drag.ballistic_drag.option = trick.DefaultAero.DRAG_OPT_CD
sv_dyn.aero_drag.ballistic_drag.Cd = 2.0
sv_dyn.aero_drag.ballistic_drag.area = 1400.0

# Configuration vehicle mass properties.
exec(compile(open( "Modified_data/mass.py", "rb").read(), "Modified_data/mass.py", 'exec'))

# Configuration vehicle initial state.
exec(compile(open( "Modified_data/state.py", "rb").read(), "Modified_data/state.py", 'exec'))
state_iss_highly_elliptical()


trick.stop(28800)
