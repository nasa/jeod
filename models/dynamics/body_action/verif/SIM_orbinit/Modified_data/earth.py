################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_earth
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
# *
# * Purpose:
# *   Initialize Earth parameters corresponding to STS_114 MET 001:19:30:59.000.
# *
# *
# *
# * Date:
# *   June, 2006
# *
# ******************************************************************************/

def set_earth(jeod_time, earth, dynamics) :

  #/* Initialize the time module to the epoch time. */
  jeod_time.time_manager_init.initializer = "UT1"
  jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

  jeod_time.time_ut1.set_date_and_time(2005, 7, 28, 10, 9, 59.0)

  jeod_time.time_tai.initialize_from_name = "UT1"
  jeod_time.time_utc.initialize_from_name = "TAI"
  jeod_time.time_tt.initialize_from_name  = "TAI"

  jeod_time.time_tai.update_from_name = "Dyn"
  jeod_time.time_ut1.update_from_name = "TAI"
  jeod_time.time_utc.update_from_name = "TAI"
  jeod_time.time_tt.update_from_name  = "TAI"

  jeod_time.time_utc.true_utc = False
  jeod_time.time_ut1.true_ut1 = False

  jeod_time.time_gmst.update_from_name = "UT1"
  jeod_time.time_gmst.initialize_from_name = "UT1"


  #/* Initialize the RNP model */
  earth.rnp.enable_polar = False

  #/* Initialize the dynamics */
  dynamics.dyn_manager_init.mode = \
    trick.DynManagerInit.EphemerisMode_SinglePlanet
  dynamics.dyn_manager_init.central_point_name = "Earth"

  return
