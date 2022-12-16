exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_inertia_optionC()
parent_mass_points_1()
components.parent_init.points[0].set_name("front_center_right")
components.parent_init.points[0].position  = [ -0.5, 0.0, 1.0]
components.parent_init.points[0].pt_orientation.trans = [[  0.0, 0.0, 1.0],
                                                         [  0.0, 1.0, 0.0],
                                                         [ -1.0, 0.0, 0.0]]


child1_mass_default()
child1_mass_inertia_optionC()
child1_mass_points_1A()
components.child1_init.points[0].set_name("back_center_right")
components.child1_init.points[0].position  = [ 0.5, 0.0, 1.0]
components.child1_init.points[0].pt_orientation.trans = [[  0.0, 0.0, 1.0],
                                                         [  0.0, 1.0, 0.0],
                                                         [ -1.0, 0.0, 0.0]]

child2_mass_default()
child2_mass_inertia_optionA()
child2_mass_points_2()

child3_mass_default()
child3_mass_points_2()

pt_attach1_default()
components.pt_attach1.subject     = components.child2_body
components.pt_attach1.parent      = components.child1_body
components.pt_attach1.subject_point_name = "ghost_front_to_back"
components.pt_attach1.parent_point_name  = "back_center_right"

pt_attach2_default()
components.pt_attach2.subject     = components.child3_body
components.pt_attach2.parent      = components.child2_body
components.pt_attach2.subject_point_name = "back_to_front"
components.pt_attach2.parent_point_name  = "front_to_back"

pt_attach3_default()
components.pt_attach3.subject_point_name = "front_to_back"
components.pt_attach3.parent_point_name  = "front_center_right"


components.file_name = "SET_test/RUN_108/mass.out"
