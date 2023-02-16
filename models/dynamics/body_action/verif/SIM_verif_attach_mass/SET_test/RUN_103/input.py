exec(compile(open("Modified_data/system.py", "rb").read(), "Modified_data/system.py", 'exec'))

#############  ASSIGNMENTS, CALLS, and CONTROL  #################
components.parent_body.set_name("Parent")
components.parent_init.set_subject_body( components.parent_body )
components.parent_init.action_name = "Parent_mass_init"
components.parent_init.properties.mass  = trick.attach_units( "kg",1.0)
components.parent_init.properties.position  = trick.attach_units( "m",[ 0.0, 0.0, 0.0])

components.parent_init.properties.inertia_spec = trick.MassPropertiesInit.Body
components.parent_init.properties.inertia[0]  = trick.attach_units( "kg*m2",[ 0.41666667, 0.0, 0.0])
components.parent_init.properties.inertia[1]  = trick.attach_units( "kg*m2",[ 0.0, 0.41666667, 0.0])
components.parent_init.properties.inertia[2]  = trick.attach_units( "kg*m2",[ 0.0, 0.0, 0.1666667])

components.parent_init.properties.pt_orientation.data_source =    trick.Orientation.InputMatrix


components.parent_init.properties.pt_orientation.trans[0]  = [ 1.0, 0.0, 0.0]
components.parent_init.properties.pt_orientation.trans[1]  = [ 0.0, 1.0, 0.0]
components.parent_init.properties.pt_orientation.trans[2]  = [ 0.0, 0.0, 1.0]

components.parent_init.num_points = 1
components.parent_init.points = trick.sim_services.alloc_type( 1 , "jeod::MassPointInit" )
components.parent_init.points[0].set_name("back_to_front")
components.parent_init.points[0].position  = trick.attach_units( "m",[ 0.5, 0.0, 0.0])
components.parent_init.points[0].pt_orientation.data_source =    trick.Orientation.InputEulerRotation


components.parent_init.points[0].pt_orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
components.parent_init.points[0].pt_orientation.euler_angles  = trick.attach_units( "degree",[    0.0, 0.0, 0.0])




#CALL
dynamics.dyn_manager.add_body_action (components.parent_init)



#/* Child1 body mass parameters */
components.child1_body.set_name("Child1")
components.child1_init.set_subject_body( components.child1_body )
components.child1_init.action_name = "Child1_mass_init"
components.child1_init.properties.mass  = trick.attach_units( "kg",1.0)
components.child1_init.properties.position  = trick.attach_units( "m",[ 0.0, 0.0, 0.0])

components.child1_init.properties.inertia_spec = trick.MassPropertiesInit.Body
components.child1_init.properties.inertia[0]  = trick.attach_units( "kg*m2",[ 0.41666667, 0.0, 0.0])
components.child1_init.properties.inertia[1]  = trick.attach_units( "kg*m2",[ 0.0, 0.41666667, 0.0])
components.child1_init.properties.inertia[2]  = trick.attach_units( "kg*m2",[ 0.0, 0.0, 0.1666667])

components.child1_init.properties.pt_orientation.data_source =    trick.Orientation.InputMatrix


components.child1_init.properties.pt_orientation.trans[0]  = [ 1.0, 0.0, 0.0]
components.child1_init.properties.pt_orientation.trans[1]  = [ 0.0, 1.0, 0.0]
components.child1_init.properties.pt_orientation.trans[2]  = [ 0.0, 0.0, 1.0]

components.child1_init.num_points = 2
components.child1_init.points = trick.sim_services.alloc_type( 2 , "jeod::MassPointInit" )
components.child1_init.points[0].set_name("front_to_back")
components.child1_init.points[0].position  = trick.attach_units( "m",[ -0.5, 0.0, 0.0])
components.child1_init.points[0].pt_orientation.data_source =    trick.Orientation.InputEulerRotation


components.child1_init.points[0].pt_orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
components.child1_init.points[0].pt_orientation.euler_angles  = trick.attach_units( "degree",[   180.0, 0.0, 0.0])


components.child1_init.points[1].set_name("back_to_front")
components.child1_init.points[1].position  = trick.attach_units( "m",[ 0.5, 0.0, 0.0])
components.child1_init.points[1].pt_orientation.data_source =    trick.Orientation.InputEulerRotation


components.child1_init.points[1].pt_orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
components.child1_init.points[1].pt_orientation.euler_angles  = trick.attach_units( "degree",[    0.0, 0.0, 0.0])




#CALL
dynamics.dyn_manager.add_body_action (components.child1_init)



#/* Child2 body mass parameters */
components.child2_body.set_name("Child2")
components.child2_init.set_subject_body( components.child2_body )
components.child2_init.action_name = "Child2_mass_init"
components.child2_init.properties.mass  = trick.attach_units( "kg",1.0)
components.child2_init.properties.position  = trick.attach_units( "m",[ 0.0, 0.0, 0.0])

components.child2_init.properties.inertia_spec = trick.MassPropertiesInit.Body
components.child2_init.properties.inertia[0]  = trick.attach_units( "kg*m2",[ 0.41666667, 0.0, 0.0])
components.child2_init.properties.inertia[1]  = trick.attach_units( "kg*m2",[ 0.0, 0.41666667, 0.0])
components.child2_init.properties.inertia[2]  = trick.attach_units( "kg*m2",[ 0.0, 0.0, 0.1666667])

components.child2_init.properties.pt_orientation.data_source =    trick.Orientation.InputMatrix


components.child2_init.properties.pt_orientation.trans[0]  = [ 1.0, 0.0, 0.0]
components.child2_init.properties.pt_orientation.trans[1]  = [ 0.0, 1.0, 0.0]
components.child2_init.properties.pt_orientation.trans[2]  = [ 0.0, 0.0, 1.0]

components.child2_init.num_points = 1
components.child2_init.points = trick.sim_services.alloc_type( 1 , "jeod::MassPointInit" )
components.child2_init.points[0].set_name("front_to_back")
components.child2_init.points[0].position  = trick.attach_units( "m",[ -0.5, 0.0, 0.0])
components.child2_init.points[0].pt_orientation.data_source =    trick.Orientation.InputEulerRotation


components.child2_init.points[0].pt_orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
components.child2_init.points[0].pt_orientation.euler_angles  = trick.attach_units( "degree",[    180.0, 0.0, 0.0])




#CALL
dynamics.dyn_manager.add_body_action (components.child2_init)



#/* Attach child1 to parent */
components.pt_attach1.action_name = "pt_attach1"
components.pt_attach1.set_subject_body( components.child1_body )
components.pt_attach1.set_parent_body( components.parent_body )
components.pt_attach1.subject_point_name = "front_to_back"
components.pt_attach1.parent_point_name  = "back_to_front"
components.pt_attach1.active = True


#CALL
dynamics.dyn_manager.add_body_action (components.pt_attach1)



#/* Attach child2 to child1 */
components.pt_attach2.action_name = "pt_attach2"
components.pt_attach2.set_subject_body( components.child2_body )
components.pt_attach2.set_parent_body( components.child1_body )
components.pt_attach2.subject_point_name = "front_to_back"
components.pt_attach2.parent_point_name  = "back_to_front"
components.pt_attach2.active = True


#CALL
dynamics.dyn_manager.add_body_action (components.pt_attach2)



#/* Mass output file */
components.file_name = "SET_test/RUN_103/mass.out"


trick.sim_services.exec_set_terminate_time(0)
