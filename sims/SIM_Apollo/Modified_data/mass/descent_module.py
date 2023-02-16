################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_mass_descent_module 
####################################################################################

#Auto-generated from the data file with this header:
#// Apollo Lunar Module Descent Module mass properties

def set_mass_descent_module(sv_mass_reference) :

  #/*
  # * Initialization data for Apollo Lunar Module Descent Module mass properties.
  # */


  #/* Set the mass properties for this vehicle. */
  sv_mass_reference.mass_init.set_subject_body( sv_mass_reference.body )

  #/* Set the struct to body reference frame orientation. */
  sv_mass_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


  sv_mass_reference.mass_init.properties.pt_orientation.eigen_angle  = trick.attach_units( "degree",180.0)
  sv_mass_reference.mass_init.properties.pt_orientation.eigen_axis    = [ 0.0, 0.0, 1.0]

  #/* Set the vehicle base mass. */
  sv_mass_reference.mass_init.properties.mass        = trick.attach_units( "lb",25640)
  sv_mass_reference.mass_init.properties.position    = trick.attach_units( "ft",[ 5.0, 0.0, 0.0])
  sv_mass_reference.mass_init.properties.inertia[0]  = trick.attach_units( "lb*ft2",[ 628180, 0.0, 0.0])
  sv_mass_reference.mass_init.properties.inertia[1]  = trick.attach_units( "lb*ft2",[ 0.0, 367506, 0.0])
  sv_mass_reference.mass_init.properties.inertia[2]  = trick.attach_units( "lb*ft2",[ 0.0, 0.0, 367506])
