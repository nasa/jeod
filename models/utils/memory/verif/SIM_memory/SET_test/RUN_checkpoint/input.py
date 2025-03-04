import sys
import os
JEOD_HOME = os.getenv("JEOD_HOME")
if JEOD_HOME is None:
    JEOD_HOME = os.path.abspath(os.path.join(os.getcwd(),"../../../../../"))
sys.path.append ('/'.join([JEOD_HOME, "lib/jeod/python"]))
import jeod_checkpoint_restart

trick.stop(2)

jeod_checkpoint_restart.create_checkpoint (1.0, 2.0)
