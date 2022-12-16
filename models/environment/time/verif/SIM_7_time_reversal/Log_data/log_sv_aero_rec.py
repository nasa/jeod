#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for orbital vehicle aerodymanic data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
# 
#*/

def log_sv_aero_rec ( log_cycle , VEH_NAME , VEH_OBJ ) :
  recording_group_name =  ""+VEH_NAME+"_Aero"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.ellip_coords.altitude")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.ellip_coords.latitude")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.ellip_coords.longitude")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.sphere_coords.altitude")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.sphere_coords.latitude")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.sphere_coords.longitude")
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".pfix.state.cart_coords[" + str(ii) + "]" )
  dr_group.add_variable(   ""+VEH_OBJ+".atmos.density")
  dr_group.add_variable(   ""+VEH_OBJ+".atmos.temperature")
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".atmos.wind[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".aero_drag.aero_force[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".aero_drag.aero_torque[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
  return
