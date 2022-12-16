exec(compile(open( "Log_data/log_data.py", "rb").read(), "Log_data/log_data.py", 'exec'))

# Setup general numerical integration parameters.
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
dynamics.dyn_manager_init.central_point_name = "Earth"


#-----------------------
# Configure the vehicle.
#-----------------------
vehicle.dyn_body.set_name("iss")

vehicle.dyn_body.integ_frame_name = "Earth.inertial"
vehicle.dyn_body.translational_dynamics = False
vehicle.dyn_body.rotational_dynamics = False

# Set up the gravity controls for the Earth
vehicle.grav_control.source_name     = "Earth"
vehicle.grav_control.active          = True
vehicle.grav_control.spherical       = False
vehicle.grav_control.degree          = 70
vehicle.grav_control.order           = 70
vehicle.grav_control.gradient        = False

vehicle.dyn_body.grav_interaction.add_control(vehicle.grav_control)

exec(compile(open( "Modified_data/mass.py", "rb").read(), "Modified_data/mass.py", 'exec'))

x_pos = 7218634.798289895
y_pos = 18998.64159785956
z_pos = 1938152.473366886

# Configuration vehicle initial state.
exec(compile(open( "Modified_data/iss_typical_state.py", "rb").read(), "Modified_data/iss_typical_state.py", 'exec'))

trick.add_read(1.0, """
vehicle.dyn_body.composite_body.state.trans.position = [x_pos, y_pos, z_pos]
""")

trick.add_read(2.0, """
vehicle.dyn_body.composite_body.state.trans.position = [-x_pos, y_pos, z_pos]
""")

trick.add_read(3.0, """
vehicle.dyn_body.composite_body.state.trans.position = [-x_pos, -y_pos, z_pos]
""")

trick.add_read(4.0, """
vehicle.dyn_body.composite_body.state.trans.position = [-x_pos, -y_pos, -z_pos]
""")

trick.add_read(5.0, """
vehicle.dyn_body.composite_body.state.trans.position = [-x_pos, y_pos, -z_pos]
""")

trick.stop(5.1)
