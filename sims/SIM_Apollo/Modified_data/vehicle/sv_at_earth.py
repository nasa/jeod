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

def set_vehicle_sv_at_earth(sv_dyn_reference, earth) :


  #/*
  # * Configure the vehicle.
  # */

  #/* Set the vehicle name. */

  #/* Configure vehicle integration information. */
  sv_dyn_reference.dyn_body.integ_frame_name = "Earth.inertial"
  sv_dyn_reference.dyn_body.translational_dynamics = True
  sv_dyn_reference.dyn_body.rotational_dynamics = True
  #/* Configure the dervied reference frames. */
  sv_dyn_reference.pfix.reference_name     = "Earth"
  sv_dyn_reference.lvlh.reference_name     = "Earth"
  sv_dyn_reference.orb_elem.reference_name = "Earth"



  #/* Make the Earth gravity GEMT1 8x8. */
  sv_dyn_reference.earth_grav_control.spherical = 0
  sv_dyn_reference.earth_grav_control.degree    = 8
  sv_dyn_reference.earth_grav_control.order     = 8

  #/* Configure planetary ephemeris model. */
  #/* Turn off the Sun and Moon gravity affects. */
  sv_dyn_reference.sun_grav_control.active  = 1
  sv_dyn_reference.moon_grav_control.active = 1

  #/* Configure the vehicle atmosphere. */
  sv_dyn_reference.atmos_state.active    = 1

  #/* Configure aerodynamics. */
  #//#include "Modified_data/aero/drag.d"
  #/* Turn on the general aerodynamics interface. */
  sv_dyn_reference.aero_drag.active = 1

  #/* Set up the aerodynamic drag parameters. */
  sv_dyn_reference.aero_drag.ballistic_drag.area = 1.0
  sv_dyn_reference.aero_drag.ballistic_drag.Cd = 0.02
  sv_dyn_reference.aero_drag.ballistic_drag.BC = 50.0
  sv_dyn_reference.aero_drag.ballistic_drag.option = trick.DefaultAero.DRAG_OPT_CD
