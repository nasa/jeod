#---------------------------------------------
# Set up the logging parameters.
#---------------------------------------------
exec(compile(open( "Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_rel_state_rec( 1.0 )
log_veh_state_rec( 1.0 )


#---------------------------------------------
# Set up time.
#---------------------------------------------
jeod_time.time_manager_init.initializer = "TAI"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.truncated_julian
jeod_time.time_tai.initializing_value = 10000
jeod_time.time_tai.update_from_name = "Dyn"


#---------------------------------------------
# Set up the dynamics manager.
#---------------------------------------------
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
dynamics.dyn_manager_init.central_point_name = "Earth"

#---------------------------------------------
# Set up the vehicles:
#---------------------------------------------
exec(compile(open("Modified_data/veh_properties.py", "rb").read(), "Modified_data/veh_properties.py", 'exec'))
#---------------------------------------------
# Configure target vehicle.
#---------------------------------------------
set_veh_properties(target, "target")
# Set vehicle initial state vector.
exec(compile(open( "Modified_data/target_state.py", "rb").read(), "Modified_data/target_state.py", 'exec'))

#---------------------------------------------
# Configure relative states.
#---------------------------------------------
rel_state.target_in_pfix.set_name("target_in_pfix")
rel_state.target_in_pfix.subject_frame_name = "target.attach1"
rel_state.target_in_pfix.target_frame_name  = "Earth.pfix"
rel_state.target_in_pfix.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget

attachTarget = trick.BodyAttachMatrix()
attachTarget.set_subject_body(target.dyn_body)
attachTarget.set_parent_frame(earth.planet.pfix)
attachTarget.offset_pstr_cstr_pstr = [10.0, 0.0, 0.0]
attachTarget.pstr_cstr.data_source = trick.Orientation.InputMatrix
attachTarget.pstr_cstr.trans = [[-1.0, 0.0, 0.0],[0.0, -1.0, 0.0],[0.0, 0.0, 1.0]]
trick.add_read(50.0, 'dynamics.dyn_manager.add_body_action(attachTarget)')


#---------------------------------------------
# Set simulation stop time.
#---------------------------------------------
trick.stop(100.0)
