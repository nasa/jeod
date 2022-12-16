################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_radiation_surface_a
####################################################################################

#NUM_PLATE = 6

fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 2.0 , 0.0, 0.0 ]
fpt.normal  = [ 1.0 , 0.0 , 0.0 ]
fpt.area  = 60.0
fpt.temperature  = 500.0
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , -2.0, 0.0 ]
fpt.normal  = [ 0.0 , -1.0 , 0.0 ]
fpt.area  = 60.0
fpt.temperature  = 100
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ -2.0 , 0.0, 0.0 ]
fpt.normal  = [ -1.0 , 0.0 , 0.0 ]
fpt.area  = 60.0
fpt.temperature  = 1000
fpt.param_name = "radiation_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 2.0, 0.0 ]
fpt.normal  = [ 0.0 , 1.0 , 0.0 ]
fpt.area  = 60.0
fpt.temperature  = 500
fpt.param_name = "no_emission_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, 7.5 ]
fpt.normal  = [ 0.0 , 0.0 , 1.0 ]
fpt.area  = 16.0
fpt.temperature  = 500
fpt.param_name = "high_C_test_material"
fpt.thermal.active = True
fpt.thermal.thermal_power_dump = 0.0


radiation.surface.add_facet( fpt )


fpt = trick.FlatPlateThermal()
fpt.thisown = 0
fpt.position  = [ 0.0 , 0.0, -7.5 ]
fpt.normal  = [ 0.0 , 0.0 , -1.0 ]
fpt.area  = 16.0
fpt.temperature  = 500
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
params.thermal.heat_capacity_per_area = 50.0

radiation.surface_factory.add_facet_params(params)

params = trick.RadiationParams()
params.thisown = 0
params.set_name("no_emission_test_material")
params.albedo = 0.5
params.diffuse = 0.5
params.thermal.emissivity = 0.0
params.thermal.heat_capacity_per_area = 50.0

radiation.surface_factory.add_facet_params(params)

params = trick.RadiationParams()
params.thisown = 0
params.set_name("high_C_test_material")
params.albedo = 0.5
params.diffuse = 0.5
params.thermal.emissivity = 0.5
params.thermal.heat_capacity_per_area = 187.5

radiation.surface_factory.add_facet_params(params)
