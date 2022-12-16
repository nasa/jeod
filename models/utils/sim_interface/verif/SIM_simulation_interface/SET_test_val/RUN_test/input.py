################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_test/
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Test the JEOD simulation interface.)
#
# 
#
#*******************************************************************************/


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_private_fields_ascii_rec.py", "rb").read(), "Log_data/log_private_fields_ascii_rec.py", 'exec'))
log_private_fields_ascii_rec(0.7)

#############  ASSIGNMENTS, CALLS, and CONTROL  #################


sim_interface_test.jcd.set_total_job_cycle_time(13.2)

read = 2.5
trick.add_read(read, """
sim_interface_test.jcd.active = True
""")

read = 5.0
trick.add_read(read, """
sim_interface_test.jcd.set_total_job_cycle_time(0.0)
""")

trick.sim_services.exec_set_terminate_time(10)
