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

def log_orb_elem_rec ( log_cycle ) :
  recording_group_name =  "ORBEL_VER_BIN"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable("orb_elem_test.orb_elem_ver.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("orb_elem_test.orb_elem_ver.velocity[" + str(ii) + "]" )
  dr_group.add_variable(   "orb_elem_test.orb_elem.semi_major_axis")
  dr_group.add_variable(   "orb_elem_test.orb_elem.semiparam")
  dr_group.add_variable(   "orb_elem_test.orb_elem.e_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem.inclination")
  dr_group.add_variable(   "orb_elem_test.orb_elem.arg_periapsis")
  dr_group.add_variable(   "orb_elem_test.orb_elem.long_asc_node")
  dr_group.add_variable(   "orb_elem_test.orb_elem.r_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem.vel_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem.true_anom")
  dr_group.add_variable(   "orb_elem_test.orb_elem.mean_anom")
  dr_group.add_variable(   "orb_elem_test.orb_elem.mean_motion")
  dr_group.add_variable(   "orb_elem_test.orb_elem.orbital_anom")
  for ii in range(0,3) :
    dr_group.add_variable("orb_elem_test.orb_elem_ver.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("orb_elem_test.orb_elem_ver.velocity[" + str(ii) + "]" )
  dr_group.add_variable(   "orb_elem_test.orb_elem.semi_major_axis")
  dr_group.add_variable(   "orb_elem_test.orb_elem.semiparam")
  dr_group.add_variable(   "orb_elem_test.orb_elem.e_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem.inclination")
  dr_group.add_variable(   "orb_elem_test.orb_elem.arg_periapsis")
  dr_group.add_variable(   "orb_elem_test.orb_elem.long_asc_node")
  dr_group.add_variable(   "orb_elem_test.orb_elem.r_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem.vel_mag")
  dr_group.add_variable(   "orb_elem_test.orb_elem.true_anom")
  dr_group.add_variable(   "orb_elem_test.orb_elem.mean_anom")
  dr_group.add_variable(   "orb_elem_test.orb_elem.mean_motion")
  dr_group.add_variable(   "orb_elem_test.orb_elem.orbital_anom")

  trick.add_data_record_group(dr_group)
  return
