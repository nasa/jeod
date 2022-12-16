def child1_mass_default():
  components.child1_body.set_name("Child1")
  components.child1_init.subject     = components.child1_body
  components.child1_init.action_name = "Child1_mass_init"
  components.child1_init.properties.mass  = 1.0
  components.child1_init.properties.position  = [ 0.0, 0.0, 0.0]

  components.child1_init.properties.inertia_spec = trick.MassPropertiesInit.Body
  components.child1_init.properties.inertia  = [
                                        [ 0.41666667, 0.0,        0.0],
                                        [ 0.0,        0.41666667, 0.0],
                                        [ 0.0,        0.0,        0.1666667]]
  components.child1_init.properties.pt_orientation.data_source = \
                                             trick.Orientation.InputQuaternion

  dynamics.dyn_manager.add_body_action (components.child1_init)


def child1_mass_inertia_optionA():
  components.child1_init.properties.inertia_spec = trick.MassPropertiesInit.Spec
  components.child1_init.properties.inertia  = [
                                         [ 1.4166667, 0.144338, -0.433013],
                                         [ 0.144338,  1.58333,   0.25],
                                         [-0.433013,  0.25,      0.333333]]
  components.child1_init.properties.inertia_offset  = [ .433013, -.25, 1]
  components.child1_init.properties.inertia_orientation.data_source = \
                                                   trick.Orientation.InputMatrix
  components.child1_init.properties.inertia_orientation.trans  = [
                                                  [ 0.8660254, -0.5,       0.0],
                                                  [ 0.5,        0.8660254, 0.0],
                                                  [ 0.0,        0.0,       1.0]]

def child1_mass_inertia_optionB():
  components.child1_init.properties.inertia_spec = \
                                                trick.MassPropertiesInit.SpecCG
  components.child1_init.properties.inertia = [
                      [ 0.35416666666667, 0.03608439182435, 0.0],
                      [ 0.03608439182435, 0.39583333333333, 0.0],
                      [ 0.0,              0.0,              0.08333333333333]]

def child1_mass_inertia_optionC():
  components.child1_init.properties.inertia_spec = trick.MassPropertiesInit.Body
  components.child1_init.properties.inertia = [
                                       [ 0.33333333, 0.0,        0.0],
                                       [ 0.0,        0.41666667, 0.0],
                                       [ 0.0,        0.0,        0.0833333333]]

def child1_mass_points_1A():
  components.child1_init.num_points = 1
  components.child1_init.points = \
                               trick.sim_services.alloc_type( 1,"jeod::MassPointInit")
  components.child1_init.points[0].set_name("right_to_top")
  components.child1_init.points[0].position  = [ 0.0, 0.0, 1.0]
  components.child1_init.points[0].pt_orientation.data_source = \
                                                   trick.Orientation.InputMatrix
  components.child1_init.points[0].pt_orientation.trans = [[ 0.0,  0.0, 1.0],
                                                           [ 0.0, -1.0, 0.0],
                                                           [ 1.0,  0.0, 0.0]]

def child1_mass_points_1B():
  components.child1_init.num_points = 1
  components.child1_init.points = \
                               trick.sim_services.alloc_type( 1,"jeod::MassPointInit")
  components.child1_init.points[0].set_name("bottom_to_top")
  components.child1_init.points[0].position  = [ 0.0, -0.5, 0.0]
  components.child1_init.points[0].pt_orientation.data_source = \
                                            trick.Orientation.InputEulerRotation
  components.child1_init.points[0].pt_orientation.euler_sequence = \
                                                trick.Orientation.Yaw_Pitch_Roll
  components.child1_init.points[0].pt_orientation.euler_angles = \
                                   trick.attach_units( "degree",[ -90.0, 0.0, 0.0])

def child1_mass_points_2():
  components.child1_init.num_points = 2
  components.child1_init.points = \
                          trick.sim_services.alloc_type( 2 , "jeod::MassPointInit" )
  components.child1_init.points[0].set_name("right_to_left")
  components.child1_init.points[0].position  = [ 0.0, 0.0, 1.0]
  components.child1_init.points[0].pt_orientation.data_source = \
                                              trick.Orientation.InputMatrix
  components.child1_init.points[0].pt_orientation.trans = [[ 0.0,  0.0, 1.0],
                                                           [ 0.0, -1.0, 0.0],
                                                           [ 1.0,  0.0, 0.0]]

  components.child1_init.points[1].set_name("left_to_right")
  components.child1_init.points[1].position  = [ 0.0, 0.0, -1.0]
  components.child1_init.points[1].pt_orientation.data_source = \
                                              trick.Orientation.InputMatrix
  components.child1_init.points[1].pt_orientation.trans = [[ 0.0, 0.0, -1.0],
                                                           [ 0.0, 1.0,  0.0],
                                                           [ 1.0, 0.0,  0.0]]
