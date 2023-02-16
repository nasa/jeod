exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

parent_mass_default()
parent_mass_inertia_optionB()
parent_mass_points_1()
components.parent_init.points[0].set_name("back_to_front")
components.parent_init.points[0].position  = [ 0.5, 0.0, 0.0]
components.parent_init.points[0].pt_orientation.trans = [[ 1.0, 0.0, 0.0],
                                                         [ 0.0, 1.0, 0.0],
                                                         [ 0.0, 0.0, 1.0]]

child1_mass_default()
child1_mass_inertia_optionC()
child1_mass_points_1A()
components.child1_init.points[0].set_name("front_to_back")
components.child1_init.points[0].position  = [ -0.5, 0.0, 0.0]
components.child1_init.points[0].pt_orientation.trans = [[ -1.0,  0.0, 0.0],
                                                         [  0.0, -1.0, 0.0],
                                                         [  0.0,  0.0, 1.0]]

child2_mass_default()
components.child2_init.properties.position = [ 0.5, 0, 1.0]
child2_mass_inertia_optionB()


pt_attach1_default()
components.pt_attach1.subject_point_name = "front_to_back"
components.pt_attach1.parent_point_name  = "back_to_front"


attach2_default()
#/* Attach child2 to parent */
components.attach2.action_name = "attach2"
components.attach2.offset_pstr_cstr_pstr  = [ 1.5, 0.0, -1.0]



#/* Re-attach child2 to parent */
components.reattach.action_name = "reattach"
components.reattach.set_subject_body( components.child2_body )
components.reattach.active    = False
components.reattach.offset_pstr_cstr_pstr = [ 1.5, 0.0, -2.0]
components.reattach.pstr_cstr.data_source = trick.Orientation.InputEulerRotation
components.reattach.pstr_cstr.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
components.reattach.pstr_cstr.euler_angles  = \
                                     trick.attach_units( "degree",[ 0.0, -90.0, 0.0])

dynamics.dyn_manager.add_body_action (components.reattach)

components.file_name = "SET_test/RUN_111/mass.out"

trick.add_read(1, "components.reattach.active = True")

trick.stop(2)
