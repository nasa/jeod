################TRICK HEADER#######################################
#PURPOSE:
#  (Replaces the default data used in the Trick 7 version)
####################################################################################

#Auto-generated from the data file with this header:
#/*
#   file purpose: Default input data for all atv_rvs.c unit test verification
#   test:         all tests
#   tester:       Scott Piggott
#*/

def set_default_data(orb_elem_test) :

  orb_elem_test.orb_elem.semiparam              = 0.0
  orb_elem_test.orb_elem.e_mag                  = 0.0
  orb_elem_test.orb_elem.inclination            = 0.0
  orb_elem_test.orb_elem.mean_anom              = 0.0
  orb_elem_test.orb_elem.arg_periapsis          = 0.0
  orb_elem_test.orb_elem.long_asc_node          = 0.0

  orb_elem_test.orb_elem_multi.semiparam        = 0.0
  orb_elem_test.orb_elem_multi.e_mag            = 0.0
  orb_elem_test.orb_elem_multi.inclination      = 0.0
  orb_elem_test.orb_elem_multi.mean_anom        = 0.0
  orb_elem_test.orb_elem_multi.arg_periapsis    = 0.0
  orb_elem_test.orb_elem_multi.long_asc_node    = 0.0

  orb_elem_test.orb_elem_ver.mu                 = 0.0
  orb_elem_test.orb_elem_ver.position         = [ 0.0, 0.0, 0.0]
  orb_elem_test.orb_elem_ver.velocity         = [ 0.0, 0.0, 0.0]
  orb_elem_test.orb_elem_ver.to_cartesian       = 0

  orb_elem_test.orb_elem_multi_ver.multi_use    = 0
  orb_elem_test.orb_elem_multi_ver.mu           = 0.0
  orb_elem_test.orb_elem_multi_ver.data_row     = 0
  orb_elem_test.orb_elem_multi_ver.data_vals_in[0] = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

  return
