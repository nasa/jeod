# Mass settings
def veh_mass( veh):
  veh.mass_init.set_subject_body( veh.dyn_body.mass )
  veh.mass_init.properties.pt_orientation.data_source = \
                                                   trick.Orientation.InputMatrix
  veh.mass_init.properties.pt_frame_spec = trick.MassPointInit.StructToBody
  veh.mass_init.properties.pt_orientation.trans = [[0.0,  0.0,  1.0],
                                                   [1.0,  0.0,  0.0],
                                                   [0.0,  1.0,  0.0]]
  veh.mass_init.properties.mass     = 123.0
  veh.mass_init.properties.position = [ 1.0,  2.0, 3.0]

  veh.mass_init.properties.inertia  = [[ 1.0, 0.0, 0.0],
                                       [ 0.0, 1.0, 0.0],
                                       [ 0.0, 0.0, 1.0]]

  dynamics.dyn_manager.add_body_action (veh.mass_init)


def veh_mass_alt( veh):
  # Set the mass porperties for this veh.
  veh.mass_init.set_subject_body( veh.dyn_body.mass )

  # Set the struct to body reference frame orientation.
  veh.mass_init.properties.pt_orientation.data_source = \
                                               trick.Orientation.InputQuaternion

  veh.mass_init.properties.mass = 1.0
  veh.mass_init.properties.position =[ 0.0, 0.0, 0.0]
  veh.mass_init.properties.inertia  = [[ 1.0, 0.0, 0.0],
                                       [ 0.0, 1.0, 0.0],
                                       [ 0.0, 0.0, 1.0]]


def veh_mass_point( veh):
  veh.mass_init.allocate_points(1)

  veh.mass_init.get_mass_point(0).set_name("RefPoint")
  veh.mass_init.get_mass_point(0).position  = [1, 0, 0]
  veh.mass_init.get_mass_point(0).pt_orientation.data_source = \
                                            trick.Orientation.InputEulerRotation
  veh.mass_init.get_mass_point(0).pt_orientation.euler_sequence  = \
                                                trick.Orientation.Yaw_Pitch_Roll
  veh.mass_init.get_mass_point(0).pt_orientation.euler_angles  = \
                                     trick.attach_units( "degree",[  90.0, 0.0, 0.0])

  dynamics.dyn_manager.add_body_action(veh.mass_init)
