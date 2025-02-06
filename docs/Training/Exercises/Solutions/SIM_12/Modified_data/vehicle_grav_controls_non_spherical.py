def veh_grav_controls(degree,order) :
  vehicle.grav_control.source_name = "Earth"
  vehicle.grav_control.active = True
  vehicle.grav_control.spherical = False
  vehicle.grav_control.degree = degree
  vehicle.grav_control.order = order

  vehicle.dyn_body.grav_interaction.add_control(vehicle.grav_control)

  return
