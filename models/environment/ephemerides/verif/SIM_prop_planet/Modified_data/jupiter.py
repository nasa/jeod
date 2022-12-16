################################################################################
# PURPOSE:
#   (To initialize the state of Jupiter.
#    Note that Jupiter can have its translational state initialized as a DynBody
#    or from the De405 ephemeris depending on the value of init_trans_state.)
################################################################################

def set_jupiter(init_trans_state) :

  jupiter.gravity_source.mu = sun.gravity_source.mu / 1047.3486

  MASS = float(jupiter.gravity_source.mu) / 6.67259e-11
  RADIUS = float(jupiter.planet.r_eq)

  MR2    = (MASS) * (RADIUS) * (RADIUS)
  IZZ    = (MR2) * 0.254
  IXX    = (MR2) * (0.254 - 14736e-6)

  jupiter.prop_planet.planet_name = "Jupiter"
  jupiter.prop_planet.parent_name = "SSBary.inertial"

  jupiter.relative.set_name("Jupiter.relative")
  jupiter.relative.subject_frame_name = "Jupiter.composite_body"
  jupiter.relative.target_frame_name = "Sun.inertial"
  jupiter.relative.direction_sense = \
    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  jupiter.mass_init.subject =  jupiter.prop_planet.body.mass
  jupiter.mass_init.action_name = "jupiter.mass_init"

  jupiter.mass_init.properties.pt_orientation.data_source = \
    trick.Orientation.InputQuaternion

  jupiter.mass_init.properties.mass = MASS
  jupiter.mass_init.properties.position = [0.0, 0.0, 0.0]
  jupiter.mass_init.properties.inertia = [[IXX, 0.0, 0.0],\
                                          [0.0, IXX, 0.0],\
                                          [0.0, 0.0, IZZ]]

  dynamics.dyn_manager.add_body_action (jupiter.mass_init)


  jupiter.rot_init.dyn_subject = jupiter.prop_planet.body
  jupiter.rot_init.action_name = "jupiter.rot_init"
  jupiter.rot_init.reference_ref_frame_name = "SSBary.inertial"
  jupiter.rot_init.body_frame_id = "composite_body"
  jupiter.rot_init.state_items = trick.DynBodyInitRotState.Both
  jupiter.rot_init.orientation.data_source = \
    trick.Orientation.InputEulerRotation
  jupiter.rot_init.orientation.euler_sequence = \
    trick.Orientation.EulerZXY
  jupiter.rot_init.orientation.euler_angles = \
    trick.attach_units("degree", [64.496, 268.057, 0.0])
  jupiter.rot_init.ang_velocity = \
    trick.attach_units("revolution/hr", [0.0, 0.0, 1.0/9.925])

  dynamics.dyn_manager.add_body_action (jupiter.rot_init)


  if (init_trans_state) :
    jupiter.prop_planet.commanded_mode = \
      trick.PropagatedPlanet.TransFromBody_RotFromBody

    jupiter.trans_init.dyn_subject = jupiter.prop_planet.body
    jupiter.trans_init.action_name = "jupiter.trans_init"
    jupiter.trans_init.reference_ref_frame_name = "SSBary.inertial"
    jupiter.trans_init.body_frame_id = "composite_body"
    jupiter.trans_init.state_items = trick.DynBodyInitTransState.Both
    jupiter.trans_init.position = [-642244094679.87073,\
                                    432756714082.86768,
                                    201130288017.2879]
    jupiter.trans_init.velocity = [-7953.5092045612919,
                                   -9143.3398686663604,
                                   -3725.5279239368501]

    dynamics.dyn_manager.add_body_action (jupiter.trans_init)

  else :
    jupiter.prop_planet.commanded_mode = \
      trick.PropagatedPlanet.TransFromPlanet_RotFromBody


  jupiter.gravity_controls[0].source_name = "Sun"
  jupiter.gravity_controls[0].active = True
  jupiter.gravity_controls[0].spherical = True

  jupiter.gravity_controls[1].source_name = "Saturn"
  jupiter.gravity_controls[1].active = True
  jupiter.gravity_controls[1].spherical = True

  jupiter.prop_planet.body.grav_interaction.add_control (
    jupiter.gravity_controls[0])
  jupiter.prop_planet.body.grav_interaction.add_control (
    jupiter.gravity_controls[1])
