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
#
#Run Scenario:
#   Model Setup -
#      Gravity Manager: On
#         Earth = Non-spherical Earth GEM-T1 8x8
#         Planetary ephemeris = On
#         Third body perturbations = On
#      Gravity Gradient Torque: On
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

# Configuration vehicle mass properties.
exec(compile(open( "Modified_data/mass.py", "rb").read(), "Modified_data/mass.py", 'exec'))

# Configuration vehicle initial state.
exec(compile(open( "Modified_data/state.py", "rb").read(), "Modified_data/state.py", 'exec'))
state_iss_highly_elliptical()

trick.stop(28800)
