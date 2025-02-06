# Set the common mass properties for this vehicle.
dawn.mass_init.set_subject_body( dawn.dyn_body.mass )
dawn.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
dawn.mass_init.properties.pt_orientation.eigen_angle = 0.0
dawn.mass_init.properties.pt_orientation.eigen_axis  = [1.0, 0.0, 0.0]
dawn.mass_init.properties.position = [ 0.0, 0.0, 0.0]
dawn.mass_init.properties.inertia_spec    = trick.MassPropertiesInit.Body
dawn.mass_init.properties.inertia_offset  = [ 0.0, 0.0, 0.0]

dynamics.dyn_manager.add_body_action(dawn.mass_init)


def set_dawn_mass(): # Mass properties for vehicle
  # Total mass of this body.
  dawn.mass_init.properties.mass  = 1250.0 # kg

  # Inertia matrix in specified initialization frame. */
  dawn.mass_init.properties.inertia[0]  = [  1.0,     0.0,     0.0]
  dawn.mass_init.properties.inertia[1]  = [   0.0,    1.0,     0.0]
  dawn.mass_init.properties.inertia[2]  = [   0.0,     0.0,    1.0]


def set_phobos_mass(): # Mass properties for Phobos
  # Total mass of Phobos.
  dawn.mass_init.properties.mass            = 1.08e+16 # kg
  
  # Inertia matrix in specified initialization frame. kg * m2
  dawn.mass_init.properties.inertia[0]  = [  4.18e+23,     0.0,     0.0]
  dawn.mass_init.properties.inertia[1]  = [   0.0,    6.08e+23,     0.0]
  dawn.mass_init.properties.inertia[2]  = [   0.0,     0.0,    6.72e+23]
