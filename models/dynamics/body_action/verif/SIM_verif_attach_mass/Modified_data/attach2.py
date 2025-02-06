def attach2_default():
  components.attach2.action_name = "attach2"
  components.attach2.set_subject_body( components.child2_body )
  components.attach2.set_parent_body( components.parent_body )
  components.attach2.offset_pstr_cstr_pstr  = [ 0.0, 0.0, 0.0]
  components.attach2.pstr_cstr.data_source =    trick.Orientation.InputQuaternion

  dynamics.dyn_manager.add_body_action (components.attach2)

def attach2_optionA():
  components.attach2.set_subject_body( components.child3_body )
  components.attach2.set_parent_body( components.child2_body )
  components.attach2.offset_pstr_cstr_pstr  = [ -1.0, 0.0, 0.0]


def pt_attach2_default():
  components.pt_attach2.action_name = "pt_attach2"
  components.pt_attach2.set_subject_body( components.child2_body )
  components.pt_attach2.set_parent_body( components.parent_body )
  components.pt_attach2.active      = True

  dynamics.dyn_manager.add_body_action (components.pt_attach2)
