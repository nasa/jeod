#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#  (Log data recording template for sim_interface verification)
#
#REFERENCES:
#  (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#  (None)
#
# 
#*/

def log_private_fields_ascii_rec ( log_cycle ) :
  recording_group_name =  "SININT_VER"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   "sim_interface_test.jcd.total_job_cycle_time")

  trick.add_data_record_group(dr_group)
  return
