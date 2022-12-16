#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   ()
#
#REFERENCE:
#   (((--) (--)(--) (--) (--) (--)))
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((Represents maximum solar activity values.))
#
# 
#*/

def set_Earth_soflx_max(earth) :

  #/* Set Flag to use Ap instead of Kp */
  earth.atmos.geo_index_type = trick.METAtmosphere.ATMOS_MET_GI_AP

  earth.atmos.geo_index = 25.0
  earth.atmos.F10       = 250.0
  earth.atmos.F10B      = 250.0


  return
