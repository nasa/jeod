vehicle.vehicle_plate.local_position  = [ 1.0 , 2.0, 0.0 ]
vehicle.vehicle_plate.area  = 1.0
vehicle.vehicle_plate.local_normal  = [ 1.0 , 0.0 , 0.0 ]
vehicle.vehicle_plate.mass_body_name = "vehicle"

vehicle.array_plate.local_position  = [ 1.0 , 2.0, 0.0 ]
vehicle.array_plate.area  = 1.0
vehicle.array_plate.local_normal  = [ 1.0 , 0.0 , 0.0 ]
vehicle.array_plate.mass_body_name = "array"

vehicle.surface.add_facet(vehicle.vehicle_plate)
vehicle.surface.add_facet(vehicle.array_plate)

################################################################

vehicle.surface.struct_body_name = "vehicle"
vehicle.surface.articulation_active = True
