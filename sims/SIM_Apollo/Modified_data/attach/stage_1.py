################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_stage_1
####################################################################################

#Auto-generated from the data file with this header:
#// Saturn V stage 1 attachment points.

def set_stage_1(sv_dyn_reference) :

  #/* Set the mass point interfaces for this vehicle. */
  sv_dyn_reference.mass_init.set_subject_body( sv_dyn_reference.dyn_body.mass )

  #/* Allocate the mass point initilization objects. */
  sv_dyn_reference.mass_init.num_points = 1
  sv_dyn_reference.mass_init.points = trick.sim_services.alloc_type(sv_dyn_reference.mass_init.num_points, "jeod::MassPointInit")

  #/* Set the mass point interface position and orientation. */
  sv_dyn_reference.mass_init.points[0].set_name("Stage 2 interface")
  sv_dyn_reference.mass_init.points[0].pt_frame_spec    = trick.MassPointInit.StructToBody
  sv_dyn_reference.mass_init.points[0].position  = trick.attach_units( "ft",[ 138.0, 0.0, 0.0])
  sv_dyn_reference.mass_init.points[0].pt_orientation.data_source     =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.points[0].pt_orientation.eigen_angle  = 0.0
  sv_dyn_reference.mass_init.points[0].pt_orientation.eigen_axis   = [ 0.0, 0.0, 1.0]
