################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_relkin_test/
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#   Purpose:   Input file used for JEOD RelativeKinematics model unit test
#
#   Test:      Verify all methods of RelativeKinematics work correctly
#
#*******************************************************************************/


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_log_relkin_verif.py", "rb").read(), "Log_data/log_log_relkin_verif.py", 'exec'))
log_log_relkin_verif(LOG_CYCLE  )

#// Specify dynamics manager settings
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
dynamics.dyn_manager_init.central_point_name  = "Earth"

#// Specify settings for the integrator
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

#// Specify veh1 desired settings
veh1.dyn_body.set_name("target")
veh1.dyn_body.integ_frame_name = "Earth.inertial"
veh1.dyn_body.translational_dynamics = True
veh1.dyn_body.rotational_dynamics = True


#// Specify veh2 desired settings
veh2.dyn_body.set_name("chaser")
veh2.dyn_body.integ_frame_name = "Earth.inertial"
veh2.dyn_body.translational_dynamics = True
veh2.dyn_body.rotational_dynamics = True


#// Specify desired mass properties & vehicle points for target
veh1.mass_init.set_subject_body( veh1.dyn_body.mass )
veh1.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation
veh1.mass_init.properties.pt_orientation.eigen_angle  = 0.0
veh1.mass_init.properties.pt_orientation.eigen_axis  = [ 0, 1, 0]
veh1.mass_init.properties.mass  = 186069.0
veh1.mass_init.properties.position  = [ -3.0, -1.5, 4.0]
veh1.mass_init.properties.inertia[0]  = [ 1.02e+8, -6.96e+6, -5.48e+6]
veh1.mass_init.properties.inertia[1]  = [ -6.96e+6,  0.91e+8,  5.90e+5]
veh1.mass_init.properties.inertia[2]  = [ -5.48e+6,  5.90e+5,  1.64e+8]

# veh1.mass_init.points = trick.MassPointInit()
# veh1.mass_init.points.thisown = 0
veh1.mass_init.points = trick.sim_services.alloc_type(1, "jeod::MassPointInit")

veh1.mass_init.num_points = 1
veh1.mass_init.points[0].set_name("reflector1")
veh1.mass_init.points[0].position  = [ -10.0, 6.0, 2.5]
veh1.mass_init.points[0].pt_orientation.data_source =    trick.Orientation.InputEigenRotation


veh1.mass_init.points[0].pt_orientation.eigen_angle  = trick.attach_units( "degree",3.0)
veh1.mass_init.points[0].pt_orientation.eigen_axis  = [ 0, 1, 0]
veh1.mass_init.points[0].pt_frame_spec = trick.MassPointInit.StructToChild


#CALL
dynamics.dyn_manager.add_body_action (veh1.mass_init)



#// Specify desired mass properties & vehicle points for chaser
veh2.mass_init.set_subject_body( veh2.dyn_body.mass )
veh2.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


veh2.mass_init.properties.pt_orientation.eigen_angle  = 0.0
veh2.mass_init.properties.pt_orientation.eigen_axis  = [ 0, 1, 0]
veh2.mass_init.properties.mass  = 186069.0
veh2.mass_init.properties.position  = [ -3.0, -1.5, 4.0]
veh2.mass_init.properties.inertia[0]  = [ 1.02e+8, -6.96e+6, -5.48e+6]
veh2.mass_init.properties.inertia[1]  = [ -6.96e+6,  0.91e+8,  5.90e+5]
veh2.mass_init.properties.inertia[2]  = [ -5.48e+6,  5.90e+5,  1.64e+8]

# veh2.mass_init.points = trick.MassPointInit()
# veh2.mass_init.points.thisown = 0
veh2.mass_init.points = trick.sim_services.alloc_type(1, "jeod::MassPointInit")

veh2.mass_init.num_points = 1
veh2.mass_init.points[0].set_name("lidar1")
veh2.mass_init.points[0].position  = [ 7.1, 2.3, 1.5]
veh2.mass_init.points[0].pt_orientation.data_source =    trick.Orientation.InputEigenRotation


veh2.mass_init.points[0].pt_orientation.eigen_angle  = trick.attach_units( "degree",8.0)
veh2.mass_init.points[0].pt_orientation.eigen_axis  = [ 0, 1, 0]
veh2.mass_init.points[0].pt_frame_spec = trick.MassPointInit.StructToChild


#CALL
dynamics.dyn_manager.add_body_action (veh2.mass_init)



#// Set the translational states of target
veh1.trans_init.set_subject_body( veh1.dyn_body )
veh1.trans_init.reference_ref_frame_name = "Earth.inertial"
veh1.trans_init.body_frame_id = "composite_body"
veh1.trans_init.position  = [ 521527.69, -5485464.19, 3834569.45 ]
veh1.trans_init.velocity  = [ 5568.479360,3407.973419, 4104.551857 ]


#CALL
dynamics.dyn_manager.add_body_action (veh1.trans_init)


#// Set the translational states of chaser
veh2.trans_init.set_subject_body( veh2.dyn_body )
veh2.trans_init.reference_ref_frame_name = "Earth.inertial"
veh2.trans_init.body_frame_id = "composite_body"
veh2.trans_init.position  = [ 501527.69, -5485364.19, 3838569.45 ]
veh2.trans_init.velocity  = [ 3568.479360,5402.973419, 4084.551857 ]


#CALL
dynamics.dyn_manager.add_body_action (veh2.trans_init)



#// Set the rotational states of target
veh1.rot_init.set_subject_body( veh1.dyn_body )
veh1.rot_init.reference_ref_frame_name = "Earth.inertial"
veh1.rot_init.body_frame_id = "composite_body"
veh1.rot_init.orientation.data_source = trick.Orientation.InputMatrix
veh1.rot_init.orientation.trans[0]  = [ 0.714588608070700, 0.449153881374066, 0.536305800884338]
veh1.rot_init.orientation.trans[1]  = [ 0.694478020841567, -0.363383601951388, -0.621009369012107]
veh1.rot_init.orientation.trans[2]  = [ -0.084044034788673, 0.816218811765232, -0.571597280904129]


#CALL
dynamics.dyn_manager.add_body_action (veh1.rot_init)



#// Set the rotational states of chaser
veh2.rot_init.set_subject_body( veh2.dyn_body )
veh2.rot_init.reference_ref_frame_name = "Earth.inertial"
veh2.rot_init.body_frame_id = "composite_body"
veh2.rot_init.orientation.data_source = trick.Orientation.InputMatrix
veh2.rot_init.orientation.trans[0]  = [ 1.0, 0.0, 0.0]
veh2.rot_init.orientation.trans[1]  = [ 0.0, 1.0, 0.0]
veh2.rot_init.orientation.trans[2]  = [ 0.0, 0.0, 1.0]


#CALL
dynamics.dyn_manager.add_body_action (veh2.rot_init)



#// Specify the desired Earth gravity model settings for each vehicle
veh1.grav_control.source_name   = "Earth"
veh1.grav_control.active        = True
veh1.grav_control.spherical     = True

veh1.dyn_body.grav_interaction.add_control(veh1.grav_control)

veh2.grav_control.source_name   = "Earth"
veh2.grav_control.active        = True
veh2.grav_control.spherical     = True

veh2.dyn_body.grav_interaction.add_control(veh2.grav_control)


#// Set up the cm-to-cm relative state to be used for testing the RelKin model
relkin.cm_relstate.set_name("chasecm_wrt_targetcm_in_target")
relkin.cm_relstate.subject_frame_name = "chaser.composite_body"
relkin.cm_relstate.target_frame_name = "target.composite_body"
relkin.cm_relstate.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget


#// Set up the sensor #1 relative state to be used for testing the RelKin model
relkin.sensor1_relstate.set_name("chase_lidar1_to_target_reflector1")
relkin.sensor1_relstate.subject_frame_name = "chaser.lidar1"
relkin.sensor1_relstate.target_frame_name = "target.reflector1"
relkin.sensor1_relstate.direction_sense = trick.RelativeDerivedState.ComputeTargetStateinSubject


#// Set up the sensor #2 relative state to be used for testing the RelKin model
relkin.sensor2_relstate.set_name("target_reflector1_to_chase_lidar1")
relkin.sensor2_relstate.subject_frame_name = "chaser.lidar1"
relkin.sensor2_relstate.target_frame_name = "target.reflector1"
relkin.sensor2_relstate.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget


#// Set up simulation termination time

trick.sim_services.exec_set_terminate_time(2000)
