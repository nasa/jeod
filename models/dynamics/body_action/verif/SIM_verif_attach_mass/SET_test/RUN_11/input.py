exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_inertia_optionB()

child1_mass_default()
child1_mass_inertia_optionC()

child2_mass_default()
components.child2_init.properties.position  = [ 0.5, 0.0, 1.0]
child2_mass_inertia_optionB()

child3_mass_default()

attach1_default()
components.attach1.offset_pstr_cstr_pstr = [ 1.0, 0.0, 0.0]

attach2_default()
components.attach2.offset_pstr_cstr_pstr = [ 1.5, 0.0, -1.0]

#/* Re-attach child2 to parent */
components.reattach.action_name = "reattach"
components.reattach.set_subject_body( components.child2_body )
components.reattach.active    = False
components.reattach.offset_pstr_cstr_pstr = [ 1.5, 0.0, -2.0]
components.reattach.pstr_cstr.data_source = \
                                         trick.Orientation.InputEulerRotation
components.reattach.pstr_cstr.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
components.reattach.pstr_cstr.euler_angles  = \
                                    trick.attach_units( "degree",[ 0.0, -90.0, 0.0])
dynamics.dyn_manager.add_body_action (components.reattach)

#/* Mass output file */
components.file_name = "SET_test/RUN_11/mass.out"


trick.add_read(1, "components.reattach.active = True")

trick.stop(2)
