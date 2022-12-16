################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_UTC_initialized/ 
####################################################################################



trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_rec_tdb.py", "rb").read(), "Log_data/log_rec_tdb.py", 'exec'))
log_rec( LOG_CYCLE )

#############  MODIFIED DATA  #################

#############  ASSIGNMENTS, CALLS, and CONTROL  #################

#sys.exec.in.trap_sigfpe          = 1;
#sys.exec.in.pre_init_checkpoint  = 0;
#sys.exec.in.post_init_checkpoint = 0;
#sys.exec.in.end_checkpoint       = 0;
#sys.exec.in.reduced_checkpoint   = 0;
#sys.exec.sim_com.monitor_on      = 0;
#sys.exec.rt_log.group[0].record  = 0;

jeod_time.manager_init.initializer = "UTC"
jeod_time.manager_init.sim_start_format = trick.TimeEnum.calendar

jeod_time.utc.calendar_year = 2017
jeod_time.utc.calendar_month = 10
jeod_time.utc.calendar_day = 28
jeod_time.utc.calendar_hour = 01.0
jeod_time.utc.calendar_minute = 23.0
jeod_time.utc.calendar_second = 45.0

#/////////////////////////////////////////////////////////////////////
#// These three blocks should be equivalent                         //
#// (but only because 50 UTC seconds = 50 TAI seconds at this time)://
#/////////////////////////////////////////////////////////////////////
jeod_time.metveh1.epoch_defined_in_name = "UTC"                    
jeod_time.metveh1.epoch_format = trick.TimeEnum.calendar            
jeod_time.metveh1.epoch_year = 1998                                
jeod_time.metveh1.epoch_month = 12                                 
jeod_time.metveh1.epoch_day = 31                                   
jeod_time.metveh1.epoch_hour = 23                                  
jeod_time.metveh1.epoch_minute = 59                                
jeod_time.metveh1.epoch_second = 0.0                               
#///////////////////////////////////////////////////////////////////////
#//time.metveh1.initial_value_format = TimeEnum::calendar;
#//time.metveh1.clock_day = 0;                            
#//time.metveh1.clock_hour = 0;                           
#//time.metveh1.clock_minute = 0;                         
#//time.metveh1.clock_second = 50.0;                      
#///////////////////////////////////////////////////////////////////////
#//time.metveh1.initial_value_format = TimeEnum::seconds_since_epoch;
#//time.metveh1.initializing_value = 50.0;                           
#///////////////////////////////////////////////////////////////////////



#///////////////////////////////////////////////////////////////////////
#// These two blocks are equivalent:                                  //
#///////////////////////////////////////////////////////////////////////
jeod_time.metveh2.initial_value_format = trick.TimeEnum.seconds_since_epoch
jeod_time.metveh2.initializing_value = -5.0
#/////////////////////////////////////////////////////////////////////////
#//time.metveh2.epoch_defined_in_name = "met_veh1";          
#//time.metveh2.epoch_format = TimeEnum::seconds_since_epoch;
#//time.metveh2.epoch_initializing_value = 55.0;             
#/////////////////////////////////////////////////////////////////////////



jeod_time.gmst.initialize_from_name = "UT1"
jeod_time.gmst.update_from_name = "UT1"

jeod_time.gps.initialize_from_name = "TAI"
jeod_time.gps.update_from_name = "TAI"

jeod_time.metveh1.update_from_name = "TAI"

jeod_time.metveh2.update_from_name = "TAI"

jeod_time.tai.initialize_from_name = "UTC"
jeod_time.tai.update_from_name = "TDB"

jeod_time.tdb.initialize_from_name = "TAI"
jeod_time.tdb.update_from_name = "Dyn"

jeod_time.tt.initialize_from_name = "TAI"
jeod_time.tt.update_from_name = "TAI"

jeod_time.utc.update_from_name = "TAI"

jeod_time.ut1.initialize_from_name = "TAI"
jeod_time.ut1.update_from_name = "TAI"



#trick.add_read(10,  """
#jeod_time.metveh2.hold = True
#jeod_time.manager.simtime = 0

#""")


#trick.add_read(20,  """
#jeod_time.metveh2.hold = False
#jeod_time.manager.simtime = 0

#""")


trick.sim_services.exec_set_terminate_time(3600*2)
