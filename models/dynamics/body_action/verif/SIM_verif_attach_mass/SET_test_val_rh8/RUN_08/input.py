# Add 3rd child body and attachment to parent.
exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_inertia_optionC()

child1_mass_default()
child1_mass_inertia_optionC()

child2_mass_default()
child2_mass_inertia_optionA()

child3_mass_default()

attach1_default()
attach1_optionB()

attach2_default()
attach2_optionA()

attach3_default()
attach3_optionA()

components.file_name = "SET_test/RUN_08/mass.out"
