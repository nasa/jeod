################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_initplanet_rpol/
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#   Purpose:   Input file used for JEOD Planet model unit test
#
#   Test:      Verify initialization of Planet model works correctly for case
#              where polar radius is supplied parameter
#
#   
#*******************************************************************************/


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 0.10

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_planet_verif.py", "rb").read(), "Log_data/log_planet_verif.py", 'exec'))
log_planet_verif(LOG_CYCLE  )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################

#define LOG_CYCLE 0.10

## Default data file generated in S_define
#include "S_default.dat"

## Set up parameters to record
#include "Log_data/log_planet_verif.d"


## Configure the dynamics manager to operate in single planet mode
mngr.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
mngr.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
mngr.dyn_manager_init.central_point_name = "Earth"


## Test initialization from polar radius
earth.planet.flat_inv = 0.0  # Zero out default value
earth.planet.r_pol  = trick.attach_units( "m",6356751.60) # From Vallado 2nd ed., p. 908

## Set up simulation termination time

trick.sim_services.exec_set_terminate_time(5)
