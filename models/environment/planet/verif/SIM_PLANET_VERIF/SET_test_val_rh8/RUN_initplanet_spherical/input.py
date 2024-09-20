################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_initplanet_spherical/
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#   Purpose:   Input file used for JEOD Planet model unit test
#
#   Test:      Verify initialization of Planet model works correctly when planet
#              is desired to be perfectly spherical (no data supplied) and r_eq
#              is also initially set to zero (tests copy from GravBody)
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


## Test initialization with only equatorial radius supplied
## result should be spherical
earth.planet.flat_inv = 0.0  # Zero out default value
earth.planet.r_eq = 10000.0

## Set up simulation termination time

trick.sim_services.exec_set_terminate_time(5)
