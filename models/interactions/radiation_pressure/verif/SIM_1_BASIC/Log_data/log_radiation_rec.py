#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for radiation testing.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
#
#*/

def log_radiation_rec ( log_cycle ) :
  recording_group_name =  "radiation_profile"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable("radiation.rad_pressure.force[" + str(ii) + "]" )
    dr_group.add_variable("radiation.rad_pressure.torque[" + str(ii) + "]" )
    dr_group.add_variable("radiation.rad_surface.force[" + str(ii) + "]" )
    dr_group.add_variable("radiation_simple.rad_pressure.force[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.structure.state.trans.position[" + str(ii) + "]" )
  dr_group.add_variable(   "radiation.rad_pressure.source.flux_mag")
  dr_group.add_variable(   "radiation_simple.rad_pressure.source.flux_mag")
  dr_group.add_variable(   "radiation_simple.rad_surface.temperature")
  dr_group.add_variable(   "radiation_simple.rad_surface.thermal.power_absorb")
  dr_group.add_variable(   "radiation_simple.rad_surface.thermal.power_emit")

  trick.add_data_record_group(dr_group)
  return
