################TRICK HEADER#######################################
#PURPOSE:
#  (To define the elements to be logged in the logging of rec) 
####################################################################################

def log_data_rec ( log_cycle ) :
  recording_group_name =  "state_profile"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always

  dr_group.add_variable( "vehicle.dyn_body.composite_body.state.rot.ang_vel_mag")
  dr_group.add_variable( "jeod_time.time_manager.dyn_time.seconds")
  dr_group.add_variable( "state.r_err")
  dr_group.add_variable( "state.v_err")
  dr_group.add_variable( "state.r_err_mag")
  dr_group.add_variable( "state.r_err_ave")
  dr_group.add_variable( "state.r_err_mag_ave")
  dr_group.add_variable( "state.log_r_err_mag_ave")

  for ii in range(0,2) :
    dr_group.add_variable("vehicle.dyn_body.mass.core_properties.Q_parent_this.vector[" + str(ii) + "]")
    dr_group.add_variable("vehicle.dyn_body.composite_body.state.trans.position["
    + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.composite_body.state.trans.velocity["
    + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
  return
