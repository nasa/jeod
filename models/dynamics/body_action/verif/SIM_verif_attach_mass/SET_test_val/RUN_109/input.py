exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_inertia_optionB()
parent_mass_orientation_optionA()
parent_mass_points_1()
components.parent_init.points[0].set_name("front_to_back")
components.parent_init.points[0].position  = [ -0.5, 0.0, 0.0]
components.parent_init.points[0].pt_orientation.trans = [[ -1.0,  0.0, 0.0],
                                                         [  0.0, -1.0, 0.0],
                                                         [  0.0,  0.0, 1.0]]

child1_mass_default()
child1_mass_inertia_optionC()
child1_mass_points_1A()
components.child1_init.points[0].set_name("back_to_front")
components.child1_init.points[0].position  = [ 0.5, 0.0, 0.0]
components.child1_init.points[0].pt_orientation.trans = [[ 1.0, 0.0, 0.0],
                                                         [ 0.0, 1.0, 0.0],
                                                         [ 0.0, 0.0, 1.0]]

pt_attach1_default()
components.pt_attach1.subject_point_name = "back_to_front"
components.pt_attach1.parent_point_name  = "front_to_back"


components.file_name = "SET_test/RUN_109/mass.out"
