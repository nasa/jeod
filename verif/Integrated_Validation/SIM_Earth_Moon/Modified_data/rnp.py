def set_rnp(name) :

  if name == "clementine" :
    vehicle.pfix.reference_name = "Moon"
    vehicle.lvlh.reference_name = "Moon"
    earth.rnp.rnp_type = trick.PlanetRNP.RotationOnly
    earth.rnp.enable_polar = False

  elif name == "rosetta" :
    earth.rnp.enable_polar = True

  return
