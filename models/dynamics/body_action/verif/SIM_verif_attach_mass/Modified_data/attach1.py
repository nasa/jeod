def attach1_default():
  components.attach1.action_name = "attach1"
  components.attach1.set_subject_body( components.child1_body )
  components.attach1.set_parent_body( components.parent_body )
  components.attach1.offset_pstr_cstr_pstr  = [ 0.0, 0.0, 0.0]
  components.attach1.pstr_cstr.data_source =    trick.Orientation.InputQuaternion

  dynamics.dyn_manager.add_body_action (components.attach1)


def attach1_optionA():
  components.attach1.offset_pstr_cstr_pstr = [ 0.0, 1.5, 0.0]
  components.attach1.pstr_cstr.data_source = trick.Orientation.InputMatrix
  components.attach1.pstr_cstr.trans  = [[ 1.0,  0.0,  0.0],
                                         [ 0.0,  0.0,  1.0],
                                         [ 0.0, -1.0,  0.0]]

def attach1_optionB():
  components.attach1.set_subject_body( components.child2_body )
  components.attach1.set_parent_body( components.child1_body )
  components.attach1.offset_pstr_cstr_pstr = [ 0.5, 0.0, 2.5]
  components.attach1.pstr_cstr.data_source =trick.Orientation.InputEulerRotation
  components.attach1.pstr_cstr.euler_sequence = trick.Orientation.Pitch_Yaw_Roll
  components.attach1.pstr_cstr.euler_angles   = \
                                     trick.attach_units( "degree",[ -90.0, 0.0, 0.0])

def pt_attach1_default():
  components.pt_attach1.action_name = "pt_attach1"
  components.pt_attach1.set_subject_body( components.child1_body )
  components.pt_attach1.set_parent_body( components.parent_body )
  components.pt_attach1.active      = True

  dynamics.dyn_manager.add_body_action (components.pt_attach1)
