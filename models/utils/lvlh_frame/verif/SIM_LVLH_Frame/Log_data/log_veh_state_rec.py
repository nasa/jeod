############################## TRICK HEADER ########################################
# PURPOSE:
#  (To define the logging function log_veh_state_rec
####################################################################################

try:
   DR_GROUP_ID
except NameError:
   def initialize_globals():
      global DR_GROUP_ID
      global drg
      DR_GROUP_ID = 0
      drg = []
   initialize_globals()

def log_veh_state_rec ( log_cycle ) :
  recording_group_name =  "VehState"
  global DR_GROUP_ID
  global drg
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable("vehicle.body.composite_body.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("vehicle.body.composite_body.state.trans.velocity[" + str(ii) + "]" )

  dr_group.add_variable("vehicle.lvlh_frame.frame.state.rot.Q_parent_this.scalar")
  for ii in range(0,3) :
    dr_group.add_variable("vehicle.lvlh_frame.frame.state.rot.Q_parent_this.vector[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("vehicle.body.composite_body.state.rot.ang_vel_this[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
  DR_GROUP_ID += 1

  return
