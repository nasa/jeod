def grav_ctrl(VEH) :
  VEH.grav_control.source_name = "Earth"
  VEH.grav_control.active = True
  VEH.grav_control.spherical = True

  VEH.dyn_body.grav_interaction.add_control(VEH.grav_control)
  return
