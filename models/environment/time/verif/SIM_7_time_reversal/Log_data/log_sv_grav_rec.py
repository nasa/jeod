#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for orbital vehicle gravity data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
# 
#*/

def log_sv_grav_rec ( log_cycle , VEH_NAME , VEH_OBJ ) :
  recording_group_name =  ""+VEH_NAME+"_Grav"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".pfix.state.cart_coords[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.grav_interaction.grav_accel[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".grav_torque.torque[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
  return
