# attach vehicles
mass_attach = trick.BodyAttachAligned()
mass_attach.thisown = 0
mass_attach.set_subject_body( vehicleB.body )
mass_attach.set_parent_body( vehicleA.body )
mass_attach.subject_point_name = "Vehicle-Vehicle Attach Point"
mass_attach.parent_point_name  = "Vehicle-Vehicle Attach Point"

dynamics.dyn_manager.add_body_action( mass_attach)
