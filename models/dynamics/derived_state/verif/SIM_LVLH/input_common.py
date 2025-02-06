trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

exec(compile(open( "Log_data/log_orbital_state_rec.py", "rb").read(), "Log_data/log_orbital_state_rec.py", 'exec'))

exec(compile(open( "Modified_data/date_and_time.py", "rb").read(), "Modified_data/date_and_time.py", 'exec'))

exec(compile(open( "Modified_data/veh_config.py", "rb").read(), "Modified_data/veh_config.py", 'exec'))
veh_config(vehA, "A")
veh_config(vehB, "B")

exec(compile(open( "Modified_data/veh_mass.py", "rb").read(), "Modified_data/veh_mass.py", 'exec'))
veh_mass(vehA)
veh_mass(vehB)

exec(compile(open( "Modified_data/veh_state.py", "rb").read(), "Modified_data/veh_state.py", 'exec'))
veh_state(vehA)
veh_state(vehB)
# Put vehB 1 degree behind vehA:
vehB.init_orbit.true_anomaly   = trick.attach_units( "degree",359.0)

vehA.lvlh.reference_name = "Earth"
vehB.lvlh.reference_name = "Earth"

rel_state.vehA_wrt_vehB.subject_frame_name = "vehicleA.composite_body"
rel_state.vehA_wrt_vehB.target_frame_name = "vehicleB.Earth.lvlh"
rel_state.vehA_wrt_vehB.direction_sense = \
                    trick.RelativeDerivedState.ComputeSubjectStateinTarget

rel_state.vehB_wrt_vehA.subject_frame_name = "vehicleB.composite_body"
rel_state.vehB_wrt_vehA.target_frame_name = "vehicleA.Earth.lvlh"
rel_state.vehB_wrt_vehA.direction_sense = \
                    trick.RelativeDerivedState.ComputeSubjectStateinTarget


trick.stop(86400)
