exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(1.0)

dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "Space"

rk_integrator = trick.RK4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = rk_integrator

vehicle.dyn_body.set_name("veh")
vehicle.dyn_body.integ_frame_name = "Space.inertial"
vehicle.dyn_body.translational_dynamics = True
vehicle.dyn_body.rotational_dynamics = True

vehicleB.dyn_body.set_name("vehB")
vehicleB.dyn_body.integ_frame_name = "Space.inertial"
vehicleB.dyn_body.translational_dynamics = True
vehicleB.dyn_body.rotational_dynamics = True

exec(compile(open("Modified_data/vehicle_mass_props.py", "rb").read(), "Modified_data/vehicle_mass_props.py", 'exec'))
mass_props(vehicle)
mass_props(vehicleB)

exec(compile(open("Modified_data/vehicle_state.py", "rb").read(), "Modified_data/vehicle_state.py", 'exec'))
exec(compile(open("Modified_data/vehicleB_state.py", "rb").read(), "Modified_data/vehicleB_state.py", 'exec'))
exec(compile(open("Modified_data/surface.py", "rb").read(), "Modified_data/surface.py", 'exec'))

# Will be overwritten, but it is good practice to always put a trick-stop
# command just in case somebody runs a file that represents a baseline as an
# actual input file.
trick.stop(0.0)
