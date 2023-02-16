################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_mass_les 
####################################################################################

#Auto-generated from the data file with this header:
#// Apollo Launch Escape System mass properties

def set_mass_les(sv_dyn_reference) :
  #/* The Lunar Escape System is modeled as a simple cylinder of dimension:
  # *    Length   = 32.5 ft
  # *    Diameter =  2.2 ft
  # */

  #/*
  # * Initialization data for Apollo Launch Escape System (LES) mass properties.
  # */


  #/* Set the mass properties for this vehicle. */
  sv_dyn_reference.mass_init.set_subject_body( sv_dyn_reference.dyn_body.mass )

  #/* Set the struct to body reference frame orientation. */
  sv_dyn_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_angle  = trick.attach_units( "degree",180.0)
  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_axis    = [ 0.0, 0.0, 1.0]

  #/* Set the vehicle base mass. */
  sv_dyn_reference.mass_init.properties.mass        = trick.attach_units( "lb",9200)
  sv_dyn_reference.mass_init.properties.position    = trick.attach_units( "ft",[ 16.25, 0.0, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[0]  = trick.attach_units( "lb*ft2",[ 5566, 0.0, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[1]  = trick.attach_units( "lb*ft2",[ 0.0, 205231, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[2]  = trick.attach_units( "lb*ft2",[ 0.0, 0.0, 205231])
