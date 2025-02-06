global DR_GROUP_ID
global drg
try:
    if DR_GROUP_ID >= 0:
        DR_GROUP_ID += 1
except NameError:
    DR_GROUP_ID = 0
    drg = []

drg.append( trick.sim_services.DRBinary("Earth_state") )
drg[DR_GROUP_ID].set_freq(trick.sim_services.DR_Always)
drg[DR_GROUP_ID].set_single_prec_only(False)
drg[DR_GROUP_ID].set_cycle(100)
drg[DR_GROUP_ID].enable()

for ii in range (0,3) :
  drg[DR_GROUP_ID].add_variable("earth_in_ssb.state.trans.position[" + str(ii) + "]")

for ii in range (0,3) :
  drg[DR_GROUP_ID].add_variable("earth_in_ssb.state.trans.velocity[" + str(ii) + "]")

for ii in range (0, 3) :
  for jj in range (0, 3) :
    drg[DR_GROUP_ID].add_variable ("earth.planet.pfix.state.rot.T_parent_this[" + str (ii) + "][" + str(jj) + "]")

for ii in range (0, 3) :
  drg[DR_GROUP_ID].add_variable ("earth.planet.pfix.state.rot.ang_vel_this[" + str(ii) + "]")

drg[DR_GROUP_ID].add_variable ("earth.planet.pfix.state.rot.Q_parent_this.scalar")
for ii in range (0, 3) :
  drg[DR_GROUP_ID].add_variable ("earth.planet.pfix.state.rot.Q_parent_this.vector[" + str (ii) + "]")

# Add group and increment counter
trick.add_data_record_group( drg[DR_GROUP_ID], trick.DR_Buffer )

