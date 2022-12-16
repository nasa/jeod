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
# Configure chaser vehicle.
#---------------------------------------------
set_veh_properties(chaser, "chaser")
# Set vehicle initial state vector.
exec(compile(open( "Modified_data/chaser_state.py", "rb").read(), "Modified_data/chaser_state.py", 'exec'))

#---------------------------------------------
# Configure relative states.
#---------------------------------------------
rel_state.vehA_in_vehA_rectilvlh.set_name("A_in_A_rectilvlh")
rel_state.vehA_in_vehA_rectilvlh.subject_frame_name = "target.composite_body"
rel_state.vehA_in_vehA_rectilvlh.target_frame_name  = "target.composite_body.Earth.lvlh"
rel_state.vehA_in_vehA_rectilvlh.lvlh_type          = trick.LvlhType.Rectilinear

rel_state.vehB_in_vehA_rectilvlh.set_name("B_in_A_rectilvlh")
rel_state.vehB_in_vehA_rectilvlh.subject_frame_name = "chaser.composite_body"
rel_state.vehB_in_vehA_rectilvlh.target_frame_name  = "target.composite_body.Earth.lvlh"
rel_state.vehB_in_vehA_rectilvlh.lvlh_type          = trick.LvlhType.Rectilinear

rel_state.vehA_in_vehA_curvilvlh.set_name("A_in_A_curvilvlh")
rel_state.vehA_in_vehA_curvilvlh.subject_frame_name = "target.composite_body"
rel_state.vehA_in_vehA_curvilvlh.target_frame_name  = "target.composite_body.Earth.lvlh"
rel_state.vehA_in_vehA_curvilvlh.lvlh_type          = trick.LvlhType.CircularCurvilinear

rel_state.vehB_in_vehA_curvilvlh.set_name("B_in_A_curvilvlh")
rel_state.vehB_in_vehA_curvilvlh.subject_frame_name = "chaser.composite_body"
rel_state.vehB_in_vehA_curvilvlh.target_frame_name  = "target.composite_body.Earth.lvlh"
rel_state.vehB_in_vehA_curvilvlh.lvlh_type          = trick.LvlhType.CircularCurvilinear

#---------------------------------------------
# Set simulation stop time.
#---------------------------------------------
trick.stop(0)
