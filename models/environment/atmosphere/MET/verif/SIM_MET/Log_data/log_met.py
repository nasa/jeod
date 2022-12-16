def log_add_atmos_state( atmos_state,
                         dr_group):
  dr_group.add_variable( atmos_state+".density")
  dr_group.add_variable( atmos_state+".pressure")
  dr_group.add_variable( atmos_state+".temperature")

def log_add_met_atmos_state( atmos_state,
                             dr_group):
  dr_group.add_variable( atmos_state+".density")
  dr_group.add_variable( atmos_state+".pressure")
  dr_group.add_variable( atmos_state+".temperature")
  dr_group.add_variable( atmos_state+".exo_temp")
  dr_group.add_variable( atmos_state+".log10_dens")
  dr_group.add_variable( atmos_state+".mol_weight")
  dr_group.add_variable( atmos_state+".N2")
  dr_group.add_variable( atmos_state+".Ox2")
  dr_group.add_variable( atmos_state+".Ox")
  dr_group.add_variable( atmos_state+".A")
  dr_group.add_variable( atmos_state+".He")
  dr_group.add_variable( atmos_state+".Hyd")

def log_add_met( dr_group):
  log_add_met_atmos_state( "atmos_met.met_atmos_state", dr_group)

def log_atmos_verif() :
   # Build a data recording group name.
   dr_group = trick.sim_services.DRAscii("atmos_verif")
   dr_group.thisown = 0
   dr_group.set_cycle(1.0)
   dr_group.freq = trick.sim_services.DR_Always

   dr_group.add_variable("vehicle.pos.ellip_coords.altitude")
   dr_group.add_variable("vehicle.pos.ellip_coords.latitude")
   dr_group.add_variable("vehicle.pos.ellip_coords.longitude")
   log_add_atmos_state( "atmos_met.atmos_state", dr_group)

   # Add the data recording group to Trick's data recording.
   trick.add_data_record_group(dr_group)
   return dr_group

