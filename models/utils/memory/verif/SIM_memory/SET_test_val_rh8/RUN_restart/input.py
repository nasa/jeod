import sys
import os
sys.path.append ('/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]))
import jeod_checkpoint_restart

jeod_checkpoint_restart.restore_from_checkpoint (
  "SET_test/RUN_checkpoint", "chkpnt_1.000000")

# Checkpoint file terminates at t=2.0.  If sim still going at t=3.0, terminate
# to prevent indefinite run.
trick.add_read(2, "trick.exec_terminate_with_return(-1, 'input.py', 13, 'Failed to load checkpoint')")
trick.stop(3)
