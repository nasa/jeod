################TRICK HEADER#######################################
#PURPOSE:
#  (To define the elements to be logged in the logging of rec) 
####################################################################################

def log_state ( log_cycle ) :
  recording_group_name =  "state"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable( "relative.pointA_wrt_bodyB_in_lvlh_bodyB.rel_state.trans.position[" + str(ii) +"]")
    dr_group.add_variable( "relative.pointB_wrt_bodyB_in_lvlh_bodyB.rel_state.trans.position[" + str(ii) +"]")
    dr_group.add_variable( "relative.pointA_wrt_pointB_in_strucB.rel_state.trans.position[" + str(ii) +"]")
    dr_group.add_variable( "relative.bodyB_wrt_bodyA_in_bodyA.rel_state.trans.position[" + str(ii) +"]")
    dr_group.add_variable( "relative.bodyA_wrt_bodyB_in_lvlh_bodyB.rel_state.trans.position[" + str(ii) +"]")
    dr_group.add_variable( "relative.bodyB_wrt_bodyA_in_lvlh_bodyA.rel_state.trans.position[" + str(ii) +"]")
    dr_group.add_variable( "relative.bodyA_wrt_strucB_in_curvi_lvlh_strucB.rel_state.trans.position[" + str(ii) +"]")
    dr_group.add_variable( "relative.bodyB_wrt_strucA_in_curvi_lvlh_strucA.rel_state.trans.position[" + str(ii) +"]")
    dr_group.add_variable( "relative.bodyB_wrt_strucA_in_lvlh_strucA.rel_state.trans.position[" + str(ii) +"]")

  trick.add_data_record_group(dr_group)

  return
