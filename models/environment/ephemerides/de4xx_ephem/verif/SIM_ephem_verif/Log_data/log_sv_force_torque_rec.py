#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for orbital vehicle force and torque data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
#
#*/

def log_sv_force_torque_rec ( log_cycle , VEH_NAME , VEH_OBJ ) :
  recording_group_name =  ""+VEH_NAME+"_Force_Torque"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.collect.effector_forc[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.collect.environ_forc[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.collect.no_xmit_forc[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.derivs.trans_accel[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.derivs.non_grav_accel[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.collect.effector_torq[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.collect.environ_torq[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.collect.no_xmit_torq[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.collect.inertial_torq[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.composite_body.state.rot.ang_vel_this[" + str(ii) + "]" )
  dr_group.add_variable(   ""+VEH_OBJ+".body.derivs.Qdot_parent_this.scalar")
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.derivs.Qdot_parent_this.vector[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".body.derivs.rot_accel[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
  return
