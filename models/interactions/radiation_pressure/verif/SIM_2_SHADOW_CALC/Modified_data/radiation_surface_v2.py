################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_radiation_surface_v2
####################################################################################

#NUM_PLATE = 10


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 2.0 , 0.0, 0.0 ]
fpt.normal  = [ -0.365 , -0.854 ,-0.370 ]
fpt.area  = 30.0
fpt.temperature  = 270.0
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, 0.0 ]
fpt.normal  = [ -0.359 , -0.910 , -0.205  ]
fpt.area  = 30.0
fpt.temperature  = 270
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, 0.0 ]
fpt.normal  = [ -0.343 , -0.939 ,-0.034  ]
fpt.area  = 30.0
fpt.temperature  = 270
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, 0.0 ]
fpt.normal  = [  -0.316 , -0.939 , 0.138  ]
fpt.area  = 30.0
fpt.temperature  = 270
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, 0.0 ]
fpt.normal  = [ -0.280 , -0.910 ,0.306 ]
fpt.area  = 30.0
fpt.temperature  = 270
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, 0.0 ]
fpt.normal  = [  -0.235 , -0.854 , 0.465  ]
fpt.area  = 30.0
fpt.temperature  = 270
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, 0.0 ]
fpt.normal  = [  -0.183 , -0.772 ,0.610 ]
fpt.area  = 30.0
fpt.temperature  = 270
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0

radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, 0.0 ]
fpt.normal  = [ -0.125 , -0.666 , 0.736   ]
fpt.area  = 30.0
fpt.temperature  = 270
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0

radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, 0.0 ]
fpt.normal  = [  -0.063 , -0.540 ,0.839   ]
fpt.area  = 30.0
fpt.temperature  = 270
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0

radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, 0.0 ]
fpt.normal  = [  -0.000 , -0.398 , 0.918   ]
fpt.area  = 30.0
fpt.temperature  = 270
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0

radiation.surface.add_facet( fpt )


#/******************************************************************************/
#/******************************************************************************/

params = trick.RadiationParams()
params.thisown = 0
params.set_name("radiation_test_material")
params.albedo = 0.5
params.diffuse = 0.5
params.thermal.emissivity = 0.5
params.thermal.heat_capacity_per_area = 100.0

radiation.surface_factory.add_facet_params(params)
