#Auto-generated from the data file with this header:
#/*******************************************************************************
#PURPOSE:
#   (Log data recording file for North-East-Down frame verification)
#REFERENCES:
#   (None)
#ASSUMPTIONS AND LIMITATIONS:
#   (None)
# 
#*******************************************************************************/

def log_ned_verif ( log_cycle ) :
  recording_group_name =  "ned_verif"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable("earth.cartesian_pos[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.cartesian_vel[" + str(ii) + "]" )
  dr_group.add_variable(   "earth.spherical_pos.altitude")
  dr_group.add_variable(   "earth.spherical_pos.latitude")
  dr_group.add_variable(   "earth.spherical_pos.longitude")
  dr_group.add_variable(   "earth.elliptical_pos.altitude")
  dr_group.add_variable(   "earth.elliptical_pos.latitude")
  dr_group.add_variable(   "earth.elliptical_pos.longitude")
  for ii in range(0,3) :
    dr_group.add_variable("earth.ned_frame.cart_coords[" + str(ii) + "]" )
  dr_group.add_variable(   "earth.ned_frame.sphere_coords.altitude")
  dr_group.add_variable(   "earth.ned_frame.sphere_coords.latitude")
  dr_group.add_variable(   "earth.ned_frame.sphere_coords.longitude")
  dr_group.add_variable(   "earth.ned_frame.ellip_coords.altitude")
  dr_group.add_variable(   "earth.ned_frame.ellip_coords.latitude")
  dr_group.add_variable(   "earth.ned_frame.ellip_coords.longitude")
  for ii in range(0,3) :
    dr_group.add_variable("earth.ned_frame.ned_frame.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.ned_frame.ned_frame.state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.ned_frame.ned_frame.state.rot.T_parent_this[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.ned_frame.ned_frame.state.rot.T_parent_this[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.ned_frame.ned_frame.state.rot.T_parent_this[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.ned_frame.ned_frame.state.rot.ang_vel_this[" + str(ii) + "]" )
  dr_group.add_variable(   "earth.ned_frame.ned_frame.state.rot.ang_vel_mag")
  dr_group.add_variable(   "earth.ned_frame.altlatlong_type")

  trick.add_data_record_group(dr_group)
  return
