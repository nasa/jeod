# Import the JEOD checkpoint/restart module.
import sys
import os
sys.path.append ('/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]))
import jeod_checkpoint_restart

trick.sim_services.exec_set_terminate_time(1)
jeod_checkpoint_restart.create_checkpoint (0.0, 1.0)

