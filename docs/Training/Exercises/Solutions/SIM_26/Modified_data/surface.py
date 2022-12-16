flat_plate = trick.FlatPlate()
flat_plate.thisown = 0
flat_plate.position = [1.0 , 0.0, 0.0 ]
flat_plate.area  = 1.5
flat_plate.normal = [1.0 , 0.0 , 0.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "white"
vehicle_surf.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlate()
flat_plate.thisown = 0
flat_plate.position = [0.0 , 0.75, 0.0 ]
flat_plate.area  = 2.0
flat_plate.normal = [0.0 , 1.0 , 0.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "white"
vehicle_surf.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlate()
flat_plate.thisown = 0
flat_plate.position = [0.0 , 0.0, 0.5 ]
flat_plate.area  = 3.0
flat_plate.normal = [0.0 , 0.0 , 1.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "white"
vehicle_surf.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlate()
flat_plate.thisown = 0
flat_plate.position = [-1.0 , 0.0, 0.0 ]
flat_plate.area  = 1.5
flat_plate.normal = [-1.0 , 0.0 , 0.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "black"
vehicle_surf.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlate()
flat_plate.thisown = 0
flat_plate.position = [0.0 , -0.75, 0.0 ]
flat_plate.area  = 2.0
flat_plate.normal = [0.0 , -1.0 , 0.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "black"
vehicle_surf.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlate()
flat_plate.thisown = 0
flat_plate.position = [0.0 , 0.0, -0.5 ]
flat_plate.area  = 3.0
flat_plate.normal = [0.0 , 0.0 ,-1.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "black"
vehicle_surf.surface.add_facet(flat_plate)
