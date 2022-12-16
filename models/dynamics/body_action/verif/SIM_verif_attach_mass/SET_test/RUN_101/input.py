# Add a child and attaching it to the parent using pt-attach.
exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_points_1()

child1_mass_default()
child1_mass_points_1A()

pt_attach1_default()
components.pt_attach1.subject_point_name = "right_to_top"
components.pt_attach1.parent_point_name  = "top_to_right"

components.file_name = "SET_test/RUN_101/mass.out"
