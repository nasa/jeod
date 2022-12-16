def child3_mass_default():
  components.child3_body.set_name("Child3")
  components.child3_init.subject     = components.child3_body
  components.child3_init.action_name = "Child3_mass_init"
  components.child3_init.properties.mass  = 1.0
  components.child3_init.properties.position  = [ 0.0, 0.0, 0.0]

  components.child3_init.properties.inertia_spec = trick.MassPropertiesInit.Body
  components.child3_init.properties.inertia = [
                                       [ 0.33333333, 0.0,        0.0],
                                       [ 0.0,        0.41666667, 0.0],
                                       [ 0.0,        0.0,        0.0833333333]]
  components.child3_init.properties.pt_orientation.data_source = \
                                             trick.Orientation.InputQuaternion

  dynamics.dyn_manager.add_body_action (components.child3_init)

def child3_mass_points_1():
  components.child3_init.num_points = 1
  components.child3_init.points = \
                          trick.sim_services.alloc_type( 2 , "jeod::MassPointInit" )
  components.child3_init.points[0].set_name("front_to_back")
  components.child3_init.points[0].position  = [ -0.5, 0.0, 0.0]
  components.child3_init.points[0].pt_orientation.data_source = \
                                              trick.Orientation.InputMatrix
  components.child3_init.points[0].pt_orientation.trans = [[ -1.0,  0.0, 0.0],
                                                           [  0.0, -1.0, 0.0],
                                                           [  0.0,  0.0, 1.0]]


def child3_mass_points_2():
  components.child3_init.num_points = 2
  components.child3_init.points = \
                          trick.sim_services.alloc_type( 2 , "jeod::MassPointInit" )
  components.child3_init.points[0].set_name("front_to_back")
  components.child3_init.points[0].position  = [ -0.5, 0.0, 0.0]
  components.child3_init.points[0].pt_orientation.data_source = \
                                              trick.Orientation.InputMatrix
  components.child3_init.points[0].pt_orientation.trans = [[ -1.0,  0.0, 0.0],
                                                           [  0.0, -1.0, 0.0],
                                                           [  0.0,  0.0, 1.0]]

  components.child3_init.points[1].set_name("back_to_front")
  components.child3_init.points[1].position  = [ 0.5, 0.0, 0.0]
  components.child3_init.points[1].pt_orientation.data_source = \
                                              trick.Orientation.InputMatrix
  components.child3_init.points[1].pt_orientation.trans = [[ 1.0, 0.0, 0.0],
                                                           [ 0.0, 1.0, 0.0],
                                                           [ 0.0, 0.0, 1.0]]
