################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_mass_sphere 
####################################################################################

#Auto-generated from the data file with this header:
#/*
# * Initialization data for ISS mass properties.
# */

def set_mass_sphere(veh_obj_reference) :
  # define M_PI        3.14159265358979323846

  #/* Set the mass porperties for this vehicle. */
  veh_obj_reference.mass_init.subject = veh_obj_reference.body.mass

  #/* Set the struct to body reference frame orientation. */
  veh_obj_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


  veh_obj_reference.mass_init.properties.pt_orientation.eigen_angle  = trick.attach_units( "degree",0.0)
  veh_obj_reference.mass_init.properties.pt_orientation.eigen_axis    = [ 0.0, 1.0, 0.0]

  #/* Set the vehicle base mass. */
  veh_obj_reference.mass_init.properties.mass              = trick.attach_units( "kg",1.0)
  veh_obj_reference.mass_init.properties.position        = trick.attach_units( "m",[ 0.0, 0.0, 0.0])
  veh_obj_reference.mass_init.properties.inertia[0]  = trick.attach_units( "kg*m2",[ 2/(5*M_PI), 0.0, 0.0])
  veh_obj_reference.mass_init.properties.inertia[1]  = trick.attach_units( "kg*m2",[ 0.0, 2/(5*M_PI), 0.0])
  veh_obj_reference.mass_init.properties.inertia[2]  = trick.attach_units( "kg*m2",[ 0.0, 0.0, 2/(5*M_PI)])


  return
