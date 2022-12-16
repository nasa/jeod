# RUN_01: parent and attached child
exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()

child1_mass_default()

attach1_default()
attach1_optionA()

components.file_name = "SET_test/RUN_01/mass.out"
