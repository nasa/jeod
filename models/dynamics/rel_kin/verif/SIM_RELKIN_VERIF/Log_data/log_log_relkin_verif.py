#Auto-generated from the data file with this header:
#/*******************************************************************************
#PURPOSE:
#   (Log data recording file for relative kinematics model verification)
#REFERENCES:
#   (None)
#ASSUMPTIONS AND LIMITATIONS:
#   (None)
#
#*******************************************************************************/

def log_log_relkin_verif ( log_cycle ) :
  recording_group_name =  "relkin_verif"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable("veh1.dyn_body.composite_body.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("veh1.dyn_body.composite_body.state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("veh1.dyn_body.composite_body.state.rot.T_parent_this[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("veh1.dyn_body.composite_body.state.rot.T_parent_this[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("veh1.dyn_body.composite_body.state.rot.T_parent_this[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("veh1.dyn_body.composite_body.state.rot.ang_vel_this[" + str(ii) + "]" )
  dr_group.add_variable(   "veh1.dyn_body.composite_body.state.rot.ang_vel_mag")
  for ii in range(0,3) :
    dr_group.add_variable("veh2.dyn_body.composite_body.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("veh2.dyn_body.composite_body.state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("veh2.dyn_body.composite_body.state.rot.T_parent_this[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("veh2.dyn_body.composite_body.state.rot.T_parent_this[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("veh2.dyn_body.composite_body.state.rot.T_parent_this[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("veh2.dyn_body.composite_body.state.rot.ang_vel_this[" + str(ii) + "]" )
  dr_group.add_variable(   "veh2.dyn_body.composite_body.state.rot.ang_vel_mag")
  for ii in range(0,3) :
    dr_group.add_variable("relkin.cm_relstate.rel_state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.cm_relstate.rel_state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.cm_relstate.rel_state.rot.T_parent_this[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.cm_relstate.rel_state.rot.T_parent_this[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.cm_relstate.rel_state.rot.T_parent_this[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.cm_relstate.rel_state.rot.ang_vel_this[" + str(ii) + "]" )
  dr_group.add_variable(   "relkin.cm_relstate.rel_state.rot.ang_vel_mag")
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor1_relstate.rel_state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor1_relstate.rel_state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor1_relstate.rel_state.rot.T_parent_this[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor1_relstate.rel_state.rot.T_parent_this[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor1_relstate.rel_state.rot.T_parent_this[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor1_relstate.rel_state.rot.ang_vel_this[" + str(ii) + "]" )
  dr_group.add_variable(   "relkin.sensor1_relstate.rel_state.rot.ang_vel_mag")
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor2_relstate.rel_state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor2_relstate.rel_state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor2_relstate.rel_state.rot.T_parent_this[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor2_relstate.rel_state.rot.T_parent_this[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor2_relstate.rel_state.rot.T_parent_this[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("relkin.sensor2_relstate.rel_state.rot.ang_vel_this[" + str(ii) + "]" )
  dr_group.add_variable(   "relkin.sensor2_relstate.rel_state.rot.ang_vel_mag")

  trick.add_data_record_group(dr_group)
  return
