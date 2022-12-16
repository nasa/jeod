############################## TRICK HEADER ########################################
# PURPOSE:
#  (To define the method set_vehicle_state
####################################################################################

def set_vehicle_state(vehicle, dynamics) :


  # State initialization data for a state.

  vehicle.trans_init.dyn_subject = vehicle.body
  vehicle.trans_init.reference_ref_frame_name = "ref_planet.inertial"
  vehicle.trans_init.body_frame_id     = "composite_body"
  vehicle.trans_init.position  = [ 10.0, 0.0, 0.0]
  vehicle.trans_init.velocity  = [ 2.0, 1.0, 0.0]


  dynamics.manager.add_body_action(vehicle.trans_init)


  vehicle.rot_init.dyn_subject = vehicle.body
  vehicle.rot_init.reference_ref_frame_name = "ref_planet.inertial"
  vehicle.rot_init.body_frame_id     = "composite_body"
  vehicle.rot_init.orientation.data_source    = trick.Orientation.InputEulerRotation
  vehicle.rot_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
  vehicle.rot_init.orientation.euler_angles   = trick.attach_units( "degree",[ 90.0, 0.0, 0.0])
  vehicle.rot_init.ang_velocity               = trick.attach_units( "degree/s",[ 0.0, 9.0, 0.0])


  dynamics.manager.add_body_action(vehicle.rot_init)
