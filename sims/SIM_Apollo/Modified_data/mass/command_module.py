################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_mass_command_module 
####################################################################################

#Auto-generated from the data file with this header:
#// Apollo Command Module mass properties

def set_mass_command_module(sv_dyn_reference) :
  #/* The command module is modeled as a cone of dimension:
  # *    Height   = 11.6 ft
  # *    Diameter = 12.8 ft
  # * The apex of the cone is the origin of the structural reference frame.
  # *            y
  # *            /
  # *           /
  # *          ^--------- z
  # *        / | \
  # *       /  |  \
  # *      /   |   \
  # *     <____|____>
  # *          | x
  # *
  # * However, the body frame of the vehicle is rotated about the Z-axis
  # * by 180 degrees.
  # */

  #/*
  # * Initialization data for Apollo Command Module (CM) mass properties.
  # */


  #/* Set the mass properties for this vehicle. */
  sv_dyn_reference.mass_init.subject = sv_dyn_reference.dyn_body.mass

  #/* Set the struct to body reference frame orientation. */
  sv_dyn_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_angle  = trick.attach_units( "degree",180.0)
  sv_dyn_reference.mass_init.properties.pt_orientation.eigen_axis    = [ 0.0, 0.0, 1.0]

  #/* Set the vehicle base mass. */
  sv_dyn_reference.mass_init.properties.mass        = trick.attach_units( "lb",12807)
  sv_dyn_reference.mass_init.properties.position    = trick.attach_units( "ft",[ 8.7, 0.0, 0.0])
  sv_dyn_reference.mass_init.properties.inertia[0]  = trick.attach_units( "lb*ft2",[ 157372,   0.0,   0.0])
  sv_dyn_reference.mass_init.properties.inertia[1]  = trick.attach_units( "lb*ft2",[    0.0, 64624,   0.0])
  sv_dyn_reference.mass_init.properties.inertia[2]  = trick.attach_units( "lb*ft2",[    0.0,   0.0, 64624])
