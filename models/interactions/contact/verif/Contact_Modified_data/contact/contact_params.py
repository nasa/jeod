#PURPOSE:
#   (This data file sets up the vehicle contact params.)
#
#REFERENCE:
#   ((JSC Engineering Orbital Dynamics Models))
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((?))
#
#


def set_contact_params(sv_dyn_reference) :

  contact_params_local = trick.ContactParams()
  contact_params_local.thisown = 0

  contact_params_local.set_name("steel")

  sv_dyn_reference.facet_params = contact_params_local
  sv_dyn_reference.contact_surface_factory.add_facet_params(contact_params_local)


  return
