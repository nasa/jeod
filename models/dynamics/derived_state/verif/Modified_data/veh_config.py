def veh_config( veh, suffix = ""):
  #Body settings
  veh.dyn_body.set_name("vehicle"+suffix)
  veh.dyn_body.integ_frame_name = "Earth.inertial"

  veh.dyn_body.translational_dynamics = True
  veh.dyn_body.rotational_dynamics = False

  # Gravity settings
  veh.grav_control.source_name   = "Earth"
  veh.grav_control.active        = 1
  veh.grav_control.spherical     = 1
  veh.grav_control.degree        = 8
  veh.grav_control.order         = 8
  veh.dyn_body.grav_interaction.add_control(veh.grav_control)

def veh_config_alt( veh, suffix, base_frame):
  veh.dyn_body.set_name("vehicle"+suffix)
  veh.dyn_body.integ_frame_name = base_frame+".inertial"
  veh.dyn_body.translational_dynamics = True
  veh.dyn_body.rotational_dynamics = True
