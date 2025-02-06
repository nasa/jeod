# Baseline sim with just parent body.
exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_inertia_optionA()
components.parent_init.properties.position  = [ 0.5, 0.0, 1.0]

components.file_name = "SET_test/RUN_05/mass.out"
