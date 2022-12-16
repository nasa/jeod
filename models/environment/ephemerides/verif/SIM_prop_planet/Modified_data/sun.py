################################################################################
# PURPOSE:
#   (To initialize the state of the Sun.
#    The Sun's translational state is always initialized from the DE405 model.
#    The rotational state is always initialized as a DynBody.)
################################################################################

MU     = 1.32712440e20
MASS   = (MU) / 6.67428e-11
RADIUS = 696000e3
MR2    = (MASS) * (RADIUS) * (RADIUS)
IZZ    = (MR2) * 0.059

sun.prop_planet.planet_name = "Sun"
sun.prop_planet.parent_name = "SSBary.inertial"

sun.relative.set_name("Sun.relative")
sun.relative.subject_frame_name = "Sun.composite_body"
sun.relative.target_frame_name = "SSBary.inertial"
sun.relative.direction_sense = \
  trick.RelativeDerivedState.ComputeSubjectStateinTarget

sun.mass_init.subject = sun.prop_planet.body.mass
sun.mass_init.action_name = "sun.mass_init"
sun.mass_init.properties.pt_orientation.data_source = \
  trick.Orientation.InputQuaternion

sun.mass_init.properties.mass = MASS
sun.mass_init.properties.position = [0.0, 0.0, 0.0]
sun.mass_init.properties.inertia = [ [IZZ, 0.0, 0.0],
                                     [0.0, IZZ, 0.0],
                                     [0.0, 0.0, IZZ]]

dynamics.dyn_manager.add_body_action (sun.mass_init)

sun.rot_init.dyn_subject = sun.prop_planet.body
sun.rot_init.action_name = "sun.rot_init"
sun.rot_init.reference_ref_frame_name = "SSBary.inertial"
sun.rot_init.body_frame_id = "composite_body"
sun.rot_init.state_items = trick.DynBodyInitRotState.Both
sun.rot_init.orientation.data_source = trick.Orientation.InputEulerRotation
sun.rot_init.orientation.euler_sequence = trick.Orientation.EulerZXY
sun.rot_init.orientation.euler_angles = \
  trick.attach_units ("degree", [63.87, 286.13, 0.0])
sun.rot_init.ang_velocity = \
  trick.attach_units ("revolution/day",[ 0.0, 0.0, 1.0/25.05])

dynamics.dyn_manager.add_body_action (sun.rot_init)


sun.gravity_controls[0].source_name = "Jupiter"
sun.gravity_controls[0].active = True
sun.gravity_controls[0].spherical = True

sun.gravity_controls[1].source_name = "Saturn"
sun.gravity_controls[1].active = True
sun.gravity_controls[1].spherical = True

sun.prop_planet.body.grav_interaction.add_control (
  sun.gravity_controls[0])
sun.prop_planet.body.grav_interaction.add_control (
  sun.gravity_controls[1])

sun.prop_planet.commanded_mode = \
  trick.PropagatedPlanet.TransFromPlanet_RotFromBody
