# attach vehicles
mass_attach = trick.BodyAttachAligned()
mass_attach.thisown = 0
mass_attach.subject = vehicleB.body
mass_attach.parent  = vehicleA.body
mass_attach.subject_point_name = "Vehicle-Vehicle Attach Point"
mass_attach.parent_point_name  = "Vehicle-Vehicle Attach Point"

dynamics.dyn_manager.add_body_action( mass_attach)
