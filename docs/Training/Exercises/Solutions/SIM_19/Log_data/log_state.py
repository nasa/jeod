################TRICK HEADER#######################################
#PURPOSE:
#  (To define the elements to be logged in the logging of rec) 
####################################################################################

def log_state ( log_cycle ) :
  recording_group_name = "mass"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always

  dr_group.add_variable( "vehicleA.body.composite_properties.mass")
  dr_group.add_variable( "vehicleA.body.core_properties.mass")
  dr_group.add_variable( "vehicleB.body.composite_properties.mass")
  dr_group.add_variable( "vehicleB.body.core_properties.mass")
  dr_group.add_variable( "vehicleA.tank.composite_properties.mass")
  dr_group.add_variable( "vehicleA.tank.core_properties.mass")
  dr_group.add_variable( "vehicleB.tank.composite_properties.mass")
  dr_group.add_variable( "vehicleB.tank.core_properties.mass")

  trick.add_data_record_group(dr_group)

  return
