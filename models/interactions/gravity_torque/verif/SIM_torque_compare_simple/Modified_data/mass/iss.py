################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_mass_iss
####################################################################################

#Auto-generated from the data file with this header:
#/*
# * Initialization data for ISS mass properties.
# */

def set_mass_iss(veh_obj_reference) :

  #/* Set the mass porperties for this vehicle. */
  veh_obj_reference.mass_init.set_subject_body( veh_obj_reference.body.mass )

  #/* Set the struct to body reference frame orientation. */
  veh_obj_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


  veh_obj_reference.mass_init.properties.pt_orientation.eigen_angle  = 0.0
  veh_obj_reference.mass_init.properties.pt_orientation.eigen_axis    = [ 0.0, 1.0, 0.0]

  #/* Set the vehicle base mass. */
  veh_obj_reference.mass_init.properties.mass              = 400000.0
  veh_obj_reference.mass_init.properties.position        = [ -3.0, -1.5, 4.0]
  veh_obj_reference.mass_init.properties.inertia[0]  = [  1.02e+8,-6.96e+6,-5.48e+6]
  veh_obj_reference.mass_init.properties.inertia[1]  = [ -6.96e+6, 0.91e+8, 5.90e+5]
  veh_obj_reference.mass_init.properties.inertia[2]  = [ -5.48e+6, 5.90e+5, 1.64e+8]
