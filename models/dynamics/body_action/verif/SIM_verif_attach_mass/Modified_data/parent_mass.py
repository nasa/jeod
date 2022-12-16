def parent_mass_default():
  components.parent_body.set_name("Parent")
  components.parent_init.subject     = components.parent_body
  components.parent_init.action_name = "Parent_mass_init"
  components.parent_init.properties.mass  = 1.0
  components.parent_init.properties.position  = [ 0.0, 0.0, 0.0]
  components.parent_init.properties.inertia_spec = trick.MassPropertiesInit.Body
  components.parent_init.properties.inertia  = [
                                         [ 0.41666667, 0.0       , 0.0],
                                         [ 0.0       , 0.41666667, 0.0],
                                         [ 0.0       , 0.0       , 0.1666667]]
  components.parent_init.properties.pt_orientation.data_source = \
                                             trick.Orientation.InputQuaternion
  dynamics.dyn_manager.add_body_action (components.parent_init)

def parent_mass_inertia_optionA():
  components.parent_init.properties.inertia_spec = trick.MassPropertiesInit.Struct
  components.parent_init.properties.inertia  = [
                               [  1.33333333, 0.0,       -0.5],
                               [  0.0,        1.66666667, 0.0],
                               [ -0.5,        0.0,        0.33333333]]

def parent_mass_inertia_optionB():
  components.parent_init.properties.inertia_spec = trick.MassPropertiesInit.StructCG
  components.parent_init.properties.inertia = [
                                 [ 0.33333333, 0.0,        0.0],
                                 [ 0.0,        0.41666667, 0.0],
                                 [ 0.0,        0.0,        0.0833333333]]

def parent_mass_inertia_optionC():
  components.parent_init.properties.inertia_spec = trick.MassPropertiesInit.Body
  components.parent_init.properties.inertia = [
                                 [ 0.33333333, 0.0,        0.0],
                                 [ 0.0,        0.41666667, 0.0],
                                 [ 0.0,        0.0,        0.0833333333]]


def parent_mass_orientation_optionA():
  components.parent_init.properties.pt_orientation.data_source = \
                                            trick.Orientation.InputMatrix
  components.parent_init.properties.pt_orientation.trans = [
                 [ .8660254,          -0.5,              0.0],
                 [ 0.35355339059327,   0.61237243569579, 0.70710678118655],
                 [ -0.35355339059327, -0.61237243569579, 0.70710678118655]]


def parent_mass_points_1():
  components.parent_init.num_points = 1
  components.parent_init.points = \
                          trick.sim_services.alloc_type(1, "jeod::MassPointInit")
  components.parent_init.points[0].set_name("top_to_right")
  components.parent_init.points[0].position  = [ 0.0, 0.5, 0.0]
  components.parent_init.points[0].pt_orientation.data_source = \
                                                trick.Orientation.InputMatrix
  components.parent_init.points[0].pt_orientation.trans = [[ 0.0, 1.0, 0.0],
                                                           [ 0.0, 0.0, 1.0],
                                                           [ 1.0, 0.0, 0.0]]

def parent_mass_points_2():
  components.parent_init.num_points = 2
  components.parent_init.points = \
                         trick.sim_services.alloc_type( 2 , "jeod::MassPointInit" )
  components.parent_init.points[0].set_name("bottom_to_top")
  components.parent_init.points[0].position  = [ 0.0, -0.5, 0.0]
  components.parent_init.points[0].pt_orientation.data_source = \
                                          trick.Orientation.InputEulerRotation
  components.parent_init.points[0].pt_orientation.euler_sequence = \
                                          trick.Orientation.Yaw_Pitch_Roll
  components.parent_init.points[0].pt_orientation.euler_angles  = \
                                    trick.attach_units( "degree",[ -90.0, 0.0, 0.0])

  components.parent_init.points[1].set_name("top_to_bottom")
  components.parent_init.points[1].position  = [ 0.0, 0.5, 0.0]
  components.parent_init.points[1].pt_orientation.data_source = \
                                          trick.Orientation.InputEulerRotation
  components.parent_init.points[1].pt_orientation.euler_sequence = \
                                          trick.Orientation.Yaw_Pitch_Roll
  components.parent_init.points[1].pt_orientation.euler_angles  = \
                                    trick.attach_units( "degree",[ 90.0, 0.0, 0.0])


def parent_mass_points_3():
  components.parent_init.num_points = 3
  components.parent_init.points = \
                         trick.sim_services.alloc_type( 3 , "jeod::MassPointInit" )
  components.parent_init.points[0].set_name("front_to_back")
  components.parent_init.points[0].position  = [ -0.5, 0.0, 0.0]
  components.parent_init.points[0].pt_orientation.data_source = \
                                                trick.Orientation.InputMatrix
  components.parent_init.points[0].pt_orientation.trans = [[ -1.0,  0.0, 0.0],
                                                           [  0.0, -1.0, 0.0],
                                                           [  0.0,  0.0, 1.0]]

  components.parent_init.points[1].set_name("back_to_front")
  components.parent_init.points[1].position  = [ 0.5, 0.0, 0.0]
  components.parent_init.points[1].pt_orientation.data_source = \
                                                trick.Orientation.InputMatrix
  components.parent_init.points[1].pt_orientation.trans = [[ 1.0, 0.0, 0.0],
                                                           [ 0.0, 1.0, 0.0],
                                                           [ 0.0, 0.0, 1.0]]

  components.parent_init.points[2].set_name("parent_to_child2")
  components.parent_init.points[2].position  = [ 2.5, 0.0, 1.0]
  components.parent_init.points[2].pt_orientation.data_source = \
                                                trick.Orientation.InputMatrix
  components.parent_init.points[2].pt_orientation.trans = [[ 1.0, 0.0,  0.0],
                                                           [ 0.0, 0.0, -1.0],
                                                           [ 0.0, 1.0,  0.0]]
  components.parent_init.points[2].pt_orientation.trans[2]  = [ 0.0, 1.0,  0.0]
