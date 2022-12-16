def veh_grav_controls(degree, order) :
  vehicle.grav_control.source_name = "Earth"
  vehicle.grav_control.active = True
  vehicle.grav_control.spherical = False
  vehicle.grav_control.degree = degree
  vehicle.grav_control.order = order

  vehicle.sbtide_ctrl.first_order_only = True
  vehicle.sbtide_ctrl.grav_effect = earth.sb_tide
  vehicle.sbtide_ctrl.grav_source = earth.gravity_source
  vehicle.sbtide_ctrl.active = True

  vehicle.grav_control.add_deltacontrol(vehicle.sbtide_ctrl)
  vehicle.dyn_body.grav_interaction.add_control(vehicle.grav_control)

  return
