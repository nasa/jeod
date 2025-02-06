#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   ()
#
#REFERENCE:
#   (((--) (--)(--) (--) (--) (--)))
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((Represents mean solar activity values.))
#
# 
#*/

def set_Earth_soflx_mean(earth) :

  #/* Set Flag to use Ap instead of Kp */
  earth.atmos.geo_index_type = trick.METAtmosphere.ATMOS_MET_GI_AP

  earth.atmos.geo_index = 15.7
  earth.atmos.F10       = 128.8
  earth.atmos.F10B      = 128.8
