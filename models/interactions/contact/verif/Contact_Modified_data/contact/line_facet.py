#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (This data file sets up the vehicle  surface facets.)
#
#REFERENCE:
#   ((JSC Engineering Orbital Dynamics Models))
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((?))
#
#
#*/

def set_contact_line_facet(sv_dyn_reference, SV_NAME) :

  exec('sv_dyn_reference.surface_model.struct_body_name = "' + SV_NAME + '"')

  fp = trick.Cylinder()
  fp.thisown = 0

  exec( 'fp.set_name("' + SV_NAME + '_facet")')
  fp.position  = [ 0.0 , 0.0, 0.0 ]
  fp.normal  = [ 1.0 , 0.0 , 0.0 ]
  fp.radius  = 1.0
  fp.length  = 2.0
  fp.param_name = "steel"
  exec('fp.mass_body_name = "' + SV_NAME + '"')

  sv_dyn_reference.facet_ptr = fp
  sv_dyn_reference.surface_model.add_facet(fp)
