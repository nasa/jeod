################TRICK HEADER#######################################
#PURPOSE:
#  (To define the elements to be logged in the logging of rec) 
####################################################################################

def log_rec ( log_cycle ) :
  recording_group_name =  "time"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable( "jeod_time.time_manager.dyn_time.seconds")

  trick.add_data_record_group(dr_group)
  return
