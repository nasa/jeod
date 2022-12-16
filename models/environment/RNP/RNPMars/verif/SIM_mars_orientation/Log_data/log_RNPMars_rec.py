#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log Mars RNP transformation)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
# 
#*/

def log_RNPMars (log_cycle) :
  recording_group_name =  "RNPMars"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always

  dr_group.add_variable ("jeod_time.tt.seconds")
  dr_group.add_variable("mars.planet.pfix.state.rot.Q_parent_this.scalar")
  for ii in range (0, 3) :
    dr_group.add_variable("mars.planet.pfix.state.rot.Q_parent_this.vector["+str(ii)+"]")
  dr_group.add_variable("mars.planet.pfix.state.rot.ang_vel_mag")
  for ii in range (0,3) :
    dr_group.add_variable("mars.planet.pfix.state.rot.ang_vel_this["+str(ii)+"]")

  for ii in range(0,3) :
    for jj in range (0, 3) :
      dr_group.add_variable("mars.planet.pfix.state.rot.T_parent_this["+str(ii)+"]["+str(jj)+"]")

  trick.add_data_record_group(dr_group)
  return
