#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for orbital vehicle state data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
#
#*/

def log_sv_rot_state_rec ( log_cycle , VEH_NAME , VEH_OBJ ) :
  recording_group_name =  ""+VEH_NAME+"_Rot_State"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   ""+VEH_OBJ+".body.composite_body.state.rot.Q_parent_this.scalar")
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.composite_body.state.rot.Q_parent_this.vector[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.composite_body.state.rot.T_parent_this[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.composite_body.state.rot.T_parent_this[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.composite_body.state.rot.T_parent_this[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.composite_body.state.rot.ang_vel_this[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.derivs.rot_accel[" + str(ii) + "]" )
  dr_group.add_variable(   ""+VEH_OBJ+".body.core_body.state.rot.Q_parent_this.scalar")
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.core_body.state.rot.Q_parent_this.vector[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.core_body.state.rot.T_parent_this[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.core_body.state.rot.T_parent_this[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.core_body.state.rot.T_parent_this[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.core_body.state.rot.ang_vel_this[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".euler.body_ref_angles[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".euler.ref_body_angles[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".lvlh_euler.body_ref_angles[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".lvlh_euler.ref_body_angles[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
  return
