fpc = trick.FlatPlateCircular()
fpc.thisown  = 0
fpc.position       = [ 0.0 , 0.0, 0.0 ]
fpc.normal         = [ 0.0 , -1.0 , 0.0 ]
fpc.radius         = 1.0
fpc.param_name     = "steel"
fpc.mass_body_name = "veh"
fpc.name           = "collision face"

interactions.veh_surface.add_facet(fpc)


fpc = trick.FlatPlateCircular()
fpc.thisown = 0
fpc.position       = [ 0.0 , 0.0, 0.0 ]
fpc.normal         = [ 0.0 , 1.0 , 0.0 ]
fpc.radius         = 1.0
fpc.param_name     = "steel"
fpc.mass_body_name = "vehB"
fpc.name           = "collision face"

interactions.vehB_surface.add_facet(fpc)

################################################################

interactions.veh_surface.struct_body_name = "veh"
interactions.vehB_surface.struct_body_name = "vehB"

################################################################

cp = trick.ContactParams()
cp.thisown = 0
cp.name = "steel"
interactions.contact_surface_factory.add_facet_params(cp)
