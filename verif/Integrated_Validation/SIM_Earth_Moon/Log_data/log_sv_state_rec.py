#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for stateal vehicle state data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
# 
#*/

def log_add_state( frame, dr_group) :
  for ii in range(0,3) :
    dr_group.add_variable( frame + ".state.trans.position[" + str(ii) + "]" )
    dr_group.add_variable( frame + ".state.trans.velocity[" + str(ii) + "]" )
    for jj in range(0,3) :
      dr_group.add_variable(frame + ".state.rot.T_parent_this[" + str(ii) + "]["+str(jj) + "]" )
    dr_group.add_variable( frame + ".state.rot.ang_vel_this[" + str(ii) + "]" )
  return



def log_sv_state_rec ( log_cycle , VEH_NAME  ) :
  recording_group_name =  ""+VEH_NAME+"_State"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always

  log_add_state( "vehicle.dyn_body.composite_body", dr_group)
  log_add_state( "vehicle.dyn_body.core_body", dr_group)
  log_add_state( "vehicle.dyn_body.structure", dr_group)

  for ii in range(0,3) :
    dr_group.add_variable("vehicle.dyn_body.derivs.non_grav_accel[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.derivs.trans_accel[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.derivs.rot_accel[" + str(ii) + "]" )

    dr_group.add_variable("vehicle.pfix.state.cart_coords[" + str(ii) + "]" )

    dr_group.add_variable("vehicle.rad_pressure.force[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
  return
