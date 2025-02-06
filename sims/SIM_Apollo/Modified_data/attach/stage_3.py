################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_stage_3
####################################################################################

#Auto-generated from the data file with this header:
#// Saturn V stage 3 attachment points.

def set_stage_3(sv_dyn_reference) :

  #/* Set the mass point interfaces for this vehicle. */
  sv_dyn_reference.mass_init.set_subject_body( sv_dyn_reference.dyn_body.mass )

  #/* Allocate the mass point initilization objects. */
  sv_dyn_reference.mass_init.num_points = 2
  sv_dyn_reference.mass_init.points = trick.sim_services.alloc_type(sv_dyn_reference.mass_init.num_points, "jeod::MassPointInit")

  #/* Set the mass point interface position and orientation. */
  sv_dyn_reference.mass_init.points[0].set_name("Stage 2 interface")
  sv_dyn_reference.mass_init.points[0].pt_frame_spec    = trick.MassPointInit.StructToBody
  sv_dyn_reference.mass_init.points[0].position  = trick.attach_units( "ft",[ 0.0, 0.0, 0.0])
  sv_dyn_reference.mass_init.points[0].pt_orientation.data_source     =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.points[0].pt_orientation.eigen_angle  = trick.attach_units( "degree",180.0)
  sv_dyn_reference.mass_init.points[0].pt_orientation.eigen_axis    = [ 0.0, 0.0, 1.0]

  sv_dyn_reference.mass_init.points[1].set_name("LEM/SM/CM interface")
  sv_dyn_reference.mass_init.points[1].pt_frame_spec    = trick.MassPointInit.StructToBody
  sv_dyn_reference.mass_init.points[1].position  = trick.attach_units( "ft",[ 61.3, 0.0, 0.0])
  sv_dyn_reference.mass_init.points[1].pt_orientation.data_source     =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.points[1].pt_orientation.eigen_angle  = 0.0
  sv_dyn_reference.mass_init.points[1].pt_orientation.eigen_axis   = [ 0.0, 0.0, 1.0]
