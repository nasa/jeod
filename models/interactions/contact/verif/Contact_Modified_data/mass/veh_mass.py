################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_mass_veh_mass 
####################################################################################

#Auto-generated from the data file with this header:
#// Initialization data for vehicle mass properties

def set_mass_veh_mass(sv_dyn_reference) :
  #// Initialization data for vehicle mass properties
  #// In this case, we model a 1m radius, 100kg solid sphere.
  #// The X-axis is the long axis.

  #/* Set the mass porperties for this vehicle. */
  sv_dyn_reference.mass_init.subject = sv_dyn_reference.body.mass

  #/* Set the struct to body reference frame orientation. */
  sv_dyn_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_angle  = 0.0
  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_axis    = [ 1.0, 0.0, 0.0]

  #/* Set the vehicle base mass. */
  #// Total mass of this body.
  sv_dyn_reference.mass_init.properties.mass              = 100.0

  #// Location of the CM in the structural reference frame.
  sv_dyn_reference.mass_init.properties.position        = [ 0.0, 0.0, 0.0]

  #// Initialization options for specifying the inertia matrix.
  sv_dyn_reference.mass_init.properties.inertia_spec          = trick.MassPropertiesInit.Body
  sv_dyn_reference.mass_init.properties.inertia_offset  = [ 0.0, 0.0, 0.0]


  #/* Inertia matrix in specified initialization frame. */
  sv_dyn_reference.mass_init.properties.inertia[0]  = [ 40.0,   0.0,    0.0]
  sv_dyn_reference.mass_init.properties.inertia[1]  = [  0.0,  40.0,    0.0]
  sv_dyn_reference.mass_init.properties.inertia[2]  = [  0.0,   0.0,   40.0]
