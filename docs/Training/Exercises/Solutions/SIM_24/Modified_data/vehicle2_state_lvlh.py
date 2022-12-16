lvlh_trans = trick.DynBodyInitLvlhTransState()
lvlh_trans.thisown = 0

lvlh_trans.dyn_subject   = vehicle2.dyn_body
lvlh_trans.body_frame_id = "composite_body"
lvlh_trans.ref_body_name = "veh"
lvlh_trans.planet_name   = "Earth"
lvlh_trans.position      = [-200.0, 0.0, 0.0]
lvlh_trans.velocity      = [10.0, 0.0, 0.0]


lvlh_rot = trick.DynBodyInitLvlhRotState()
lvlh_rot.thisown = 0

lvlh_rot.dyn_subject    =  vehicle2.dyn_body
lvlh_rot.body_frame_id  = "composite_body"
lvlh_rot.planet_name    = "Earth"
lvlh_rot.orientation.data_source    = trick.Orientation.InputEulerRotation
lvlh_rot.orientation.euler_sequence = trick.sim_services.Yaw_Pitch_Roll
lvlh_rot.orientation.euler_angles= trick.sim_services.attach_units("degree",                            [0.0, 90.0, 0.0])
lvlh_rot.ang_velocity = [0.0, 0.0, 0.0]


dynamics.dyn_manager.add_body_action( lvlh_trans )
dynamics.dyn_manager.add_body_action( lvlh_rot )
