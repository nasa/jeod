################ TRICK HEADER ###############################################
# PURPOSE:
#   (To define the input data for simulation SET_test/RUN_test/
#############################################################################


#---------------------------------------------
# Set up the simulation executive parameters.
#---------------------------------------------
trick.sim_services.exec_set_trap_sigfpe = True


#---------------------------------------------
# Set up the logging parameters.
#---------------------------------------------
LOG_CYCLE = 1.0
exec(compile(open( "Log_data/log_veh_state_rec.py", "rb").read(), "Log_data/log_veh_state_rec.py", 'exec'))
log_veh_state_rec( LOG_CYCLE )
exec(compile(open( "Log_data/log_lvlh_state_rec.py", "rb").read(), "Log_data/log_lvlh_state_rec.py", 'exec'))
log_lvlh_state_rec( LOG_CYCLE )


#---------------------------------------------
# Set up the dynamics manager.
#---------------------------------------------
# Configure the dynamics manager to operate in single planet mode.
dynamics.manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
dynamics.manager_init.central_point_name = "ref_planet"

# Set up general numerical integration parameters.
dynamics.manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

# Set up the planet used for LVLH.
vehicle.reference_planet.set_name("ref_planet")

#---------------------------------------------
# Set up time.
#---------------------------------------------
jeod_time.time_manager_init.initializer = "TAI"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.truncated_julian
jeod_time.time_tai.initializing_value = 10000
jeod_time.time_tai.update_from_name = "Dyn"


#---------------------------------------------
# Configure the vehicle.
#---------------------------------------------
# Set the vehicle name.
vehicle.body.set_name("vehicle")

# Configure vehicle integration information.
vehicle.body.integ_frame_name = "ref_planet.inertial"
vehicle.body.translational_dynamics = True
vehicle.body.rotational_dynamics = True

# Configure vehicle mass properties.
exec(compile(open( "Modified_data/veh_mass.py", "rb").read(), "Modified_data/veh_mass.py", 'exec'))
set_vehicle_mass(vehicle, dynamics)

# Set vehicle initial state vector.
exec(compile(open( "Modified_data/veh_state.py", "rb").read(), "Modified_data/veh_state.py", 'exec'))
set_vehicle_state(vehicle, dynamics)

# Configure the LVLH reference frame.
vehicle.lvlh_frame.set_subject_name ("vehicle.composite_body")
vehicle.lvlh_frame.set_planet_name ("ref_planet")


#---------------------------------------------
# Set simulation stop time.
#---------------------------------------------
trick.sim_services.exec_set_terminate_time(100)
