################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_UDE_initialized/
####################################################################################


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_rec.py", "rb").read(), "Log_data/log_rec.py", 'exec'))
log_rec( LOG_CYCLE )

#############  MODIFIED DATA  #################

#############  ASSIGNMENTS, CALLS, and CONTROL  #################


jeod_time.manager_init.initializer = "met_veh1"

jeod_time.metveh1.epoch_defined_in_name = "UTC"
jeod_time.metveh1.epoch_format = trick.TimeEnum.calendar
jeod_time.metveh1.epoch_year = 1998
jeod_time.metveh1.epoch_month = 12
jeod_time.metveh1.epoch_day = 31
jeod_time.metveh1.epoch_hour = 23
jeod_time.metveh1.epoch_minute = 59
jeod_time.metveh1.epoch_second = 0.0

#///////////////////////////////////////////////////////////////////////
#// These two blocks are equivalent                                   //
#///////////////////////////////////////////////////////////////////////
jeod_time.metveh1.initial_value_format = trick.TimeEnum.clock
jeod_time.metveh1.clock_day = 0
jeod_time.metveh1.clock_hour = 0
jeod_time.metveh1.clock_minute = 0
jeod_time.metveh1.clock_second = 50.0
#///////////////////////////////////////////////////////////////////////
#//time.metveh1.initial_value_format = TimeEnum::seconds_since_epoch
#//time.metveh1.initializing_value = 50.0
#///////////////////////////////////////////////////////////////////////



#///////////////////////////////////////////////////////////////////////
#// These two blocks are equivalent:                                  //
#///////////////////////////////////////////////////////////////////////
jeod_time.metveh2.initial_value_format = trick.TimeEnum.seconds_since_epoch
jeod_time.metveh2.initializing_value = -5.0
#/////////////////////////////////////////////////////////////////////////
#//time.metveh2.epoch_defined_in_name = "met_veh1"
#//time.metveh2.epoch_format = TimeEnum::seconds_since_epoch
#//time.metveh2.epoch_initializing_value = 55.0
#/////////////////////////////////////////////////////////////////////////



jeod_time.gmst.initialize_from_name = "UT1"
jeod_time.gmst.update_from_name = "UT1"

jeod_time.gps.initialize_from_name = "TAI"
jeod_time.gps.update_from_name = "TAI"

jeod_time.metveh1.update_from_name = "TAI"

jeod_time.metveh2.update_from_name = "TAI"

jeod_time.tai.initialize_from_name = "UTC"
jeod_time.tai.update_from_name = "Dyn"

jeod_time.tdb.initialize_from_name = "TAI"
jeod_time.tdb.update_from_name = "TAI"

jeod_time.tt.initialize_from_name = "TAI"
jeod_time.tt.update_from_name = "TAI"

jeod_time.utc.initialize_from_name = "TAI"
jeod_time.utc.update_from_name = "TAI"

jeod_time.ut1.initialize_from_name = "TAI"
jeod_time.ut1.update_from_name = "TAI"


trick.add_read(10,  """
jeod_time.metveh2.hold = True
jeod_time.manager.simtime = 0

""")


trick.add_read(20,  """
jeod_time.metveh2.hold = False
jeod_time.manager.simtime = 0

""")


trick.sim_services.exec_set_terminate_time(25)
