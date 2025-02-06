#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for orbital vehicle atmospheric data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
#
#*/

def log_sv_atmos_rec ( log_cycle , VEH_NAME , VEH_OBJ ) :
  recording_group_name =  ""+VEH_NAME+"_Atmos"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   "jeod_time.utc.calendar_year")
  dr_group.add_variable(   "jeod_time.utc.calendar_month")
  dr_group.add_variable(   "jeod_time.utc.calendar_day")
  dr_group.add_variable(   "jeod_time.utc.calendar_hour")
  dr_group.add_variable(   "jeod_time.utc.calendar_minute")
  dr_group.add_variable(   "jeod_time.utc.calendar_second")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.ellip_coords.altitude")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.ellip_coords.latitude")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.ellip_coords.longitude")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.sphere_coords.altitude")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.sphere_coords.latitude")
  dr_group.add_variable(   ""+VEH_OBJ+".pfix.state.sphere_coords.longitude")
  for ii in range(0,3) :
    dr_group.add_variable(""+VEH_OBJ+".pfix.state.cart_coords[" + str(ii) + "]" )
  dr_group.add_variable(   ""+VEH_OBJ+".atmos.temperature")
  dr_group.add_variable(   "earth.atmos.state.exo_temp")
  dr_group.add_variable(   ""+VEH_OBJ+".atmos.density")
  dr_group.add_variable(   "earth.atmos.state.log10_dens")
  dr_group.add_variable(   ""+VEH_OBJ+".atmos.pressure")
  dr_group.add_variable(   "earth.atmos.state.mol_weight")

  trick.add_data_record_group(dr_group)
  return
