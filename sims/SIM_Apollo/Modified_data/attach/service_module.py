################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_service_module
####################################################################################

#Auto-generated from the data file with this header:
#// Apollo Service Module attachment points.

def set_service_module(sv_dyn_reference) :

  #/* Set the mass point interfaces for this vehicle. */
  sv_dyn_reference.mass_init.subject = sv_dyn_reference.dyn_body.mass

  #/* Allocate the mass point initilization objects. */
  sv_dyn_reference.mass_init.num_points = 2
  sv_dyn_reference.mass_init.points = trick.sim_services.alloc_type(sv_dyn_reference.mass_init.num_points, "jeod::MassPointInit")

  #/* Set the mass point interface position and orientation. */
  sv_dyn_reference.mass_init.points[0].set_name("Stage 3 interface")
  sv_dyn_reference.mass_init.points[0].pt_frame_spec    = trick.MassPointInit.StructToBody
  sv_dyn_reference.mass_init.points[0].position  = trick.attach_units( "ft",[ -20.9, 0.0, 0.0])
  sv_dyn_reference.mass_init.points[0].pt_orientation.data_source     =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.points[0].pt_orientation.eigen_angle  = trick.attach_units( "degree",180.0)
  sv_dyn_reference.mass_init.points[0].pt_orientation.eigen_axis    = [ 0.0, 0.0, 1.0]

  sv_dyn_reference.mass_init.points[1].set_name("CM interface")
  sv_dyn_reference.mass_init.points[1].pt_frame_spec    = trick.MassPointInit.StructToBody
  sv_dyn_reference.mass_init.points[1].position  = trick.attach_units( "ft",[ 24.6, 0.0, 0.0])
  sv_dyn_reference.mass_init.points[1].pt_orientation.data_source     =    trick.Orientation.InputEigenRotation


  sv_dyn_reference.mass_init.points[1].pt_orientation.eigen_angle  = 0.0
  sv_dyn_reference.mass_init.points[1].pt_orientation.eigen_axis   = [ 0.0, 0.0, 1.0]
