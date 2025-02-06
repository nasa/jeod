# Import the JEOD checkpoint/restart module.
import sys 
import os
sys.path.append ('/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]))
import jeod_checkpoint_restart

# Restart the sim from the checkpoint data in SET_test/RUN_empty.
jeod_checkpoint_restart.restore_from_checkpoint (
   "SET_test/RUN_empty", "chkpnt_0.000000")

# Run will terminate at t=1 if the checkpoint file is found.
# If not, it will run forever.  Drop a warning and terminate.
trick.add_read(2,"print 'Checkpoint file not found, terminating.'")
trick.stop(2)
