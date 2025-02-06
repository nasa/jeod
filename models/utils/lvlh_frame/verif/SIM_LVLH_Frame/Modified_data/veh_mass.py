############################## TRICK HEADER ########################################
# PURPOSE:
#  (To define the method set_vehicle_mass 
####################################################################################

# Initialization data for vehicle mass properties

def set_vehicle_mass(vehicle, dynamics) :

  # Set the mass properties for this vehicle.
  vehicle.mass_init.set_subject_body( vehicle.body.mass )

  # Set the struct to body reference frame orientation.
  vehicle.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


  vehicle.mass_init.properties.pt_orientation.eigen_angle  = 0.0
  vehicle.mass_init.properties.pt_orientation.eigen_axis    = [ 0.0, 1.0, 0.0]

  # Set the vehicle base mass.
  # Total mass of this body.
  vehicle.mass_init.properties.mass              = 1.0

  # Location of the CM in the structural reference frame.
  vehicle.mass_init.properties.position        = [ 0.0, 0.0, 0.0]

  # Inertia matrix in specified initialization frame.
  vehicle.mass_init.properties.inertia[0]  = [   1.0,     0.0,     0.0]
  vehicle.mass_init.properties.inertia[1]  = [   0.0,     1.0,     0.0]
  vehicle.mass_init.properties.inertia[2]  = [   0.0,     0.0,     1.0]

  dynamics.manager.add_body_action(vehicle.mass_init)
