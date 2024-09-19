exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_points_1()
components.parent_init.get_mass_point(0).set_name("left_to_right")
components.parent_init.get_mass_point(0).position  = [ 0.0, 0.0, -1.0]
components.parent_init.get_mass_point(0).pt_orientation.trans = [[ 0.0, 0.0, -1.0],
                                                         [ 0.0, 1.0,  0.0],
                                                         [ 1.0, 0.0,  0.0]]
child1_mass_default()
child1_mass_points_2()

child2_mass_default()
child2_mass_points_1A()

pt_attach1_default()
components.pt_attach1.subject_point_name = "right_to_left"
components.pt_attach1.parent_point_name  = "left_to_right"


pt_attach2_default()
components.pt_attach2.set_parent_body( components.child1_body )
components.pt_attach2.subject_point_name = "right_to_left"
components.pt_attach2.parent_point_name  = "left_to_right"

components.file_name = "SET_test/RUN_104/mass.out"
