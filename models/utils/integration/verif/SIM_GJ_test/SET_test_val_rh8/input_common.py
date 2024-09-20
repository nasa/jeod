trick.sim_services.exec_set_trap_sigfpe(1)

exec(compile(open( "Log_data/log_data_rec.py", "rb").read(), "Log_data/log_data_rec.py", 'exec'))

state.r_err_mag_tot = 1.0E-16

jeod_time.time_manager_init.initializer = "UTC"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

#// Time itime_nitialization data.
#// Midnigtime_ht on November 20, 2007.
jeod_time.time_utc.calendar_year   = 2007
jeod_time.time_utc.calendar_month  =   11
jeod_time.time_utc.calendar_day    =   20
jeod_time.time_utc.calendar_hour   =    0
jeod_time.time_utc.calendar_minute =    0
jeod_time.time_utc.calendar_second =  0.0

jeod_time.time_tai.initialize_from_name = "UTC"
jeod_time.time_ut1.initialize_from_name = "TAI"
jeod_time.time_tt.initialize_from_name  = "TAI"

jeod_time.time_tai.update_from_name = "Dyn"
jeod_time.time_ut1.update_from_name = "TAI"
jeod_time.time_utc.update_from_name = "TAI"
jeod_time.time_tt.update_from_name  = "TAI"






vehicle.dyn_body.set_name("vehicle")
vehicle.dyn_body.integ_frame_name = "Earth.inertial"
vehicle.dyn_body.translational_dynamics = True
vehicle.dyn_body.rotational_dynamics = False


vehicle.mass_init.set_subject_body(vehicle.dyn_body.mass)
vehicle.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation
vehicle.mass_init.properties.pt_orientation.eigen_angle  = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis  = [ 0, 1, 0]
vehicle.mass_init.properties.mass  = 300
vehicle.mass_init.properties.position  = [ 0.0, 0.0, 0.0]
vehicle.mass_init.properties.inertia[0]  = [ 100.0,  0.0,   0.0]
vehicle.mass_init.properties.inertia[1]  = [ 0.0,    200.0, 0.0]
vehicle.mass_init.properties.inertia[2]  = [ 0.0,    0.0,   400.0]


dynamics.dyn_manager.add_body_action (vehicle.mass_init)


vehicle.trans_init.set_subject_body(vehicle.dyn_body)
vehicle.trans_init.reference_ref_frame_name = "Earth.inertial"
vehicle.trans_init.body_frame_id = "composite_body"

earth.gravity_source.mu = 5.76E14
vehicle.trans_init.position  = [ 9.0E6, 0, 0]
vehicle.trans_init.velocity  = [ 0,8000.0,0]
dynamics.dyn_manager.add_body_action (vehicle.trans_init)


vehicle.rot_init.set_subject_body(vehicle.dyn_body)
vehicle.rot_init.reference_ref_frame_name = "Earth.inertial"
vehicle.rot_init.body_frame_id = "composite_body"
vehicle.rot_init.ang_velocity  = [ 0,0, 0.07]
vehicle.rot_init.orientation.data_source = trick.Orientation.InputMatrix

vehicle.rot_init.orientation.trans[0]  = [ 1.0, 0.0, 0.0]
vehicle.rot_init.orientation.trans[1]  = [ 0.0, 1.0, 0.0]
vehicle.rot_init.orientation.trans[2]  = [ 0.0, 0.0, 1.0]

dynamics.dyn_manager.add_body_action (vehicle.rot_init)

vehicle.grav_control.source_name   = "Earth"
vehicle.grav_control.active        = True
vehicle.grav_control.spherical     = True

vehicle.dyn_body.grav_interaction.add_control(vehicle.grav_control)
