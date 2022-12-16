def set_solar(name) :

  #/* Set Flag to use Ap instead of Kp */
  if name == "rosetta" :
    earth.atmos.geo_index_type = trick.METAtmosphere.ATMOS_MET_GI_AP
    earth.atmos.geo_index =  0.0
    earth.atmos.F10       =  0.0
    earth.atmos.F10B      =  0.0
  #else atmosphere is off anyway.
  
  return
