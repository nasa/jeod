################TRICK HEADER#######################################
#PURPOSE:
#  (To define the elements to be logged in the logging of rec) 
####################################################################################

def log_state ( log_cycle ) :
  recording_group_name = "state" 
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable( "vehicle.dyn_body.composite_body.state.trans.position[" + str(ii) +"]")
    dr_group.add_variable( "vehicleB.dyn_body.composite_body.state.trans.position[" + str(ii) +"]")
    dr_group.add_variable( "vehicle.dyn_body.composite_body.state.trans.velocity[" + str(ii) +"]")
    dr_group.add_variable( "vehicleB.dyn_body.composite_body.state.trans.velocity[" + str(ii) +"]")
    dr_group.add_variable( "vehicleB.dyn_body.mass.composite_properties.position[" + str(ii) +"]")
    dr_group.add_variable( "interactions.veh_contact_surface.contact_force[" + str(ii) +"]")
    dr_group.add_variable( "interactions.vehB_contact_surface.contact_force[" + str(ii) +"]")
  dr_group.add_variable( "jeod_time.time_manager.dyn_time.seconds")


  trick.add_data_record_group(dr_group)

  return
