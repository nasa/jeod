#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#  (Log data recording template for TGM verification)
#
#REFERENCES:
#  (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#  (None)
#
# 
#*/

def log_orb_elem_multi_rec ( log_cycle ) :
  recording_group_name =  "ORBEL_VER_BIN"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,6) :
    dr_group.add_variable("orb_elem_test.orb_elem_multi_ver.data_vals_inout[" + str(ii) + "]" )
  for ii in range(0,6) :
    dr_group.add_variable("orb_elem_test.orb_elem_multi_ver.data_vals_out[" + str(ii) + "]" )
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.semi_major_axis")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.semiparam")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.e_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.inclination")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.arg_periapsis")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.long_asc_node")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.r_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.vel_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.true_anom")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.mean_anom")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.mean_motion")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.orbital_anom")
  for ii in range(0,6) :
    dr_group.add_variable("orb_elem_test.orb_elem_multi_ver.data_vals_inout[" + str(ii) + "]" )
  for ii in range(0,6) :
    dr_group.add_variable("orb_elem_test.orb_elem_multi_ver.data_vals_out[" + str(ii) + "]" )
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.semi_major_axis")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.semiparam")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.e_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.inclination")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.arg_periapsis")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.long_asc_node")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.r_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.vel_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.true_anom")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.mean_anom")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.mean_motion")
  dr_group.add_variable(   "orb_elem_test.orb_elem_multi.orbital_anom")

  trick.add_data_record_group(dr_group)
  return
