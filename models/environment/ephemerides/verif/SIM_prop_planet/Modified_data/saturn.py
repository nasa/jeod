################################################################################
# PURPOSE:
#   (To initialize the state of Saturn.
#    Note that Saturn can have its translational state initialized as a DynBody
#    or from the De405 ephemeris depending on the value of init_trans_state.)
################################################################################

def set_saturn(init_trans_state) :

  saturn.gravity_source.mu = sun.gravity_source.mu / 3047.898

  MASS = float(saturn.gravity_source.mu) / 6.67259e-11
  RADIUS = float(saturn.planet.r_eq)

  MR2    = (MASS) * (RADIUS) * (RADIUS)
  IZZ    = (MR2) * 0.210
  IXX    = (MR2) * (0.210 - 16298-6)

  saturn.prop_planet.planet_name = "Saturn"
  saturn.prop_planet.parent_name = "SSBary.inertial"

  saturn.relative.set_name("Saturn.relative")
  saturn.relative.subject_frame_name = "Saturn.composite_body"
  saturn.relative.target_frame_name = "Sun.inertial"
  saturn.relative.direction_sense = \
    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  saturn.mass_init.set_subject_body( saturn.prop_planet.body.mass )
  saturn.mass_init.action_name = "saturn.mass_init"

  saturn.mass_init.properties.pt_orientation.data_source = \
    trick.Orientation.InputQuaternion

  saturn.mass_init.properties.mass = MASS
  saturn.mass_init.properties.position = [0.0, 0.0, 0.0]
  saturn.mass_init.properties.inertia = [[IXX, 0.0, 0.0],\
                                         [0.0, IXX, 0.0],\
                                         [0.0, 0.0, IZZ]]

  dynamics.dyn_manager.add_body_action (saturn.mass_init)


  saturn.rot_init.set_subject_body( saturn.prop_planet.body )
  saturn.rot_init.action_name = "saturn.rot_init"
  saturn.rot_init.reference_ref_frame_name = "SSBary.inertial"
  saturn.rot_init.body_frame_id = "composite_body"
  saturn.rot_init.state_items = trick.DynBodyInitRotState.Both
  saturn.rot_init.orientation.data_source = \
    trick.Orientation.InputEulerRotation
  saturn.rot_init.orientation.euler_sequence = \
    trick.Orientation.EulerZXY
  saturn.rot_init.orientation.euler_angles = \
    trick.attach_units("degree", [83.537, 40.589, 0.0])
  saturn.rot_init.ang_velocity = \
    trick.attach_units("revolution/hr", [0.0, 0.0, 1.0/10.57])

  dynamics.dyn_manager.add_body_action (saturn.rot_init)


  if (init_trans_state) :
    saturn.prop_planet.commanded_mode = \
      trick.PropagatedPlanet.TransFromBody_RotFromBody

    saturn.trans_init.set_subject_body( saturn.prop_planet.body )
    saturn.trans_init.action_name = "saturn.trans_init"
    saturn.trans_init.reference_ref_frame_name = "SSBary.inertial"
    saturn.trans_init.body_frame_id = "composite_body"
    saturn.trans_init.state_items = trick.DynBodyInitTransState.Both
    saturn.trans_init.position = [ -50890496646.389175,\
                                  1246479898193.126,\
                                   517035300988.70905]
    saturn.trans_init.velocity = [-10167.305390009018,
                                    -517.55833690457985,
                                     223.93584762030568]

    dynamics.dyn_manager.add_body_action (saturn.trans_init)

  else :
    saturn.prop_planet.commanded_mode = \
      trick.PropagatedPlanet.TransFromPlanet_RotFromBody


  saturn.gravity_controls[0].source_name = "Sun"
  saturn.gravity_controls[0].active = True
  saturn.gravity_controls[0].spherical = True

  saturn.gravity_controls[1].source_name = "Jupiter"
  saturn.gravity_controls[1].active = True
  saturn.gravity_controls[1].spherical = True

  saturn.prop_planet.body.grav_interaction.add_control (
    saturn.gravity_controls[0])
  saturn.prop_planet.body.grav_interaction.add_control (
    saturn.gravity_controls[1])
