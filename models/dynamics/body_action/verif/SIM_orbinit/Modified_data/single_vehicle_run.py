################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_single_vehicle_run
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
# *
# * Purpose:
# *   Single vehicle run.
# *   Expected Trick variable settings:
# *   - TARGET_NAME  Either ISS or STS_114
# *   - TARGET_TRANS The translation state spec
# *   - TARGET_ROT   The rotational state spec OR
# *   - TARGET_ATT   The attitude spec
# *   - TARGET_RATE  The attitude rate spec
# *
# *
# *
# * Date:
# *   November, 2009
# *
# ******************************************************************************/

exec(compile(open("Log_data/log_data.py", "rb").read(), "Log_data/log_data.py", 'exec'))
exec(compile(open("Modified_data/run_files.py", "rb").read(), "Modified_data/run_files.py", 'exec'))

def set_single_vehicle_run(jeod_sys, earth, target, options) :

  # Load executive options
  set_system()

  # Initialize the Earth object.
  set_earth(jeod_time, earth, dynamics)

  # set up integration things
  dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4



  options["LOG_TARGET_TRANS"] = 1
  options["LOG_TARGET_ATT"] = 1
  options["LOG_TARGET_RATE"] = 1

  log_data(options)


  jeod_sys.interface_init.message_suppression_level = trick.MessageHandler.Debug


  #// Initialize the launch pad.
  earth.pad_39a.loc.altitude  = 3.0
  earth.pad_39a.loc.latitude  = trick.attach_units( "degree",28.6082)
  earth.pad_39a.loc.longitude  = trick.attach_units( "degree",-80.6040)
  earth.pad_39a.site_id = "pad_39a"

  options["VEHICLE"] = "target"
  options["VEHICLE_NAME"] = options["TARGET_NAME"]

  set_vehicle_setup(target, options)

  eval("set_" + options["TARGET_NAME"] + "_mass(target, dynamics)")

  if "TARGET_FULL" in options :
     eval("set_" + options["TARGET_NAME"] + "_" + options["TARGET_FULL"] + "(target, dynamics)")
  else :
     eval("set_" + options["TARGET_NAME"] + "_" + options["TARGET_TRANS"] + "(target, dynamics)")

     if "TARGET_ROT" in options :
        eval("set_" + options["TARGET_NAME"] + "_" + options["TARGET_ROT"] + "(target, dynamics)")
     elif ("TARGET_ATT" in options) == True :
        eval("set_" + options["TARGET_NAME"] + "_" + options["TARGET_ATT"] + "(target, dynamics)")
        eval("set_" + options["TARGET_NAME"] + "_" + options["TARGET_RATE"] + "(target, dynamics)")
     else :
        target.dyn_body.rotational_dynamics = False



  if "TARGET_REF" in options :
     eval("set_" + options["TARGET_NAME"] + "_reference_" + options["TARGET_REF"] + "_trans_state(target)")



  target.lvlh.reference_name = "Earth"
  target.ned.reference_name  = "Earth"

  if "TARGET_ELLIPTICAL_NED" in options :
     target.ned.ned_state.altlatlong_type = \
        trick.NorthEastDown.elliptical
  else :
     target.ned.ned_state.altlatlong_type = \
        trick.NorthEastDown.spherical

  set_target_rel_state (options["TARGET_NAME"])


  #// Disable jobs and objects
  # trick_sys.sched.set_job_onoff("target.orb_elem.from_cartesian", 0)
  trick_sys.sched.set_sim_object_onoff("chaser", 0)
  trick_sys.sched.set_sim_object_onoff("chaser_rel_state", 0)


  #// This simulation is an initialization sim only.
  trick.exec_set_terminate_time(0)
