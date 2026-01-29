def set_aero_drag_ballistic() :
  #/* Set up the aerodynamic drag parameters. */
  vehicle.aero_drag.ballistic_drag.option = trick.DefaultAero.DRAG_OPT_CD
  vehicle.aero_drag.ballistic_drag.Cd = 0.02
  vehicle.aero_drag.ballistic_drag.BC = 50.0
  vehicle.aero_drag.ballistic_drag.area = 1.0
  vehicle.aero_drag.ballistic_drag.drag = 3.14e-7



def set_aero_drag_iss () :
  set_aero_drag_ballistic()
  vehicle.aero_drag.ballistic_drag.Cd = 2.0
  vehicle.aero_drag.ballistic_drag.area = 1400.0
  

def set_aero_const_density_drag() :
  vehicle.aero_drag.constant_density = True
  vehicle.aero_drag.density = 1.4e-12
