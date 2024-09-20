################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_vehicle_setup
####################################################################################

def set_vehicle_setup(vehicle_reference, options) :

  vehicle_reference.dyn_body.set_name( options["VEHICLE_NAME"] )
  vehicle_reference.dyn_body.integ_frame_name = "Earth.inertial"

  vehicle_reference.dyn_body.translational_dynamics = True
  vehicle_reference.dyn_body.rotational_dynamics = True

  vehicle_reference.earth_grav_ctrl.source_name   = "Earth"
  vehicle_reference.earth_grav_ctrl.active        = True
  vehicle_reference.earth_grav_ctrl.spherical     = True

  vehicle_reference.dyn_body.grav_interaction.add_control(
                                               vehicle_reference.earth_grav_ctrl)

  return
