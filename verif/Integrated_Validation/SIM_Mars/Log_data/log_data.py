def log_ascii_rec ( log_cycle , VEH_NAME ) :
  dr_group = trick.sim_services.DRAscii(VEH_NAME+"_ASCII")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable("dawn.dyn_body.composite_body.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("dawn.dyn_body.composite_body.state.trans.velocity[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)

def log_orb_rec ( log_cycle , VEH_NAME) :
  dr_group = trick.sim_services.DRBinary(VEH_NAME+"_Orb_Elem")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   "dawn.orb_elem.elements.semi_major_axis")
  dr_group.add_variable(   "dawn.orb_elem.elements.semiparam")
  dr_group.add_variable(   "dawn.orb_elem.elements.e_mag")
  dr_group.add_variable(   "dawn.orb_elem.elements.inclination")
  dr_group.add_variable(   "dawn.orb_elem.elements.arg_periapsis")
  dr_group.add_variable(   "dawn.orb_elem.elements.long_asc_node")
  dr_group.add_variable(   "dawn.orb_elem.elements.r_mag")
  dr_group.add_variable(   "dawn.orb_elem.elements.vel_mag")
  dr_group.add_variable(   "dawn.orb_elem.elements.true_anom")
  dr_group.add_variable(   "dawn.orb_elem.elements.mean_motion")
  dr_group.add_variable(   "dawn.orb_elem.elements.orbital_anom")
  dr_group.add_variable(   "dawn.orb_elem.elements.orb_energy")
  dr_group.add_variable(   "dawn.orb_elem.elements.orb_ang_momentum")

  trick.add_data_record_group(dr_group)

def log_trans_state_rec ( log_cycle , VEH_NAME) :
  dr_group = trick.sim_services.DRBinary(VEH_NAME+"_State")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable("dawn.body.core_body.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("dawn.body.core_body.state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("dawn.body.composite_body.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("dawn.body.composite_body.state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("dawn.body.derivs.non_grav_accel[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("dawn.body.derivs.trans_accel[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
