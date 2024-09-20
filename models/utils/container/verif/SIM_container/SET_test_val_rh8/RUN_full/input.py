# Import the JEOD checkpoint/restart module.
import sys
import os
sys.path.append ('/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]))
import jeod_checkpoint_restart

test_sim_object.container.test1 ()
trick.sim_services.exec_set_terminate_time(2)

jeod_sys.jeod_sim_interface.set_checkpoint_file_name ("")
jeod_checkpoint_restart.create_checkpoint (1.0, 2.0)
