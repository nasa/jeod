#*****************************************************************************
#           Run restart:  Test the checkpoint and restart mechanism
#*****************************************************************************/
# This file initializes the RUN to Scenario 6C. But, at time 0 restores the sim to
# the full model scenario captured in the first checkpoint file in RUN_checkpoint_create.
# The log output will be identical to RUN_checkpoint_create starting at restart time.
#
#Description:
#The purpose of this test is to verify the restart mechanism
#
#Run Scenario:
#   Initializes to Scenario 6C. Restarts to Full Model in RUN_checkpoint_create
#
#   Simulation Duration - 12000 sec (8 hrs)
#
#   Data Collection Rate - 10 sec., 600 sec. starting at restart time
#*****************************************************************************/

# Import the JEOD checkpoint/restart module.
import sys
import os
JEOD_HOME = os.getenv("JEOD_HOME")
if JEOD_HOME is None:
    JEOD_HOME = os.path.abspath(os.path.join(os.getcwd(),"../../"))
sys.path.append ('/'.join([JEOD_HOME, "lib/jeod/python"]))
import jeod_checkpoint_restart

#Set up scenario 6C
exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

# Set logging
LOG_CYCLE = 60.0
exec(compile(open( "Log_data/log_suite.py", "rb").read(), "Log_data/log_suite.py", 'exec'))

# Set initial attitude off LVLH
vehicle.lvlh_init.orientation.euler_angles = [0.0, 0.0, 0.0]

# Reconfigure mass to be cylinder
set_mass_cylinder()

# Begin maneuver.
trick.add_read(1000.0, "vehicle.force_extern.force  = [ 0.0, -29000.0, 0.0]")
# End maneuver
trick.add_read(1048.0, "vehicle.force_extern.force  = [ 0.0, 0.0, 0.0]")

exec(compile(open( "SET_test/chkpt_restart_times.py", "rb").read(), "SET_test/chkpt_restart_times.py", 'exec'))
filename = "chkpnt_"+str(chkpt_restart_time)+"00000"

jeod_checkpoint_restart.restore_from_checkpoint('SET_test/RUN_checkpoint_create', filename)


