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

def set_vehicle_grav_controls(veh_obj_reference) :



  #/* Set up the gravity controls for the Earth. */
  veh_obj_reference.earth_grav_ctrl.source_name = "Earth"
  veh_obj_reference.earth_grav_ctrl.active      = 1
  veh_obj_reference.earth_grav_ctrl.spherical   = 0
  veh_obj_reference.earth_grav_ctrl.degree      = 36
  veh_obj_reference.earth_grav_ctrl.order       = 36
  veh_obj_reference.earth_grav_ctrl.gradient    = True

  veh_obj_reference.body.grav_interaction.add_control(veh_obj_reference.earth_grav_ctrl)

  #/* Set up the gravity controls for the Sun. */
  veh_obj_reference.sun_grav_ctrl.source_name   = "Sun"
  veh_obj_reference.sun_grav_ctrl.active        = 1
  veh_obj_reference.sun_grav_ctrl.spherical     = 1
  veh_obj_reference.sun_grav_ctrl.gradient      = True

  veh_obj_reference.body.grav_interaction.add_control(veh_obj_reference.sun_grav_ctrl)

  #/* Set up the gravity controls for the Moon. */
  veh_obj_reference.moon_grav_ctrl.source_name  = "Moon"
  veh_obj_reference.moon_grav_ctrl.active       = 1
  veh_obj_reference.moon_grav_ctrl.spherical    = 1
  veh_obj_reference.moon_grav_ctrl.gradient     = True

  veh_obj_reference.body.grav_interaction.add_control(veh_obj_reference.moon_grav_ctrl)


  return
