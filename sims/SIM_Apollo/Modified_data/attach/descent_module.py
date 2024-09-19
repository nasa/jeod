################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_descent_module
####################################################################################

#Auto-generated from the data file with this header:
#// Apollo Lunar Module Descent Module attachment points.

def set_descent_module(sv_mass_reference) :

  #/* Set the mass point interfaces for this vehicle. */
  sv_mass_reference.mass_init.set_subject_body( sv_mass_reference.body )

  #/* Allocate the mass point initilization objects. */
  sv_mass_reference.mass_init.allocate_points(2)


  #/* Set the mass point interface position and orientation. */
  sv_mass_reference.mass_init.get_mass_point(0).set_name("Ascent Module interface")
  sv_mass_reference.mass_init.get_mass_point(0).pt_frame_spec    = trick.MassPointInit.StructToBody
  sv_mass_reference.mass_init.get_mass_point(0).position  = trick.attach_units( "ft",[ 0.0, 0.0, 0.0])
  sv_mass_reference.mass_init.get_mass_point(0).pt_orientation.data_source     =    trick.Orientation.InputEigenRotation


  sv_mass_reference.mass_init.get_mass_point(0).pt_orientation.eigen_angle  = trick.attach_units( "degree",0.0)
  sv_mass_reference.mass_init.get_mass_point(0).pt_orientation.eigen_axis    = [ 0.0, 0.0, 1.0]

  sv_mass_reference.mass_init.get_mass_point(1).set_name("Stage 3 interface")
  sv_mass_reference.mass_init.get_mass_point(1).pt_frame_spec    = trick.MassPointInit.StructToBody
  sv_mass_reference.mass_init.get_mass_point(1).position  = trick.attach_units( "ft",[ -10.0, 0.0, 0.0])
  sv_mass_reference.mass_init.get_mass_point(1).pt_orientation.data_source     =    trick.Orientation.InputEigenRotation


  sv_mass_reference.mass_init.get_mass_point(1).pt_orientation.eigen_angle  = trick.attach_units( "degree",180.0)
  sv_mass_reference.mass_init.get_mass_point(1).pt_orientation.eigen_axis    = [ 0.0, 0.0, 1.0]
