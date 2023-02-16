#//************************************

veh1.attach_to_2.action_name        = "attach_1_to_2"
veh1.attach_to_2.set_subject_body( veh1.dyn_body.mass )
veh1.attach_to_2.set_parent_body( veh2.dyn_body.mass )
veh1.attach_to_2.subject_point_name = "node12"
veh1.attach_to_2.parent_point_name  = "node21"
veh1.attach_to_2.active             = False

dynamics.dyn_manager.add_body_action (veh1.attach_to_2)


#***********************************
veh1.attach_to_2b.action_name        = "attach_1_to_2_b"
veh1.attach_to_2b.set_subject_body( veh1.dyn_body )
veh1.attach_to_2b.set_parent_body( veh2.dyn_body.mass )
veh1.attach_to_2b.subject_point_name = "node12"
veh1.attach_to_2b.parent_point_name  = "node21"
veh1.attach_to_2b.active             = False

dynamics.dyn_manager.add_body_action (veh1.attach_to_2b)


#//************************************
veh1.attach_to_3.action_name        = "attach_1_to_3"
veh1.attach_to_3.set_subject_body( veh1.dyn_body.mass )
veh1.attach_to_3.set_parent_body( veh3.dyn_body )
veh1.attach_to_3.subject_point_name = "node13"
veh1.attach_to_3.parent_point_name  = "node31"
veh1.attach_to_3.active             = False

dynamics.dyn_manager.add_body_action (veh1.attach_to_3)


#//************************************
veh1.detach_from_2.action_name = "detach_1_from_2"
veh1.detach_from_2.set_subject_body( veh1.dyn_body )
veh1.detach_from_2.active = False

dynamics.dyn_manager.add_body_action (veh1.detach_from_2)


#//************************************
veh1.detach_from_3.action_name = "detach_1_from_3_via_2"
veh1.detach_from_3.set_subject_body( veh1.dyn_body.mass )
veh1.detach_from_3.set_detach_from_body( veh3.dyn_body )
veh1.detach_from_3.active      = False

dynamics.dyn_manager.add_body_action (veh1.detach_from_3)
