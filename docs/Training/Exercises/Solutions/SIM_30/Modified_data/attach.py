# attach vehicles
vehicle.mass_attach.set_subject_body(vehicle.array)
vehicle.mass_attach.set_parent_body(vehicle.dyn_body)
vehicle.mass_attach.offset_pstr_cstr_pstr = [0,0,0]
vehicle.mass_attach.pstr_cstr.data_source = trick.Orientation.InputEigenRotation
vehicle.mass_attach.pstr_cstr.eigen_angle = 0.0
vehicle.mass_attach.pstr_cstr.eigen_axis  = [0.0, 1.0, 0.0]
dynamics.dyn_manager.add_body_action( vehicle.mass_attach)
