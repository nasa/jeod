# This is an input file for the surface model verification, Trick 10 version
#
# Andrew Spencer
# November, 2010
#

# allocate the flat plate and populate it

flat_plate = trick.FlatPlate()
flat_plate.thisown = 0
flat_plate.position = [1255.0, 0.0, 383.4]
flat_plate.area = 119.4454385
flat_plate.normal = [1.0, 0.0, 0.0]
flat_plate.param_name = "flat_plate_material"

surface_model.surface.add_facet(flat_plate)

demo_facet = trick.DemoFacet()
demo_facet.thisown = 0
demo_facet.set_name("demo_basic_facet")
demo_facet.some_int = 1337 # random integer
demo_facet.param_name = "demo_basic_facet"

surface_model.surface.add_facet(demo_facet)

# facets have now been added to the surface.
# add the facet factories onto the surface factories

# Add the factory to create a FlatPlateDemo1 from a FlatPlate

fpf1 = trick.FlatPlateDemoFactory1()
fpf1.thisown = 0
surface_model.surf_fact1.add_facet_factory(fpf1)


# Add the factory to create a FlatPlateDemo2 from a FlatPlate

fpf2 = trick.FlatPlateDemoFactory2()
fpf2.thisown = 0
surface_model.surf_fact2.add_facet_factory(fpf2)

# Add the factory to create a DemoInteractionFacet1 from a DemoFacet

dff1 = trick.DemoFacetFactory1()
dff1.thisown = 0
surface_model.surf_fact1.add_facet_factory(dff1)

# Add the factory to create a DemoINteractionFacet2 from a DemoFacet

dff2 = trick.DemoFacetFactory2()
dff2.thisown = 0
surface_model.surf_fact2.add_facet_factory(dff2)

# The facet factories have now all been added.
# Now add the params to the facet factories

params1 = trick.DemoParams1()
params1.thisown = 0

params1.weight = 3.14
params1.set_name("demo_basic_facet")
surface_model.surf_fact1.add_facet_params(params1)

# Add demo params to surface factory 2

params2 = trick.DemoParams2()
params2.thisown = 0

params2.color = "burnt orange"
params2.set_name("demo_basic_facet")
surface_model.surf_fact2.add_facet_params(params2)

# add flat plate demo params to surface factory 1

fpparams1 = trick.FlatPlateDemoParams1()
fpparams1.thisown = 0

fpparams1.shape = "octagon"
fpparams1.set_name("flat_plate_material")
surface_model.surf_fact1.add_facet_params(fpparams1)

# add the flat plate demo params to surface actory 2

fpparams2 = trick.FlatPlateDemoParams2()
fpparams2.thisown = 0

fpparams2.sides = 6
fpparams2.set_name("flat_plate_material")
surface_model.surf_fact2.add_facet_params(fpparams2)

surface_model.inter1.interaction_number = 24601
surface_model.inter2.interaction_name = "demonstration interaction"


trick.exec_set_terminate_time(0.1)

