################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_mass_service_module 
####################################################################################

#Auto-generated from the data file with this header:
#// Apollo Service Module mass properties

def set_mass_service_module(sv_dyn_reference) :
  #/* The Service Module is modeled as a simple cylinder of dimension:
  # *    Length   = 24.6 ft
  # *    Diameter = 12.8 ft
  # */

  #/*
  # * Initialization data for Apollo Service Module (SM) mass properties.
  # */


  #/* Set the mass properties for this vehicle. */
  sv_dyn_reference.mass_init.set_subject_body( sv_dyn_reference.dyn_body.mass )

  #/* Set the struct to body reference frame orientation. */
  sv_dyn_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation

  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_angle  = 0.0
  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_axis   = [ 0.0, 0.0, 1.0]

  #/* Set the vehicle base mass. */
  sv_dyn_reference.mass_init.properties.mass        = trick.attach_units( "lb",54064)
  sv_dyn_reference.mass_init.properties.position    = trick.attach_units( "ft",[ 12.3, 0.0, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[0]  = trick.attach_units( "lb*ft2",[ 1107231, 0.0, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[1]  = trick.attach_units( "lb*ft2",[ 0.0, 1235227, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[2]  = trick.attach_units( "lb*ft2",[ 0.0, 0.0, 1235227])
