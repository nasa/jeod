#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (General log data for DSES Task 1 tests.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
#
#*/

def log_sv_state_ascii_rec ( log_cycle , VEH_NAME , VEH_OBJ ) :
  recording_group_name =  ""+VEH_NAME+"_ASCII"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".pfix.state.cart_coords[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.composite_body.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.composite_body.state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.derivs.trans_accel[" + str(ii) + "]" )
  dr_group.add_variable(   ""+VEH_OBJ+".body.composite_body.state.rot.Q_parent_this.scalar")
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.composite_body.state.rot.Q_parent_this.vector[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.composite_body.state.rot.ang_vel_this[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.derivs.rot_accel[" + str(ii) + "]" )
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.ellip_coords.altitude")
  dr_group.add_variable(   ""+VEH_OBJ+".atmos.density")
  dr_group.add_variable(   ""+VEH_OBJ+".atmos.temperature")
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.grav_interaction.grav_accel[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".grav_torque.torque[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".aero_drag.aero_force[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".aero_drag.aero_torque[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".force_extern.force[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".torque_extern.torque[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.collect.no_xmit_forc[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.collect.no_xmit_torq[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
  return
