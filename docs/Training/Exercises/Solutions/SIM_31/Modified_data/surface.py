vehicle.vehicle_plate.local_position  = [ 1.0 , 0.0, 0.0 ]
vehicle.vehicle_plate.area  = 1.0
vehicle.vehicle_plate.local_normal  = [ 1.0 , 0.0 , 0.0 ]
vehicle.vehicle_plate.mass_body_name = "vehicle"
vehicle.vehicle_plate.param_name = "anything"

vehicle.array_plate.local_position  = [ 1.0 , 0.0, 0.0 ]
vehicle.array_plate.area  = 1.0
vehicle.array_plate.local_normal  = [ 1.0 , 0.0 , 0.0 ]
vehicle.array_plate.mass_body_name = "array"
vehicle.array_plate.param_name = "anything"

vehicle.surface.add_facet(vehicle.vehicle_plate)
vehicle.surface.add_facet(vehicle.array_plate)

################################################################

vehicle.surface.struct_body_name = "vehicle"
vehicle.surface.articulation_active = True


################################################################
params = trick.RadiationParams()
params.thisown = 0
params.name = "anything"
params.albedo = 0.2
params.diffuse = 0.95
params.thermal.emissivity = 0.8
params.thermal.heat_capacity_per_area = 200.0
interactions.rad_surface_factory.add_facet_params(params)
