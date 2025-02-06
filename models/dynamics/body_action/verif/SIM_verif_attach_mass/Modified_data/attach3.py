def attach3_default():
  components.attach3.action_name = "attach3"
  components.attach3.set_subject_body( components.child3_body )
  components.attach3.set_parent_body( components.parent_body )
  components.attach3.offset_pstr_cstr_pstr  = [ 0.0, 0.0, 0.0]
  components.attach3.pstr_cstr.data_source =    trick.Orientation.InputQuaternion

  dynamics.dyn_manager.add_body_action (components.attach3)

def attach3_optionA():
  components.attach3.offset_pstr_cstr_pstr  = [ -0.5, 0.0, 1.5]
  components.attach3.pstr_cstr.data_source =    trick.Orientation.InputEulerRotation
  components.attach3.pstr_cstr.euler_sequence = trick.Orientation.Pitch_Yaw_Roll
  components.attach3.pstr_cstr.euler_angles  = trick.attach_units( "degree",[ -90.0, 0.0, 0.0])


def pt_attach3_default():
  components.pt_attach3.action_name = "pt_attach3"
  components.pt_attach3.set_subject_body( components.child3_body )
  components.pt_attach3.set_parent_body( components.parent_body )
  components.pt_attach3.active      = True

  dynamics.dyn_manager.add_body_action (components.pt_attach3)
