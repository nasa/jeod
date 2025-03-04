################TRICK HEADER#######################################
#PURPOSE:
#  (To define the elements to be logged in the logging of log_contact_data)
####################################################################################

def log_contact_data ( log_cycle ) :
   # Import the JEOD logger
   import sys
   import os
   JEOD_HOME = os.getenv("JEOD_HOME")
   if JEOD_HOME is None:
      JEOD_HOME = os.path.abspath(os.path.join(os.getcwd(),"../../../../../"))
   sys.path.append ('/'.join([JEOD_HOME, "lib/jeod/python"]))
   import jeod_log

   # Create the logger.
   logger = jeod_log.Logger ()

   logger.open_group (1, "contact_data")

   logger.log_scalar(
      ("veh1_dyn.body.mass.composite_properties.mass",
       "veh2_dyn.body.mass.composite_properties.mass"))

   logger.log_vector(
      ("veh1_dyn.body.composite_body.state.trans.position",
       "veh1_dyn.body.composite_body.state.trans.velocity",
       "veh1_dyn.contact_surface.contact_force",
       "veh1_dyn.contact_surface.contact_torque",
       "veh2_dyn.body.composite_body.state.trans.position",
       "veh2_dyn.body.composite_body.state.trans.velocity",
       "veh2_dyn.contact_surface.contact_force",
       "veh2_dyn.contact_surface.contact_torque"))

   logger.close_group()

