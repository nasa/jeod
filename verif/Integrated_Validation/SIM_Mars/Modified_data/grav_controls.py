# Set up the gravity controls for Mars.
dawn.grav_control.source_name = "Mars"
dawn.grav_control.active      = True
dawn.grav_control.spherical   = False
dawn.grav_control.degree      = 110
dawn.grav_control.order       = 110
dawn.grav_control.gradient    = False

# Set up the gravity controls for the Sun.
dawn.sun_grav_control.source_name = "Sun"
dawn.sun_grav_control.active      = True
dawn.sun_grav_control.spherical   = True
dawn.sun_grav_control.gradient    = False

dawn.dyn_body.grav_interaction.add_control(dawn.grav_control)
dawn.dyn_body.grav_interaction.add_control(dawn.sun_grav_control)

def set_grav_controls_8x8():
  dawn.grav_control.degree      = 8
  dawn.grav_control.order       = 8
