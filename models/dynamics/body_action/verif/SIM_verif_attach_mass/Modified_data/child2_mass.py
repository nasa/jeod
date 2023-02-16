def child2_mass_default():
  components.child2_body.set_name("Child2")
  components.child2_init.set_subject_body( components.child2_body )
  components.child2_init.action_name = "Child2_mass_init"
  components.child2_init.properties.mass  = 1.0
  components.child2_init.properties.position  = [ 0.0, 0.0, 0.0]

  components.child2_init.properties.inertia_spec = trick.MassPropertiesInit.Body
  components.child2_init.properties.inertia  = [
                                        [ 0.41666667, 0.0,        0.0],
                                        [ 0.0,        0.41666667, 0.0],
                                        [ 0.0,        0.0,        0.1666667]]
  components.child2_init.properties.pt_orientation.data_source = \
                                             trick.Orientation.InputQuaternion

  dynamics.dyn_manager.add_body_action (components.child2_init)


def child2_mass_inertia_optionA():
  components.child2_init.properties.inertia_spec = trick.MassPropertiesInit.Body
  components.child2_init.properties.inertia = [
                                       [ 0.33333333, 0.0,        0.0],
                                       [ 0.0,        0.41666667, 0.0],
                                       [ 0.0,        0.0,        0.0833333333]]

def child2_mass_inertia_optionB():
  components.child2_init.properties.inertia_spec = \
                                                 trick.MassPropertiesInit.Struct
  components.child2_init.properties.inertia = [
                                       [  1.33333333, 0.0,       -0.5],
                                       [  0.0,        1.66666667, 0.0],
                                       [ -0.5,        0.0,        0.33333333]]


def child2_mass_points_1A():
  components.child2_init.num_points = 1
  components.child2_init.points = \
                               trick.sim_services.alloc_type( 1,"jeod::MassPointInit")
  components.child2_init.points[0].set_name("right_to_left")
  components.child2_init.points[0].position  = [ 0.0, 0.0, 1.0]
  components.child2_init.points[0].pt_orientation.data_source = \
                                                  trick.Orientation.InputMatrix
  components.child2_init.points[0].pt_orientation.trans = [[ 0.0,  0.0, 1.0],
                                                           [ 0.0, -1.0, 0.0],
                                                           [ 1.0,  0.0, 0.0]]

def child2_mass_points_1B():
  components.child2_init.num_points = 1
  components.child2_init.points = \
                               trick.sim_services.alloc_type( 1,"jeod::MassPointInit")
  components.child2_init.points[0].set_name("top_to_bottom")
  components.child2_init.points[0].position  = [ 0.0, 0.5, 0.0]
  components.child2_init.points[0].pt_orientation.data_source = \
                                            trick.Orientation.InputEulerRotation
  components.child2_init.points[0].pt_orientation.euler_sequence = \
                                                trick.Orientation.Yaw_Pitch_Roll
  components.child2_init.points[0].pt_orientation.euler_angles = \
                                   trick.attach_units( "degree",[ 90.0, 0.0, 0.0])


def child2_mass_points_2():
  components.child2_init.num_points = 2
  components.child2_init.points = \
                          trick.sim_services.alloc_type( 2 , "jeod::MassPointInit" )
  components.child2_init.points[0].set_name("front_to_back")
  components.child2_init.points[0].position  = [ -0.5, 0.0, 0.0]
  components.child2_init.points[0].pt_orientation.data_source = \
                                              trick.Orientation.InputMatrix
  components.child2_init.points[0].pt_orientation.trans = [[ -1.0,  0.0, 0.0],
                                                           [  0.0, -1.0, 0.0],
                                                           [  0.0,  0.0, 1.0]]

  components.child2_init.points[1].set_name("ghost_front_to_back")
  components.child2_init.points[1].position  = [ -1.5, 0.0, 0.0]
  components.child2_init.points[1].pt_orientation.data_source = \
                                              trick.Orientation.InputMatrix
  components.child2_init.points[1].pt_orientation.trans = [[ -1.0,  0.0, 0.0],
                                                           [  0.0, -1.0, 0.0],
                                                           [  0.0,  0.0, 1.0]]
