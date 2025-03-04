def log_arg_peri (log_cycle) :

   # Import the JEOD logger
   import sys
   import os
   JEOD_HOME = os.getenv("JEOD_HOME")
   if JEOD_HOME is None:
      JEOD_HOME = os.path.abspath(os.path.join(os.getcwd(),"../../../../../"))
   sys.path.append ('/'.join([JEOD_HOME, "lib/jeod/python"]))
   import jeod_log

   # Create and initialize the logger.
   logger = jeod_log.Logger ()
   logger.open_group (log_cycle, "peri")

   # Log state
   logger.log_scalar ([
      "jeod_time.time_tt.seconds",
      "mercury.orb_elem.e_mag",
      "mercury.orb_elem.arg_periapsis"])

   # Close the group.
   logger.close_group()
