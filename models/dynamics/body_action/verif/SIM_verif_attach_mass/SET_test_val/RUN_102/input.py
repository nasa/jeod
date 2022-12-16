exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_points_2()

child1_mass_default()
child1_mass_points_1B()

child2_mass_default()
child2_mass_points_1B()

pt_attach1_default()
components.pt_attach1.subject_point_name = "bottom_to_top"
components.pt_attach1.parent_point_name  = "top_to_bottom"

pt_attach2_default()
components.pt_attach2.subject_point_name = "top_to_bottom"
components.pt_attach2.parent_point_name  = "bottom_to_top"

components.file_name = "SET_test/RUN_102/mass.out"
