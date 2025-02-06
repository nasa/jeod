#Auto-generated from the data file with this header:
#/*******************************************************************************
#PURPOSE:
#   (Log data recording file for planet-fixed position model verification)
#REFERENCES:
#   (None)
#ASSUMPTIONS AND LIMITATIONS:
#   (None)
# 
#*******************************************************************************/

def log_pfixposn_verif ( log_cycle ) :
  recording_group_name =  "pfixposn_verif"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable("earth.cartesian_pos[" + str(ii) + "]" )
  dr_group.add_variable(   "earth.spherical_pos.altitude")
  dr_group.add_variable(   "earth.spherical_pos.latitude")
  dr_group.add_variable(   "earth.spherical_pos.longitude")
  dr_group.add_variable(   "earth.elliptical_pos.altitude")
  dr_group.add_variable(   "earth.elliptical_pos.latitude")
  dr_group.add_variable(   "earth.elliptical_pos.longitude")
  for ii in range(0,3) :
    dr_group.add_variable("earth.pfix_pos.cart_coords[" + str(ii) + "]" )
  dr_group.add_variable(   "earth.pfix_pos.sphere_coords.altitude")
  dr_group.add_variable(   "earth.pfix_pos.sphere_coords.latitude")
  dr_group.add_variable(   "earth.pfix_pos.sphere_coords.longitude")
  dr_group.add_variable(   "earth.pfix_pos.ellip_coords.altitude")
  dr_group.add_variable(   "earth.pfix_pos.ellip_coords.latitude")
  dr_group.add_variable(   "earth.pfix_pos.ellip_coords.longitude")

  trick.add_data_record_group(dr_group)
  return
