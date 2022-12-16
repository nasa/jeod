#Auto-generated from the data file with this header:
#/*******************************************************************************
#PURPOSE:
#   (Log data recording file for Planet model verification)
#REFERENCES:
#   (None)
#ASSUMPTIONS AND LIMITATIONS:
#   (None)
#
#*******************************************************************************/

def log_planet_verif ( log_cycle ) :
  recording_group_name =  "planet_verif"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   "earth.planet.r_eq")
  dr_group.add_variable(   "earth.planet.r_pol")
  dr_group.add_variable(   "earth.planet.e_ellipsoid")
  dr_group.add_variable(   "earth.planet.e_ellip_sq")
  dr_group.add_variable(   "earth.planet.flat_coeff")
  dr_group.add_variable(   "earth.planet.flat_inv")
  dr_group.add_variable(   "earth.gravity_source.radius")

  trick.add_data_record_group(dr_group)
  return
