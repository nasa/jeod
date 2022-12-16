def set_rad_pressure( name) :
  # defaults
  vehicle.rad_surface.albedo = 1.0
  vehicle.rad_surface.diffuse = .27
  vehicle.rad_surface.temperature = 0.0
  vehicle.rad_surface.thermal.active = False
  vehicle.rad_surface.thermal.thermal_power_dump = 0.0
  vehicle.rad_surface.thermal.emissivity = 1.0e-12
  vehicle.rad_surface.thermal.heat_capacity = 0.0


  if name == "clementine" :
    vehicle.rad_surface.cx_area  = trick.attach_units( "m2",2.1432)

  elif name == "rosetta" :
    vehicle.rad_surface.cx_area  = trick.attach_units( "m2",20.0)
    vehicle.rad_pressure.active =  False

  return
