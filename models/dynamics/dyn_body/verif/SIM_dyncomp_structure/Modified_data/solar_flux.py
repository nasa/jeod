def set_solar_flux(level = "mean") :
  if (level not in ["min", "mean", "max"]):
    print("ERROR : invalid solar-flux specification.  Assuming mean.")
    set_solar_flux("mean")
    return
    
  #/* Set Flag to use Ap instead of Kp */
  earth.atmos.geo_index_type = trick.METAtmosphere.ATMOS_MET_GI_AP
  
  if (level == "max") :  
    earth.atmos.geo_index = 25.0
    earth.atmos.F10       = 250.0
    earth.atmos.F10B      = 250.0
  elif (level == "mean") :
    earth.atmos.geo_index = 15.7
    earth.atmos.F10       = 128.8
    earth.atmos.F10B      = 128.8
  else : # min
    earth.atmos.geo_index = 0.0
    earth.atmos.F10       = 70.0
    earth.atmos.F10B      = 70.0


# Set to "mean" by default:
set_solar_flux("mean")
