################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_mass_stage_3 
####################################################################################

#Auto-generated from the data file with this header:
#// Saturn V Stage 2 with Interstage mass properties

def set_mass_stage_3(sv_dyn_reference) :
  #/* The second stage is modeled as a simple cylinder of dimension:
  # *    Length   = 81.5 ft
  # *    Diameter = 33.0 ft
  # */

  #/*
  # * Initialization data for Saturn V third stage mass properties.
  # */

  #/* Set the mass properties for this vehicle. */
  sv_dyn_reference.mass_init.set_subject_body( sv_dyn_reference.dyn_body.mass )

  #/* Set the struct to body reference frame orientation. */
  sv_dyn_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation

  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_angle  = 0.0
  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_axis   = [ 0.0, 0.0, 1.0]

  #/* Set the vehicle base mass. */
  sv_dyn_reference.mass_init.properties.mass        = trick.attach_units( "lb",274171)
  sv_dyn_reference.mass_init.properties.position    = trick.attach_units( "ft",[ 30.65, 0.0, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[0]  = trick.attach_units( "lb*ft2",[ 16138048, 0.0, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[1]  = trick.attach_units( "lb*ft2",[ 0.0, 29532558, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[2]  = trick.attach_units( "lb*ft2",[ 0.0, 0.0, 29532558])
