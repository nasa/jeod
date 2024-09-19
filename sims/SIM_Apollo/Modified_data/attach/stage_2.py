################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_stage_2
####################################################################################

#Auto-generated from the data file with this header:
#// Saturn V stage 2 attachment points.

def set_stage_2(sv_dyn_reference) :

  #/* Set the mass point interfaces for this vehicle. */
  sv_dyn_reference.mass_init.set_subject_body( sv_dyn_reference.dyn_body.mass )

  #/* Allocate the mass point initilization objects. */
  sv_dyn_reference.mass_init.allocate_points(2)


  #/* Set the mass point interface position and orientation. */
  sv_dyn_reference.mass_init.get_mass_point(0).set_name("Stage 1 interface")
  sv_dyn_reference.mass_init.get_mass_point(0).pt_frame_spec    = trick.MassPointInit.StructToBody
  sv_dyn_reference.mass_init.get_mass_point(0).position  = trick.attach_units( "ft",[ 0.0, 0.0, 0.0])
  sv_dyn_reference.mass_init.get_mass_point(0).pt_orientation.data_source     =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.get_mass_point(0).pt_orientation.eigen_angle  = trick.attach_units( "degree",180.0)
  sv_dyn_reference.mass_init.get_mass_point(0).pt_orientation.eigen_axis    = [ 0.0, 0.0, 1.0]

  sv_dyn_reference.mass_init.get_mass_point(1).set_name("Stage 3 interface")
  sv_dyn_reference.mass_init.get_mass_point(1).pt_frame_spec    = trick.MassPointInit.StructToBody
  sv_dyn_reference.mass_init.get_mass_point(1).position  = trick.attach_units( "ft",[ 81.5, 0.0, 0.0])
  sv_dyn_reference.mass_init.get_mass_point(1).pt_orientation.data_source     =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.get_mass_point(1).pt_orientation.eigen_angle  = 0.0
  sv_dyn_reference.mass_init.get_mass_point(1).pt_orientation.eigen_axis   = [ 0.0, 0.0, 1.0]
