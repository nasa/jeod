def log_3vec( drg, var):
  for ii in range(3):
    drg.add_variable( var + "[" + str(ii) + "]")

dr_group = trick.sim_services.DRBinary("tide_verif_data")
dr_group.set_cycle(60)
dr_group.freq = trick.sim_services.DR_Always

log_3vec( dr_group, "sv_dyn.dyn_body.collect.no_xmit_forc" )
log_3vec( dr_group, "sv_dyn.dyn_body.derivs.trans_accel" )
log_3vec( dr_group, "sv_dyn.dyn_body.derivs.non_grav_accel" )
log_3vec( dr_group, "sv_dyn.dyn_body.collect.no_xmit_torq" )
log_3vec( dr_group, "sv_dyn.dyn_body.collect.inertial_torq" )
log_3vec( dr_group, "sv_dyn.dyn_body.composite_body.state.rot.ang_vel_this" )
dr_group.add_variable( "sv_dyn.dyn_body.derivs.Qdot_parent_this.scalar")
log_3vec( dr_group, "sv_dyn.dyn_body.derivs.Qdot_parent_this.vector" )
log_3vec( dr_group, "sv_dyn.dyn_body.derivs.rot_accel" )

dr_group.add_variable( "sv_dyn.dyn_body.grav_interaction.grav_pot")
log_3vec( dr_group, "sv_dyn.dyn_body.grav_interaction.grav_accel" )
log_3vec( dr_group, "sv_dyn.pfix.state.cart_coords" )
log_3vec( dr_group, "sv_dyn.grav_torque.torque" )


dr_group.add_variable( "sv_dyn.orb_elem.elements.semi_major_axis")
dr_group.add_variable( "sv_dyn.orb_elem.elements.semiparam")
dr_group.add_variable( "sv_dyn.orb_elem.elements.e_mag")
dr_group.add_variable( "sv_dyn.orb_elem.elements.inclination")
dr_group.add_variable( "sv_dyn.orb_elem.elements.arg_periapsis")
dr_group.add_variable( "sv_dyn.orb_elem.elements.long_asc_node")
dr_group.add_variable( "sv_dyn.orb_elem.elements.r_mag")
dr_group.add_variable( "sv_dyn.orb_elem.elements.vel_mag")
dr_group.add_variable( "sv_dyn.orb_elem.elements.true_anom")
dr_group.add_variable( "sv_dyn.orb_elem.elements.mean_motion")
dr_group.add_variable( "sv_dyn.orb_elem.elements.orbital_anom")
dr_group.add_variable( "sv_dyn.orb_elem.elements.orb_energy")
dr_group.add_variable( "sv_dyn.orb_elem.elements.orb_ang_momentum")

trick.add_data_record_group(dr_group, trick.DR_Buffer)
