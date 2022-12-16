#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (This data file sets up the ground facets.)
#
#REFERENCE:
#   ((JSC Engineering Orbital Dynamics Models))
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((?))
#
#
#*/

def set_ground_facet(contact_ground_reference) :

  gp = trick.GroundFacet()
  gp.thisown = 0

  contact_params_local = trick.ContactParams()
  contact_params_local.thisown = 0

  contact_params_local.set_name("dirt")

  gp.surface_type = contact_params_local

  contact_ground_reference.contact.register_ground_facet(gp)

  return
