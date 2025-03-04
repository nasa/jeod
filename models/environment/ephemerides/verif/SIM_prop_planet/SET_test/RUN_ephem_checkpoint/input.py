################################################################################
# PURPOSE:
#   (To define the input data for simulation run SET_test/RUN_ephem_checkpoint/)
################################################################################


# Import the JEOD checkpoint/restart module.
import sys
import os
JEOD_HOME = os.getenv("JEOD_HOME")
if JEOD_HOME is None:
    JEOD_HOME = os.path.abspath(os.path.join(os.getcwd(),"../../../../../"))
sys.path.append ('/'.join([JEOD_HOME, "lib/jeod/python"]))
import jeod_checkpoint_restart

# Set up the simulation.
# The default setup uses the ephemeris model to set translational state.
LOG_CYCLE = 60*86400
exec(compile(open("Modified_data/setup.py", "rb").read(), "Modified_data/setup.py", 'exec'))
set_setup()

# Run the sim for 120 days only.
trick.stop(120*86400)

trick.checkpoint_post_init(True)

# ... and at 60 days.
# Note that the end time in the checkpoint file is 150 years and 30 days.
jeod_checkpoint_restart.create_checkpoint (60*86400, 86400*(365*150+30))
