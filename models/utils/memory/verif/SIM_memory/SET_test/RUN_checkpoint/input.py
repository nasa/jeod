import sys
import os
sys.path.append ('/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]))
import jeod_checkpoint_restart

trick.stop(2)

jeod_checkpoint_restart.create_checkpoint (1.0, 2.0)
