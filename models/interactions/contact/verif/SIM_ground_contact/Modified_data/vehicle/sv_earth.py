#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (This data file provides a template of a general space vehicle.  This
#    data file is intended to be included in an input file and then
#    extended.)
#
#REFERENCE:
#   ((JSC Engineering Orbital Dynamics Models))
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((Relies on the following defined parameters:
#        SV_NAME: Name of the vehicle
#        SV_DYN:  SIM_OBJECT name for the space vehicle dynamics
#        SV_AUX:  SIM_OBJECT name for the space vehicle auxiliary state)
#    (Initialized about the Earth.))
#
#
#*/

def set_vehicle_earth(sv_dyn_reference, SV_NAME) :


  #/*
  # * Configure the vehicle.
  # */

  #/* Set the vehicle name. */
  exec('sv_dyn_reference.body.set_name("' + SV_NAME + '")')

  # Configure vehicle integration information.
  sv_dyn_reference.body.integ_frame_name = "Earth.inertial"
  sv_dyn_reference.body.translational_dynamics = True
  sv_dyn_reference.body.rotational_dynamics = True

  # Set up the gravity controls for the Earth.
  sv_dyn_reference.earth_grav_control.source_name = "Earth"
  sv_dyn_reference.earth_grav_control.active      = True
  sv_dyn_reference.earth_grav_control.spherical   = True
  sv_dyn_reference.earth_grav_control.degree      = 0
  sv_dyn_reference.earth_grav_control.order       = 0
  sv_dyn_reference.earth_grav_control.gradient    = False

  sv_dyn_reference.body.grav_interaction.add_control(sv_dyn_reference.earth_grav_control)

  return
