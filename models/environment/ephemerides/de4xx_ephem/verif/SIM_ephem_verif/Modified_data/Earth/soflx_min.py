#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   ()
#
#REFERENCE:
#   (((--) (--)(--) (--) (--) (--)))
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((Represents minimum solar activity values.))
#
#
#*/

def set_Earth_soflx_min(earth) :

  #/* Set Flag to use Ap instead of Kp */
  earth.atmos.geo_index_type = trick.METAtmosphere.ATMOS_MET_GI_AP

  earth.atmos.geo_index = 0.0
  earth.atmos.F10       = 70.0
  earth.atmos.F10B      = 70.0


  return
