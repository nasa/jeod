################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_mass_stage_1 
####################################################################################

#Auto-generated from the data file with this header:
#// Saturn V Stage 1 mass properties

def set_mass_stage_1(sv_dyn_reference) :
  #/* The first stage is modeled as a simple cylinder of dimension:
  # *    Length   = 138.0 ft
  # *    Diameter =  33.0 ft
  # */

  #/*
  # * Initialization data for Saturn V first stage mass properties.
  # */

  #/* Set the mass properties for this vehicle. */
  sv_dyn_reference.mass_init.set_subject_body( sv_dyn_reference.dyn_body.mass )

  #/* Set the struct to body reference frame orientation. */
  sv_dyn_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_angle  = 0.0
  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_axis   = [ 0.0, 0.0, 1.0]

  #/* Set the vehicle base mass. */
  sv_dyn_reference.mass_init.properties.mass        = trick.attach_units( "lb",5031023)
  sv_dyn_reference.mass_init.properties.position    = trick.attach_units( "ft",[ 69.0, 0.0, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[0]  = trick.attach_units( "lb*ft2",[ 684848006, 0.0, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[1]  = trick.attach_units( "lb*ft2",[ 0.0, 2338482378, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[2]  = trick.attach_units( "lb*ft2",[ 0.0, 0.0, 2338482378])
