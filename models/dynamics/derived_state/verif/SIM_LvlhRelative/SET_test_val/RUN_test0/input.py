# Set up the simulation executive parameters.
trick.sim_services.exec_set_trap_sigfpe = True

# Set up the logging parameters.
exec(compile(open( "Log_data/log_data.py", "rb").read(), "Log_data/log_data.py", 'exec'))

# Set up the dynamics manager.
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
dynamics.dyn_manager_init.central_point_name = "ref_planet"

# Set up the planet used for LVLH.
dynamics.reference_planet.set_name("ref_planet")

#---------------------------------------------
# Configure vehicles
#---------------------------------------------
exec(compile(open( "Modified_data/veh_config.py", "rb").read(), "Modified_data/veh_config.py", 'exec'))
veh_config_alt( vehA, "A", "ref_planet")
veh_config_alt( vehB, "B", "ref_planet")

# Configure vehicle mass properties.
exec(compile(open( "Modified_data/veh_mass.py", "rb").read(), "Modified_data/veh_mass.py", 'exec'))
veh_mass_alt(vehA)
veh_mass_alt(vehB)

# Set vehicle initial states.
exec(compile(open( "Modified_data/veh_state.py", "rb").read(), "Modified_data/veh_state.py", 'exec'))
veh_state_alt(vehA)
veh_state_alt(vehB)
# make adjustments to vehA state:
vehA.trans_init.position  = [ 10.0, 0.0, 0.0]
vehA.trans_init.velocity  = [  0.0, 1.0, 0.0]
# make adjustments to vehB state:
vehB.trans_init.position  = [ 0.0, 10.0, 0.0]
vehB.trans_init.velocity  = [ -(2**0.5/2), 0.0, 2**0.5/2 ]
vehB.rot_init.ang_velocity= trick.attach_units( "degree/s",[ 0.0, 0.0, 4.5])

# Configure LVLH frames
vehA.lvlh_frame.subject_name = "vehicleA.composite_body"
vehA.lvlh_frame.planet_name  = "ref_planet"
vehB.lvlh_frame.subject_name = "vehicleB.composite_body"
vehB.lvlh_frame.planet_name  = "ref_planet"

#---------------------------------------------
# Configure relative states.
#---------------------------------------------
rel_state.vehA_wrt_vehB_in_B.set_name("A_wrt_B_in_B")
rel_state.vehA_wrt_vehB_in_B.subject_frame_name = "vehicleA.composite_body"
rel_state.vehA_wrt_vehB_in_B.target_frame_name  = "vehicleB.composite_body.ref_planet.lvlh"
rel_state.vehA_wrt_vehB_in_B.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget

rel_state.vehB_wrt_vehA_in_A.set_name("B_wrt_A_in_A")
rel_state.vehB_wrt_vehA_in_A.subject_frame_name = "vehicleB.composite_body"
rel_state.vehB_wrt_vehA_in_A.target_frame_name  = "vehicleA.composite_body.ref_planet.lvlh"
rel_state.vehB_wrt_vehA_in_A.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget

rel_state.vehB_in_vehA_rectilvlh.set_name("B_in_A_rectilvlh")
rel_state.vehB_in_vehA_rectilvlh.subject_frame_name = "vehicleB.composite_body"
rel_state.vehB_in_vehA_rectilvlh.target_frame_name  = "vehicleA.composite_body.ref_planet.lvlh"
rel_state.vehB_in_vehA_rectilvlh.lvlh_type          = trick.LvlhType.Rectilinear

rel_state.vehA_in_vehB_rectilvlh.set_name("A_in_B_rectilvlh")
rel_state.vehA_in_vehB_rectilvlh.subject_frame_name = "vehicleA.composite_body"
rel_state.vehA_in_vehB_rectilvlh.target_frame_name  = "vehicleB.composite_body.ref_planet.lvlh"
rel_state.vehA_in_vehB_rectilvlh.lvlh_type          = trick.LvlhType.Rectilinear

rel_state.vehB_in_vehA_curvilvlh.set_name("B_in_A_curvilvlh")
rel_state.vehB_in_vehA_curvilvlh.subject_frame_name = "vehicleB.composite_body"
rel_state.vehB_in_vehA_curvilvlh.target_frame_name  = "vehicleA.composite_body.ref_planet.lvlh"
rel_state.vehB_in_vehA_curvilvlh.lvlh_type          = trick.LvlhType.CircularCurvilinear

rel_state.vehA_in_vehB_curvilvlh.set_name("A_in_B_curvilvlh")
rel_state.vehA_in_vehB_curvilvlh.subject_frame_name = "vehicleA.composite_body"
rel_state.vehA_in_vehB_curvilvlh.target_frame_name  = "vehicleB.composite_body.ref_planet.lvlh"
rel_state.vehA_in_vehB_curvilvlh.lvlh_type          = trick.LvlhType.CircularCurvilinear

rel_state.test_c2r.set_name("test_c2r")
rel_state.test_c2r.subject_frame_name = "vehicleB.composite_body"
rel_state.test_c2r.target_frame_name  = "vehicleA.composite_body.ref_planet.lvlh"
rel_state.test_c2r.lvlh_type          = trick.LvlhType.CircularCurvilinear

# Turn on theta dot correction
rel_state.vehA_in_vehB_curvilvlh.use_theta_dot_correction = True
rel_state.vehB_in_vehA_curvilvlh.use_theta_dot_correction = True
rel_state.test_c2r.use_theta_dot_correction = True


# Set simulation stop time.
trick.stop(100)
