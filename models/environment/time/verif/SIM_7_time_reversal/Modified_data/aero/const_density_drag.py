def set_aero_const_density_drag(veh_obj_reference) :

  #/* Turn on the general aerodynamics interface. */
  veh_obj_reference.aero_drag.active = 1
  veh_obj_reference.aero_drag.constant_density = 1
  veh_obj_reference.aero_drag.density = 1.4e-12

  #/* Set up the aerodynamic drag parameters. */
  veh_obj_reference.aero_drag.ballistic_drag.option = trick.DefaultAero.DRAG_OPT_CD
  veh_obj_reference.aero_drag.ballistic_drag.Cd = 0.02
  veh_obj_reference.aero_drag.ballistic_drag.BC = 50.0
  veh_obj_reference.aero_drag.ballistic_drag.area = 1.0
  veh_obj_reference.aero_drag.ballistic_drag.drag = 3.14e-7
