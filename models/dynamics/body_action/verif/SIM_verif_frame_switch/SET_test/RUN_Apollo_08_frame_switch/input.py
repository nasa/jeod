exec(compile(open("SET_test/RUN_Apollo_08_ECI_integ/input.py", "rb").read(), "SET_test/RUN_Apollo_08_ECI_integ/input.py", 'exec'))

# Configure and enable the frame switch.
veh.frame_switch.action_name = "frame_switch"
veh.frame_switch.integ_frame_name = "Moon.inertial"
veh.frame_switch.set_subject_body( veh.dyn_body )
veh.frame_switch.switch_sense = trick.DynBodyFrameSwitch.SwitchOnApproach
veh.frame_switch.switch_distance  = 66.1e6
veh.frame_switch.active = True
dynamics.dyn_manager.add_body_action (veh.frame_switch)
