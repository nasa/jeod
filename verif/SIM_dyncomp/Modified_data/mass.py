def set_mass_iss() :
  # Set the vehicle base vehicle.mass.
  vehicle.mass_init.properties.mass        = 400000.0
  vehicle.mass_init.properties.position    = [ -3.0, -1.5, 4.0]
  vehicle.mass_init.properties.inertia[0]  = [  1.02e+8,-6.96e+6,-5.48e+6]
  vehicle.mass_init.properties.inertia[1]  = [ -6.96e+6, 0.91e+8, 5.90e+5]
  vehicle.mass_init.properties.inertia[2]  = [ -5.48e+6, 5.90e+5, 1.64e+8]

def set_mass_cylinder() :
  # In this case, we model a 12m long, 1m radius, 1000kg solid cylinder.
  # The X-axis is the long axis.
  vehicle.mass_init.properties.mass        = 1000.0
  vehicle.mass_init.properties.position    = [ 6.0, 0.0, 0.0]
  vehicle.mass_init.properties.inertia[0]  = [ 500.0,     0.0,     0.0]
  vehicle.mass_init.properties.inertia[1]  = [   0.0, 12250.0,     0.0]
  vehicle.mass_init.properties.inertia[2]  = [   0.0,     0.0, 12250.0]

def set_mass_sphere() :
  # Models a sphere with radius = 1.0 m and mass = 1.0 kg
  vehicle.mass_init.properties.mass        = 1.0
  vehicle.mass_init.properties.position    = [ 0.0, 0.0, 0.0]
  vehicle.mass_init.properties.inertia[0]  = [ 0.4, 0.0, 0.0]
  vehicle.mass_init.properties.inertia[1]  = [ 0.0, 0.4, 0.0]
  vehicle.mass_init.properties.inertia[2]  = [ 0.0, 0.0, 0.4]


# Set the vehicle.mass porperties for this vehicle.
vehicle.mass_init.subject = vehicle.dyn_body.mass

# Set the struct to body reference frame orientation.
vehicle.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis  = [ 0.0, 1.0, 0.0]

# default to iss-mass-properties
set_mass_iss()

dynamics.dyn_manager.add_body_action( vehicle.mass_init)


