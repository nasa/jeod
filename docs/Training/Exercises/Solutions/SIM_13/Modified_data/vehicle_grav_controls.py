def veh_grav_controls(degree, order, moon_active=False, sun_active=False) :
  vehicle.earth_grav_control.source_name = "Earth"
  vehicle.earth_grav_control.active = True
  vehicle.earth_grav_control.spherical = False
  vehicle.earth_grav_control.degree = degree
  vehicle.earth_grav_control.order = order

  vehicle.moon_grav_control.source_name = "Moon"
  vehicle.moon_grav_control.active = moon_active
  vehicle.moon_grav_control.spherical = True

  vehicle.sun_grav_control.source_name = "Sun"
  vehicle.sun_grav_control.active = sun_active
  vehicle.sun_grav_control.spherical = True


  vehicle.dyn_body.grav_interaction.add_control(vehicle.earth_grav_control)
  vehicle.dyn_body.grav_interaction.add_control(vehicle.moon_grav_control)
  vehicle.dyn_body.grav_interaction.add_control(vehicle.sun_grav_control)

  return
