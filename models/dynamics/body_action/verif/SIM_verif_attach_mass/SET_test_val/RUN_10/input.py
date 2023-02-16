exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_inertia_optionB()

child1_mass_default()
child1_mass_inertia_optionC()

child2_mass_default()
child2_mass_inertia_optionB()

child3_mass_default()

attach1_default()
components.attach1.offset_pstr_cstr_pstr = [ -1.0, 0.0, 0.0]

attach2_default()
components.attach2.offset_pstr_cstr_pstr = [ 2.5, 0.0, 1.0]
components.attach2.pstr_cstr.data_source = \
                          trick.Orientation.InputEulerRotation
components.attach2.pstr_cstr.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
components.attach2.pstr_cstr.euler_angles  = \
                                   trick.attach_units( "degree",[ 0.0, 180.0, 0.0])


attach3_default()
components.attach3.offset_pstr_cstr_pstr = [ 1.0, 0.0, 0.0]

#/* Detach child2 from parent */
components.detach.action_name = "detach"
components.detach.set_subject_body( components.child2_body )
components.detach.active    = False
dynamics.dyn_manager.add_body_action (components.detach)

components.file_name = "SET_test/RUN_10/mass.out"

trick.add_read(1, "components.detach.active    = True")

trick.stop(2)
