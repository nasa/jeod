trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "Space"


exec(compile(open( "Log_data/log_data.py", "rb").read(), "Log_data/log_data.py", 'exec'))

exec(compile(open( "Modified_data/veh_config.py", "rb").read(), "Modified_data/veh_config.py", 'exec'))
veh_config_alt( vehA, "A", "Space")
veh_config_alt( vehB, "B", "Space")

exec(compile(open( "Modified_data/veh_mass.py", "rb").read(), "Modified_data/veh_mass.py", 'exec'))
veh_mass_alt( vehA)
veh_mass_point( vehA)
veh_mass_alt( vehB)
veh_mass_point( vehB)
# Turn the point on vehB on yaw axis.  vehA yaw = 90 deg; vehB yaw = -90 deg.
vehB.mass_init.points[0].pt_orientation.euler_angles  = \
                                    trick.attach_units( "degree",[  -90.0, 0.0, 0.0])

exec(compile(open( "Modified_data/veh_state.py", "rb").read(), "Modified_data/veh_state.py", 'exec'))
veh_state_alt( vehA)
veh_state_alt( vehB)
vehA.trans_init.position                 = [ 10.0, 0.0, 0.0]
vehA.rot_init.orientation.data_source    = trick.Orientation.InputEulerRotation
vehA.rot_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
vehA.rot_init.orientation.euler_angles   = \
                                     trick.attach_units( "degree",[ 90.0, 0.0, 0.0])


# Configure relative states.
rel_state.vehA_wrt_vehB_in_B.set_name("A_wrt_B_in_B")
rel_state.vehA_wrt_vehB_in_B.subject_frame_name = "vehicleA.RefPoint"
rel_state.vehA_wrt_vehB_in_B.target_frame_name  = "vehicleB.RefPoint"
rel_state.vehA_wrt_vehB_in_B.direction_sense    = \
                          trick.RelativeDerivedState.ComputeSubjectStateinTarget

rel_state.vehB_wrt_vehA_in_A.set_name("B_wrt_A_in_A")
rel_state.vehB_wrt_vehA_in_A.subject_frame_name = "vehicleA.RefPoint"
rel_state.vehB_wrt_vehA_in_A.target_frame_name  = "vehicleB.RefPoint"
rel_state.vehB_wrt_vehA_in_A.direction_sense    = \
                          trick.RelativeDerivedState.ComputeTargetStateinSubject

rel_state.vehA_wrt_vehB_in_B_generalized.set_name("A_wrt_B_in_B_general")
rel_state.vehA_wrt_vehB_in_B_generalized.subject_frame_name = "vehicleA.RefPoint"
rel_state.vehA_wrt_vehB_in_B_generalized.target_frame_name  = "vehicleB.RefPoint"
rel_state.vehA_wrt_vehB_in_B_generalized.direction_sense    = \
                          trick.RelativeDerivedState.ComputeSubjectStateinTarget

trick.stop(100)
