# This run is simply to confirm that the various implementations of the model
# return exactly the same results.
exec(compile(open("SET_test/RUN_T01_MET_VER/input.py", "rb").read(), "SET_test/RUN_T01_MET_VER/input.py", 'exec'))

log_add_atmos_state( "vehicle.atmos_state", dr_group)
log_add_atmos_state( "vehicle.atmos_state_deprecate", dr_group)
log_add_atmos_state( "vehicle.met_atmos_state", dr_group)
log_add_atmos_state( "vehicle.met_atmos_state_deprecate", dr_group)
log_add_met_atmos_state( "vehicle.met_atmos_state", dr_group)
log_add_met_atmos_state( "vehicle.met_atmos_state_deprecate", dr_group)
