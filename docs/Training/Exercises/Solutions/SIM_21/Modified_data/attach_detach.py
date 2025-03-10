mass_attach = trick.BodyAttachAligned()
mass_attach.thisown = 0
mass_attach.set_subject_body( vehicle.dyn_body )
mass_attach.set_parent_body( vehicleB.dyn_body )
mass_attach.subject_point_name = "Vehicle A Mass Point 1"
mass_attach.parent_point_name  = "Vehicle B Mass Point 1"
mass_attach.active = False
dynamics.dyn_manager.add_body_action( mass_attach)

mass_detach = trick.BodyDetach()
mass_detach.thisown = 0
mass_detach.set_subject_body( vehicle.dyn_body )
mass_detach.active = False
dynamics.dyn_manager.add_body_action( mass_detach)

