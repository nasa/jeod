################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_trans_Orbit_pfix_body_set06
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify STS-114 orbit, planet-fixed reference, set #06,
#   at STS_114 MET 001:19:30:59.000.
#   Data based on state vectors from
#   The Flight Operations Directorate, NASA JSC )
#
#
#
#*******************************************************************************/

def set_STS_114_trans_Orbit_pfix_body_set06(vehicle_reference, dynamics) :




  vehicle_reference.orb_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.orb_init.action_name = "STS_114.trans_Orbit_pfix_body_set06"

  vehicle_reference.orb_init.reference_ref_frame_name = "Earth.pfix"
  vehicle_reference.orb_init.body_frame_id = "composite_body"

  vehicle_reference.orb_init.set              = 6
  vehicle_reference.orb_init.orbit_frame_name = "Earth.pfix"
  vehicle_reference.orb_init.planet_name      = "Earth"
  vehicle_reference.orb_init.arg_latitude  = trick.attach_units( "degree",400.493755029)
  vehicle_reference.orb_init.inclination  = trick.attach_units( "degree",51.649034566)
  vehicle_reference.orb_init.ascending_node  = trick.attach_units( "degree",311.090372692)
  vehicle_reference.orb_init.radial_vel  = trick.attach_units( "m/s",-8.37547080)
  vehicle_reference.orb_init.orb_radius  = trick.attach_units( "km",6728.37997875)
  vehicle_reference.orb_init.semi_major_axis  = trick.attach_units( "km",6732.16361166)



  dynamics.dyn_manager.add_body_action (vehicle_reference.orb_init)

  return
