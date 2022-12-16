def configure_mass_init( mass, mass_init, name):
  mass.set_name( name )
  mass_init.subject = mass
  mass_init.action_name = name + "_init"
  mass_init.properties.mass = 1.0

  mass_init.properties.inertia_spec = trick.MassPropertiesInit.Body
  mass_init.properties.inertia = [[1.0, 0.0, 0.0],
                                  [0.0, 1.0, 0.0],
                                  [0.0, 0.0, 1.0]]


configure_mass_init( articulation.mass_bodya,
                     articulation.mass_bodya_init,
                     "mass_bodya")
configure_mass_init( articulation.mass_bodyb,
                     articulation.mass_bodyb_init,
                     "mass_bodyb")
configure_mass_init( articulation.mass_bodyc,
                     articulation.mass_bodyc_init,
                     "mass_bodyc")
configure_mass_init( articulation.mass_bodyd,
                     articulation.mass_bodyd_init,
                     "mass_bodyd")



# Add the FlatPlates to the surface model
articulation.surface.add_facet(articulation.flat_plate_1)
articulation.surface.add_facet(articulation.flat_plate_2)
articulation.surface.add_facet(articulation.flat_plate_3)
articulation.surface.add_facet(articulation.flat_plate_4)
articulation.surface.add_facet(articulation.flat_plate_5)
articulation.surface.add_facet(articulation.flat_plate_6)
articulation.surface.add_facet(articulation.flat_plate_7)
articulation.surface.add_facet(articulation.flat_plate_8)

trick.stop(10)
