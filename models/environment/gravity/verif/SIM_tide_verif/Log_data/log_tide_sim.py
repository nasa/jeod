def log_3vec( drg, var):
  for ii in range(3):
    drg.add_variable( var + "[" + str(ii) + "]")

dr_group = trick.sim_services.DRBinary("tide_verif_data")
dr_group.set_cycle(60)
dr_group.freq = trick.sim_services.DR_Always

log_3vec( dr_group, "sv_dyn.dyn_body.derivs.rot_accel" )
log_3vec( dr_group, "sv_dyn.dyn_body.derivs.trans_accel" )
log_3vec( dr_group, "sv_dyn.dyn_body.collect.inertial_torq" )

dr_group.add_variable( "sv_dyn.dyn_body.grav_interaction.grav_pot")
log_3vec( dr_group, "sv_dyn.dyn_body.grav_interaction.grav_accel" )
log_3vec( dr_group, "sv_dyn.grav_torque.torque" )

log_3vec( dr_group, "sv_dyn.dyn_body.composite_body.state.trans.position" )
log_3vec( dr_group, "sv_dyn.dyn_body.composite_body.state.trans.velocity" )
log_3vec( dr_group, "sv_dyn.dyn_body.composite_body.state.rot.ang_vel_this" )
dr_group.add_variable( "sv_dyn.dyn_body.derivs.Qdot_parent_this.scalar")
log_3vec( dr_group, "sv_dyn.dyn_body.derivs.Qdot_parent_this.vector" )
log_3vec( dr_group, "sv_dyn.pfix.state.cart_coords" )

dr_group.add_variable( "earth.sb_tide.dC20" )

trick.add_data_record_group(dr_group, trick.DR_Buffer)
