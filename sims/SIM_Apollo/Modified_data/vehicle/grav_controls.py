#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (This data file sets up the vehicle gravity model controls.)
#
#REFERENCE:
#   ((JSC Engineering Orbital Dynamics Models))
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((?))
#
# 
#*/

def set_vehicle_grav_controls(sv_dyn_reference) :
  # Set up the gravity controls for the Earth.
  sv_dyn_reference.earth_grav_control.source_name = "Earth"
  sv_dyn_reference.earth_grav_control.active      = True
  sv_dyn_reference.earth_grav_control.spherical   = False
  sv_dyn_reference.earth_grav_control.degree      = 36
  sv_dyn_reference.earth_grav_control.order       = 36
  sv_dyn_reference.earth_grav_control.gradient    = False

  # Set up the gravity controls for the Moon.
  sv_dyn_reference.moon_grav_control.source_name = "Moon"
  sv_dyn_reference.moon_grav_control.active      = True
  sv_dyn_reference.moon_grav_control.spherical   = True
  sv_dyn_reference.moon_grav_control.gradient    = False

  # Set up the gravity controls for the Sun.
  sv_dyn_reference.sun_grav_control.source_name = "Sun"
  sv_dyn_reference.sun_grav_control.active      = True
  sv_dyn_reference.sun_grav_control.spherical   = True
  sv_dyn_reference.sun_grav_control.gradient    = False

  #CALL
  sv_dyn_reference.dyn_body.grav_interaction.add_control(sv_dyn_reference.earth_grav_control)
  sv_dyn_reference.dyn_body.grav_interaction.add_control(sv_dyn_reference.moon_grav_control)
  sv_dyn_reference.dyn_body.grav_interaction.add_control(sv_dyn_reference.sun_grav_control)
