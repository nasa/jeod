# * Configure the vehicle.
#/* Set the vehicle name. */
moon_vehicle.dyn_body.set_name("Clementine")

#/* Configure vehicle integration information. */
moon_vehicle.dyn_body.integ_frame_name = "Moon.inertial"
moon_vehicle.dyn_body.translational_dynamics = True
moon_vehicle.dyn_body.rotational_dynamics = True

exec(compile(open( "Modified_data/moon_vehicle/gravity.py", "rb").read(), "Modified_data/moon_vehicle/gravity.py", 'exec'))
exec(compile(open( "Modified_data/moon_vehicle/state.py", "rb").read(), "Modified_data/moon_vehicle/state.py", 'exec'))
exec(compile(open( "Modified_data/moon_vehicle/mass.py", "rb").read(), "Modified_data/moon_vehicle/mass.py", 'exec'))
