#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for orbital vehicle state data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
#
#*/


def log_log_states (planets) :

   # Import the JEOD logger
   import sys
   import os
   sys.path.append ('/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]))
   import jeod_log

   # Create and initialize the logger.
   logger = jeod_log.Logger ()
   logger.open_group (LOG_CYCLE, "state")

   # Log time
   logger.log_scalar (["jeod_time.time_manager.simtime",
                       "jeod_time.time_manager.dyn_time.seconds",
                       "jeod_time.time_tai.seconds",
                       "jeod_time.time_tt.seconds"])
   # Log the translational states for the Sun and Jupiter
   for planet in planets :
      logger.log_ref_frame_trans_state (
         [planet + ".prop_planet.body.composite_body",
          planet + ".planet.inertial"])
      logger.log_trans_state (
          planet + ".relative.rel_state.trans")
      logger.log_vector (
          planet + ".prop_planet.body.grav_interaction.grav_accel")

   # Close the logger.
   logger.close_group ()
