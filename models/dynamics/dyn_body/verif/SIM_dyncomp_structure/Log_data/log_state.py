###############################################################################
###################          State Blocks         #############################
###############################################################################
def log_add_derivs( dr_group) :
  for ii in range(0,3) :
    dr_group.add_variable("vehicle.dyn_body.derivs.non_grav_accel[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.derivs.trans_accel[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.derivs.rot_accel[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.derivs.Qdot_parent_this.vector[" + str(ii) + "]" )
  dr_group.add_variable("vehicle.dyn_body.derivs.Qdot_parent_this.scalar")


def log_add_ref_frame_state( dr_group, ref_frame):
  for ii in range(0,3) :
    dr_group.add_variable( ref_frame+".state.trans.position[" + str(ii) + "]" )
    dr_group.add_variable( ref_frame+".state.trans.velocity[" + str(ii) + "]" )
    dr_group.add_variable( ref_frame+".state.rot.ang_vel_this[" + str(ii) + "]" )
    for jj in range(0,3) :
      dr_group.add_variable(ref_frame+".state.rot.T_parent_this["+str(ii)+"]["+str(jj)+"]")
    dr_group.add_variable( ref_frame+".state.rot.Q_parent_this.vector[" + str(ii) + "]" )
  dr_group.add_variable( ref_frame+".state.rot.Q_parent_this.scalar")
  

def log_add_state ( dr_group ):
  log_add_ref_frame_state( dr_group, "vehicle.dyn_body.composite_body")
  log_add_ref_frame_state( dr_group, "vehicle.dyn_body.core_body")
  log_add_ref_frame_state( dr_group, "vehicle.dyn_body.structure")
  log_add_derivs( dr_group)


def log_add_euler_angles( dr_group) :
  for ii in range(0,3) :
    dr_group.add_variable("vehicle.euler.body_ref_angles[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.euler.ref_body_angles[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.lvlh_euler.body_ref_angles[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.lvlh_euler.ref_body_angles[" + str(ii) + "]" )


def log_add_pfix_state( dr_group) :
  dr_group.add_variable(   "vehicle.pfix.state.ellip_coords.altitude")
  dr_group.add_variable(   "vehicle.pfix.state.ellip_coords.latitude")
  dr_group.add_variable(   "vehicle.pfix.state.ellip_coords.longitude")
  dr_group.add_variable(   "vehicle.pfix.state.sphere_coords.altitude")
  dr_group.add_variable(   "vehicle.pfix.state.sphere_coords.latitude")
  dr_group.add_variable(   "vehicle.pfix.state.sphere_coords.longitude")
  for ii in range(0,3) :
    dr_group.add_variable( "vehicle.pfix.state.cart_coords[" + str(ii) + "]" )

###############################################################################
###################          Log groups           #############################
###############################################################################

def log_euler_angles (log_cycle) :
  dr_group = trick.sim_services.DRBinary("euler_angles")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  log_add_euler_angles( dr_group)
  trick.add_data_record_group(dr_group)

def log_pfix_state (log_cycle) :
  dr_group = trick.sim_services.DRBinary("pfix")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  log_add_pfix_state( dr_group)
  trick.add_data_record_group(dr_group)



def log_state ( log_cycle ):
  dr_group = trick.sim_services.DRBinary("state")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  log_add_state( dr_group)
  trick.add_data_record_group(dr_group)


def log_state_ascii ( log_cycle ):
  dr_group = trick.sim_services.DRAscii( "state_ASCII")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  log_add_state( dr_group)
  trick.add_data_record_group(dr_group)


def log_state_ascii_compare ( log_cycle, name ):
  dr_group = trick.sim_services.DRAscii( name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  log_add_state( dr_group)
  trick.add_data_record_group(dr_group)
  return dr_group
