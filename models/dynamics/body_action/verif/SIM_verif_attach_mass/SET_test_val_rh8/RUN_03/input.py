# Rerun RUN_02 with different attach points.
exec(compile(open("SET_test/RUN_02/input.py", "rb").read(), "SET_test/RUN_02/input.py", 'exec'))

# Overwrite attach child1 to parent data from RUN_02
components.attach1.offset_pstr_cstr_pstr  = [ 1.0, 0.0, 0.0]

# Overwrite attach child2 to parent data from RUN_02
components.attach2.set_parent_body( components.child1_body )
components.attach2.offset_pstr_cstr_pstr  = [ 1.0, 0.0, 0.0]

components.file_name = "SET_test/RUN_03/mass.out"
