################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_default_mods
####################################################################################

#Auto-generated from the data file with this header:
#/*
#   file purpose: Default input data for all atv_rvs.c unit test verification
#   test:         all tests
#   tester:       Scott Piggott
#*/

exec(compile(open("Modified_data/default_data.py", "rb").read(), "Modified_data/default_data.py", 'exec'))
exec(compile(open("Log_data/log_orb_elem_rec.py", "rb").read(), "Log_data/log_orb_elem_rec.py", 'exec'))

def set_default_mods(orb_elem_test) :


  set_default_data(orb_elem_test)

  # Trick 7 version was hard coded in the logfile as 0.1 seconds
  log_orb_elem_rec(0.1)


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

  orb_elem_test.orb_elem_ver.mu                 = 3.986012E14
  orb_elem_test.orb_elem_ver.position         = [ 0.0, 0.0, 0.0]
  orb_elem_test.orb_elem_ver.velocity         = [ 0.0, 0.0, 0.0]
  orb_elem_test.orb_elem_ver.to_cartesian       = False

  orb_elem_test.orb_elem_multi_ver.multi_use    = False
  orb_elem_test.orb_elem_multi_ver.mu           = 3.986012E14
  orb_elem_test.orb_elem_multi_ver.data_row     = 0



  return
