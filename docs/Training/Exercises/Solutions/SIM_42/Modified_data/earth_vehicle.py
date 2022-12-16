# * Configure the vehicle.
#/* Set the vehicle name. */
earth_vehicle.dyn_body.set_name("ISS")

#/* Configure vehicle integration information. */
earth_vehicle.dyn_body.integ_frame_name = "Earth.inertial"
earth_vehicle.dyn_body.translational_dynamics = True
earth_vehicle.dyn_body.rotational_dynamics = True

exec(compile(open( "Modified_data/earth_vehicle/gravity.py", "rb").read(), "Modified_data/earth_vehicle/gravity.py", 'exec'))
exec(compile(open( "Modified_data/earth_vehicle/state.py", "rb").read(), "Modified_data/earth_vehicle/state.py", 'exec'))
exec(compile(open( "Modified_data/earth_vehicle/mass.py", "rb").read(), "Modified_data/earth_vehicle/mass.py", 'exec'))
