flat_plate = trick.FlatPlateThermal()
flat_plate.thisown = 0
flat_plate.position =  [1.0 , 0.0, 0.0 ]
flat_plate.area  =  1.5
flat_plate.normal = [1.0 , 0.0 , 0.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "white"
flat_plate.thermal.active = True
interactions.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlateThermal()
flat_plate.thisown = 0
flat_plate.position =  [0.0 , 0.75, 0.0 ]
flat_plate.area  =  2.0
flat_plate.normal = [0.0 , 1.0 , 0.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "white"
flat_plate.thermal.active = True
interactions.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlateThermal()
flat_plate.thisown = 0
flat_plate.position =  [0.0 , 0.0, 0.5 ]
flat_plate.area  =  3.0
flat_plate.normal = [0.0 , 0.0 , 1.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "white"
flat_plate.thermal.active = True
interactions.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlateThermal()
flat_plate.thisown = 0
flat_plate.position =  [-1.0 , 0.0, 0.0 ]
flat_plate.area  =  1.5
flat_plate.normal = [-1.0 , 0.0 , 0.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "black"
flat_plate.thermal.active = True
interactions.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlateThermal()
flat_plate.thisown = 0
flat_plate.position =  [0.0 , -0.75, 0.0 ]
flat_plate.area  =  2.0
flat_plate.normal = [0.0 , -1.0 , 0.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "black"
flat_plate.thermal.active = True
interactions.surface.add_facet(flat_plate)

flat_plate = trick.FlatPlateThermal()
flat_plate.thisown = 0
flat_plate.position =  [0.0 , 0.0, -0.5 ]
flat_plate.area  =  3.0
flat_plate.normal = [0.0 , 0.0 ,-1.0  ]
flat_plate.temperature = 200
flat_plate.param_name = "black"
flat_plate.thermal.active = True
interactions.surface.add_facet(flat_plate)

#######################################################################
params = trick.FlatPlateAeroParams()
params.thisown = 0
params.name = "black"
params.coef_method = trick.AeroDragEnum.Diffuse
params.calculate_drag_coef = True
interactions.aero_surface_factory.add_facet_params(params)

params = trick.FlatPlateAeroParams()
params.thisown = 0
params.name = "white"
params.coef_method = trick.AeroDragEnum.Mixed
params.epsilon = 0.8
params.calculate_drag_coef = True
interactions.aero_surface_factory.add_facet_params(params)


#######################################################################
params = trick.RadiationParams()
params.thisown = 0
params.name = "black"
params.albedo = 0.2
params.diffuse = 0.95
params.thermal.emissivity = 0.8
params.thermal.heat_capacity_per_area = 200.0
interactions.rad_surface_factory.add_facet_params(params)

params = trick.RadiationParams()
params.thisown = 0
params.name = "white"
params.albedo = 0.7
params.diffuse = 0.7
params.thermal.emissivity = 0.3
params.thermal.heat_capacity_per_area =  500.0
interactions.rad_surface_factory.add_facet_params(params)
