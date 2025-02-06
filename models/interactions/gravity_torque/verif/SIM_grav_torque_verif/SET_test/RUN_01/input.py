from math import *

trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager.deriv_ephem_update = False
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

#---------------------------------------------
# Set up general parameters to record.
#---------------------------------------------*/
exec(compile(open("Log_data/log_data.py", "rb").read(), "Log_data/log_data.py", 'exec'))

#---------------------------------------------
# Configure the vehicle.
#---------------------------------------------*/

# Set the vehicle name. */
vehicle.dyn_body.set_name("iss")

# Configure vehicle integration information. */
vehicle.dyn_body.integ_frame_name = "Earth.inertial"
vehicle.dyn_body.translational_dynamics = False
vehicle.dyn_body.rotational_dynamics = False

#*****************************************************************************/

# Set up the gravity controls for the Earth */
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


# Configuration vehicle mass properties. */
exec(compile(open("Modified_data/mass.py", "rb").read(), "Modified_data/mass.py", 'exec'))


# Configuration vehicle initial state. */
exec(compile(open("Modified_data/state.py", "rb").read(), "Modified_data/state.py", 'exec'))

# vehicle points, these represent a non-uniform mass distribution in the
# vehicle.
vehicle.grav_verif.vehicle_pt[0].mass = 50.0
vehicle.grav_verif.vehicle_pt[0].position = [1.625,  1.250, -1.750]
vehicle.grav_verif.vehicle_pt[1].mass = 20.0
vehicle.grav_verif.vehicle_pt[1].position = [-2.375, -1.750,  3.250]
vehicle.grav_verif.vehicle_pt[2].mass = 10.0
vehicle.grav_verif.vehicle_pt[2].position = [-3.375, -2.750,  2.250]


#  pitch/yaw/roll, deg (Euler sequence 2-3-1)
vehicle.attitude_PYR = [20.0, 30.0, 40.0]

vehicle.grav_torque.active = True

def new_state( lat, lon, r):
  lat_rad = radians(lat)
  lon_rad = radians(lon)
  vehicle.position = [r*cos(lat_rad)*cos(lon_rad), \
                      r*cos(lat_rad)*sin(lon_rad), \
                      r*sin(lat_rad)]
  print("\n**************\nSIM_TIME: %19.16f" % jeod_time.time_manager.dyn_time.seconds)
  print("Location: %19.6f %19.6f %19.6f" % tuple(vehicle.position) )

trick.add_read(0.0, "new_state( lat = 90, lon = 120, r = 6800000)")
trick.add_read(1.0, "new_state( lat = 90, lon = 120, r = 6800000)")
trick.add_read(2.0, "new_state( lat = 89.99, lon = 353.00, r   = 6800000.0)")
trick.add_read(3.0, "new_state( lat = 73.0, lon = 9.0, r   = 6800000.0)")
trick.add_read(4.0, "new_state( lat = 13.0, lon = 100.0, r   = 6800000.0)")
trick.add_read(5.0, "new_state( lat = 0.0, lon = 0.0, r   = 6800000.0)")
trick.add_read(6.0, "new_state( lat = 0.0, lon = 180.0, r   = 6800000.0)")
trick.add_read(7.0, "new_state( lat = -26.0, lon =  210.0, r   = 6800000.0)")
trick.add_read(8.0, "new_state( lat = -52.0, lon =  310.0, r   = 6800000.0)")
trick.add_read(9.0, "new_state( lat = -89.99, lon =  10.0, r   = 6800000.0)")
trick.add_read(10.0, "new_state( lat = -90.0, lon = 160.0, r   = 6800000.0)")

trick.stop(10.1)
