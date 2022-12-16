################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_mass_cylinder 
####################################################################################

#Auto-generated from the data file with this header:
#// Initialization data for vehicle mass properties

def set_mass_cylinder(veh_obj_reference) :
  #// In this case, we model a 12m long, 1m radius, 1000kg solid cylinder.
  #// The X-axis is the long axis.

  #/* Set the mass porperties for this vehicle. */
  veh_obj_reference.mass_init.subject = veh_obj_reference.body.mass

  #/* Set the struct to body reference frame orientation. */
  veh_obj_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


  veh_obj_reference.mass_init.properties.pt_orientation.eigen_angle  = trick.attach_units( "degree",0.0)
  veh_obj_reference.mass_init.properties.pt_orientation.eigen_axis    = [ 0.0, 1.0, 0.0]

  #/* Set the vehicle base mass. */
  #// Total mass of this body.
  veh_obj_reference.mass_init.properties.mass              = trick.attach_units( "kg",1000.0)

  #// Location of the CM in the structural reference frame.
  veh_obj_reference.mass_init.properties.position        = trick.attach_units( "m",[ 6.0, 0.0, 0.0])

  #// Initialization options for specifying the inertia matrix.
  veh_obj_reference.mass_init.properties.inertia_spec          = trick.MassPropertiesInit.Body
  veh_obj_reference.mass_init.properties.inertia_offset  = trick.attach_units( "m",[ 0.0, 0.0, 0.0])

  #// Initialize the inertia matrix frame to body frame transformation.
  veh_obj_reference.mass_init.properties.inertia_orientation.data_source =    trick.Orientation.InputEigenRotation


  veh_obj_reference.mass_init.properties.inertia_orientation.eigen_angle  = trick.attach_units( "degree",0.0)
  veh_obj_reference.mass_init.properties.inertia_orientation.eigen_axis   = [ 0.0, 1.0, 0.0]

  #/* Inertia matrix in specified initialization frame. */
  veh_obj_reference.mass_init.properties.inertia[0]  = trick.attach_units( "kg*m2",[ 500.0,     0.0,     0.0])
  veh_obj_reference.mass_init.properties.inertia[1]  = trick.attach_units( "kg*m2",[   0.0, 12250.0,     0.0])
  veh_obj_reference.mass_init.properties.inertia[2]  = trick.attach_units( "kg*m2",[   0.0,     0.0, 12250.0])


  return
