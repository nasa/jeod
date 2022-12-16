#Build on RUN_01, adding 2nd child body and attachment to parent.
exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
child1_mass_default()

attach1_default()
components.attach1.offset_pstr_cstr_pstr  = [ 0.0, 1.0, 0.0]

child2_mass_default()

attach2_default()
components.attach2.offset_pstr_cstr_pstr  = [ 0.0, -1.0, 0.0]

components.file_name = "SET_test/RUN_02/mass.out"
