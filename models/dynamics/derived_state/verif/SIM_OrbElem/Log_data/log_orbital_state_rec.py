def log_3_vec( drg, var):
  for ii in range(3):
    drg.add_variable(var+"["+str(ii)+"]")

dr_group = trick.sim_services.DRAscii("orb_elem")
dr_group.set_cycle(12)
dr_group.freq = trick.sim_services.DR_Always
dr_group.add_variable( "veh.orb_elem.elements.semi_major_axis")
dr_group.add_variable( "veh.orb_elem.elements.semiparam")
dr_group.add_variable( "veh.orb_elem.elements.e_mag")
dr_group.add_variable( "veh.orb_elem.elements.inclination")
dr_group.add_variable( "veh.orb_elem.elements.arg_periapsis")
dr_group.add_variable( "veh.orb_elem.elements.long_asc_node")
dr_group.add_variable( "veh.orb_elem.elements.r_mag")
dr_group.add_variable( "veh.orb_elem.elements.vel_mag")
dr_group.add_variable( "veh.orb_elem.elements.true_anom")
dr_group.add_variable( "veh.orb_elem.elements.mean_anom")
dr_group.add_variable( "veh.orb_elem.elements.mean_motion")
dr_group.add_variable( "veh.orb_elem.elements.orbital_anom")
dr_group.add_variable( "veh.orb_elem.elements.orb_energy")
dr_group.add_variable( "veh.orb_elem.elements.orb_ang_momentum")
log_3_vec( dr_group,   "veh.dyn_body.structure.state.trans.position")

trick.add_data_record_group(dr_group)
