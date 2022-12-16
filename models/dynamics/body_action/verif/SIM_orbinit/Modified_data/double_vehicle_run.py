################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_double_vehicle_run
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
# *
# * Purpose:
# *   Double vehicle run.
# *   Expected Trick variable settings:
# *   - TARGET_NAME  The name of the target vehicle (ISS)
# *   - CHASER_NAME  The name of the chaser vehicle (STS_114)
# *   - CHASER_TRANS The translation state spec
# *   - CHASER_ROT   The rotational state spec OR
# *   - CHASER_ATT   The attitude spec
# *   - CHASER_RATE  The attitude rate spec
# *
# *
# *
# * Date:
# *   November, 2009
# *
# ******************************************************************************/

exec(compile(open("Log_data/log_data.py", "rb").read(), "Log_data/log_data.py", 'exec'))
exec(compile(open("Modified_data/run_files.py", "rb").read(), "Modified_data/run_files.py", 'exec'))

def set_double_vehicle_run(jeod_sys, earth, target, options) :

  # Load executive options
  set_system()

  # set up integration things
  dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

  set_earth(jeod_time, earth, dynamics)

  if "TARGET_FULL" in options :
     options["LOG_TARGET_TRANS"] = 1
     options["LOG_TARGET_ATT"] = 1
     options["LOG_TARGET_RATE"] = 1

  elif "TARGET_TRANS" in options :
     options["LOG_TARGET_TRANS"] = 1
     options["LOG_TARGET_ATT"] = 1
     options["LOG_TARGET_RATE"] = 1

  options["LOG_CHASER_TRANS"] = 1
  options["LOG_CHASER_ATT"] = 1
  options["LOG_CHASER_RATE"] = 1

  log_data(options)
  jeod_sys.interface_init.message_suppression_level = 1000

  #// Initialize the launch pad.
  earth.pad_39a.loc.altitude  = 3.0
  earth.pad_39a.loc.latitude  = trick.attach_units( "degree",28.6082)
  earth.pad_39a.loc.longitude  = trick.attach_units( "degree",-80.6040)
  earth.pad_39a.site_id = "pad_39a"

  options["VEHICLE"] = "chaser"
  options["VEHICLE_NAME"] = options["CHASER_NAME"]

  set_vehicle_setup(chaser, options)

  eval("set_" + options["CHASER_NAME"] + "_mass(chaser, dynamics)")

# The following control logic doesn't completely follow the Trick 07 version's, but
# it should be completely equivalent

  if "CHASER_FULL" in options :
     eval("set_" + options["CHASER_NAME"] + "_" + options["CHASER_FULL"] + "(chaser, dynamics)")
  else :
     eval("set_" + options["CHASER_NAME"] + "_" + options["CHASER_TRANS"] + "(chaser, dynamics)")

     if "CHASER_ROT" in options :
        eval("set_" + options["CHASER_NAME"] + "_" + options["CHASER_ROT"] + "(chaser, dynamics)")
     elif "CHASER_ATT" in options :
        eval("set_" + options["CHASER_NAME"] + "_" + options["CHASER_ATT"] + "(chaser, dynamics)")
        eval("set_" + options["CHASER_NAME"] + "_" + options["CHASER_RATE"] + "(chaser, dynamics)")
     else :
        chaser.dyn_body.rotational_dynamics = False


  options["VEHICLE"] = "target"
  options["VEHICLE_NAME"] = options["TARGET_NAME"]

  set_vehicle_setup(target, options)

  eval("set_" + options["TARGET_NAME"] + "_mass(target, dynamics)")

  if "TARGET_FULL" in options :
     eval("set_" + options["TARGET_NAME"] + "_" + options["TARGET_FULL"] + "(target, dynamics)")
  elif "TARGET_TRANS" in options :
     eval("set_" + options["TARGET_NAME"] + "_" + options["TARGET_TRANS"] + "(target, dynamics)")

     if "TARGET_ROT" in options :
        eval("set_" + options["TARGET_NAME"] + "_" + options["TARGET_ROT"] + "(target, dynamics)")
     else :
        eval("set_" + options["TARGET_NAME"] + "_" + options["TARGET_ATT"] + "(target, dynamics)")
        eval("set_" + options["TARGET_NAME"] + "_" + options["TARGET_RATE"] + "(target, dynamics)")
  else :
     eval("set_" + options["TARGET_NAME"] + "_trans_TransState_inertial_body(target, dynamics)")
     eval("set_" + options["TARGET_NAME"] + "_rot_LvlhRotState_lvlh_body(target, dynamics)")



  if "TARGET_REF" in options :
     eval("set_" + options["TARGET_NAME"] + "_reference_" + options["TARGET_REF"] + "_trans_state(target)")







  target.lvlh.reference_name = "Earth"
  target.ned.reference_name  = "Earth"

  if "TARGET_ELLIPTICAL_NED" in options :
     target.ned.ned_state.altlatlong_type = trick.NorthEastDown.elliptical
  else :
     target.ned.ned_state.altlatlong_type = trick.NorthEastDown.spherical

  set_target_rel_state(options["TARGET_NAME"])
  set_chaser_rel_state(options["CHASER_NAME"], options["TARGET_NAME"])

  #// Disable jobs
  # trick_sys.sched.set_job_onoff("orb_elem.from_cartesian", 0)


  #// This simulation is an initialization sim only.
  trick.exec_set_terminate_time(0)

  return
