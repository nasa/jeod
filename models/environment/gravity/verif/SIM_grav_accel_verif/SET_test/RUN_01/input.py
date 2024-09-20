from math import *

trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager.deriv_ephem_update = False
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

# Log Data
exec(compile(open( "Log_data/log_data.py", "rb").read(), "Log_data/log_data.py", 'exec'))

#---------------------------------------------
# Configure the vehicle.
#---------------------------------------------

# Set the vehicle name.
vehicle.dyn_body.set_name("iss")

# Configure vehicle integration information.
vehicle.dyn_body.integ_frame_name = "Earth.inertial"
vehicle.dyn_body.translational_dynamics = False
vehicle.dyn_body.rotational_dynamics = False

# Set up the gravity controls for the Earth/
vehicle.grav_control.source_name     = "Earth"
vehicle.grav_control.active          = True
vehicle.grav_control.spherical       = False
vehicle.grav_control.degree          = 5
vehicle.grav_control.order           = 5
vehicle.grav_control.gradient        = True
vehicle.grav_control.gradient_degree = 5
vehicle.grav_control.gradient_order  = 5
vehicle.grav_control.perturbing_only = False

vehicle.dyn_body.grav_interaction.add_control(vehicle.grav_control)

# Configuration vehicle mass properties.
exec(compile(open( "Modified_data/mass.py", "rb").read(), "Modified_data/mass.py", 'exec'))

# Configuration vehicle initial state.
exec(compile(open( "Modified_data/state.py", "rb").read(), "Modified_data/state.py", 'exec'))


def new_state( lat, lon, r):
  lat_rad = radians(lat)
  lon_rad = radians(lon)
  vehicle.dyn_body.composite_body.state.trans.position = \
                                   [r*cos(lat_rad)*cos(lon_rad), \
                                    r*cos(lat_rad)*sin(lon_rad), \
                                    r*sin(lat_rad)]
  vehicle.dyn_body.composite_body.state.trans.velocity = [ 1.0, 1.0, 1.0]
  time = jeod_time.time_manager.dyn_time.seconds
  print("\n\nSIM_TIME {0:19.16f} {1}".format(float(time),time.units))

trick.add_read( 1.0, "new_state( lat =  90, lon =   0, r = 6778000.0)")
trick.add_read( 2.0, "new_state( lat =  73, lon =   9, r = 6778000.0)")
trick.add_read( 3.0, "new_state( lat =  13, lon = 100, r = 6778000.0)")
trick.add_read( 4.0, "new_state( lat =   0, lon =   0, r = 6778000.0)")
trick.add_read( 5.0, "new_state( lat =   0, lon = 180, r = 6778000.0)")
trick.add_read( 6.0, "new_state( lat =   0, lon = 270, r = 6778000.0)")
trick.add_read( 7.0, "new_state( lat = -26, lon = 210, r = 6778000.0)")
trick.add_read( 8.0, "new_state( lat = -87, lon = 350, r = 6778000.0)")
trick.add_read( 9.0, "new_state( lat = -90, lon = 180, r = 6778000.0)")
trick.add_read(10.0, "new_state( lat = -90, lon = 160, r = 6800000.0)")

trick.stop(10.1)
