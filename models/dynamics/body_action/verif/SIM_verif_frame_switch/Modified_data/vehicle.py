veh.dyn_body.set_name("veh")
veh.dyn_body.integ_frame_name = "Earth.inertial"
veh.dyn_body.translational_dynamics = True
veh.dyn_body.rotational_dynamics = True

# Configure the mass
veh.mass_init.action_name = "veh_mass_init"
veh.mass_init.set_subject_body( veh.dyn_body.mass )
veh.mass_init.properties.pt_orientation.data_source = \
  trick.Orientation.InputEigenRotation
veh.mass_init.properties.pt_orientation.eigen_angle  = 0.0 
veh.mass_init.properties.pt_orientation.eigen_axis = [ 1, 0, 0]
veh.mass_init.properties.mass = 91589.71
veh.mass_init.properties.position = \
  trick.attach_units( "in",[ 1098.0, 0.0, 372.0])
veh.mass_init.properties.inertia[0] = \
  trick.attach_units( "slug*ft2",[ 100000.0,      0.0,      0.0])
veh.mass_init.properties.inertia[1] = \
  trick.attach_units( "slug*ft2",[      0.0, 200000.0,      0.0])
veh.mass_init.properties.inertia[2] = \
  trick.attach_units( "slug*ft2",[      0.0,      0.0, 400000.0])
dynamics.dyn_manager.add_body_action (veh.mass_init)

# Configure the vehicle translational state via a body action.
veh.trans_init.action_name = "veh_init_trans"
veh.trans_init.set_subject_body( veh.dyn_body )
veh.trans_init.reference_ref_frame_name = "Earth.inertial"
veh.trans_init.body_frame_id = "composite_body"
veh.trans_init.position = \
    [  302274887.75381017, -119023818.10882501, -56915743.953866437]
veh.trans_init.velocity = \
    [  942.18249467301985, -189.92063800611407, -292.95966550646989]
dynamics.dyn_manager.add_body_action (veh.trans_init)


# Configure the vehicle rotational state via a body action.
veh.rot_init.action_name = "veh_init_rot"
veh.rot_init.reference_ref_frame_name = "Moon.inertial"
veh.rot_init.body_frame_id = "composite_body"
veh.rot_init.set_subject_body( veh.dyn_body )
veh.rot_init.orientation.data_source = trick.Orientation.InputEulerRotation
veh.rot_init.orientation.eigen_angle  = 0.0
veh.rot_init.orientation.eigen_axis  = [ 0, 1, 0]
dynamics.dyn_manager.add_body_action (veh.rot_init)

# Specify the objects toward which the vehicle gravitates.
veh.sun_grav_ctrl.source_name   = "Sun"
veh.sun_grav_ctrl.active        = True
veh.sun_grav_ctrl.spherical     = True
veh.dyn_body.grav_interaction.add_control(veh.sun_grav_ctrl);

veh.earth_grav_ctrl.source_name   = "Earth"
veh.earth_grav_ctrl.active        = True
veh.earth_grav_ctrl.spherical     = True
veh.dyn_body.grav_interaction.add_control(veh.earth_grav_ctrl);

veh.moon_grav_ctrl.source_name   = "Moon"
veh.moon_grav_ctrl.active        = True
veh.moon_grav_ctrl.spherical     = True
veh.dyn_body.grav_interaction.add_control(veh.moon_grav_ctrl);


# Configure the relative state.
veh.embary.subject_frame_name = "veh.composite_body"
veh.embary.target_frame_name = "EMBary.inertial"
veh.embary.direction_sense = \
  trick.RelativeDerivedState.ComputeSubjectStateinTarget
