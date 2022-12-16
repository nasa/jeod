exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_inertia_optionB()
parent_mass_points_3()


child1_mass_default()
child1_mass_inertia_optionC()
child1_mass_points_1A()
components.child1_init.points[0].set_name("back_to_front")
components.child1_init.points[0].position  = [ 0.5, 0.0, 0.0]
components.child1_init.points[0].pt_orientation.trans = [[ 1.0, 0.0, 0.0],
                                                         [ 0.0, 1.0, 0.0],
                                                         [ 0.0, 0.0, 1.0]]

child2_mass_default()
components.child2_init.properties.position = [ 0.5, 0, 1.0]
child2_mass_inertia_optionB()
child2_mass_points_1A()
components.child2_init.points[0].set_name("child2_to_parent")
components.child2_init.points[0].position = [ 0.0, 0.0, 0.0]
components.child2_init.points[0].pt_orientation.trans = [[ 1.0, 0.0,  0.0],
                                                         [ 0.0, 0.0, -1.0],
                                                         [ 0.0, 1.0,  0.0]]


child3_mass_default()
child3_mass_points_1()


pt_attach1_default()
components.pt_attach1.subject_point_name = "back_to_front"
components.pt_attach1.parent_point_name  = "front_to_back"


pt_attach2_default()
components.pt_attach2.action_name = "pt_attach2"
components.pt_attach2.subject_point_name = "child2_to_parent"
components.pt_attach2.parent_point_name  = "parent_to_child2"

pt_attach3_default()
components.pt_attach3.subject_point_name = "front_to_back"
components.pt_attach3.parent_point_name  = "back_to_front"

components.file_name = "SET_test/RUN_110/mass.out"


#/* Detach child2 from parent */
components.detach.action_name = "detach"
components.detach.subject     = components.child2_body
components.detach.active    = False
dynamics.dyn_manager.add_body_action (components.detach)

trick.add_read(1, "components.detach.active    = True")

trick.stop(2)
