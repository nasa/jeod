exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_inertia_optionB()
parent_mass_points_1()
components.parent_init.get_mass_point(0).set_name("right_to_left")
components.parent_init.get_mass_point(0).position  = [ 0.0, 0.0, 1.0]
components.parent_init.get_mass_point(0).pt_orientation.trans = [[ 0.0,  0.0, 1.0],
                                                         [ 0.0, -1.0, 0.0],
                                                         [ 1.0,  0.0, 0.0]]

child1_mass_default()
child1_mass_inertia_optionA()
child1_mass_points_1A()
components.child1_init.get_mass_point(0).set_name("left_to_right")
components.child1_init.get_mass_point(0).position  = [ 0.0, 0.0, -1.0]
components.child1_init.get_mass_point(0).pt_orientation.trans = [[ 0.0, 0.0, -1.0],
                                                         [ 0.0, 1.0,  0.0],
                                                         [ 1.0, 0.0,  0.0]]

pt_attach1_default()
components.pt_attach1.subject_point_name = "left_to_right"
components.pt_attach1.parent_point_name  = "right_to_left"

components.file_name = "SET_test/RUN_106/mass.out"
