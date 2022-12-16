################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_trans_Orbit_pfix_body_set02
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify STS-114 orbit, planet-fixed reference, set #02,
#   at STS_114 MET 001:19:30:59.000.
#   Data based on state vectors from
#   The Flight Operations Directorate, NASA JSC )
#
#
#
#*******************************************************************************/

def set_STS_114_trans_Orbit_pfix_body_set02(vehicle_reference, dynamics) :




  vehicle_reference.orb_init.dyn_subject = vehicle_reference.dyn_body
  vehicle_reference.orb_init.action_name = "STS_114.trans_Orbit_pfix_body_set02"

  vehicle_reference.orb_init.reference_ref_frame_name = "Earth.pfix"
  vehicle_reference.orb_init.body_frame_id = "composite_body"

  vehicle_reference.orb_init.set              = 2
  vehicle_reference.orb_init.orbit_frame_name = "Earth.pfix"
  vehicle_reference.orb_init.planet_name      = "Earth"
  vehicle_reference.orb_init.arg_periapsis  = trick.attach_units( "degree",103.233805460)
  vehicle_reference.orb_init.eccentricity            =    0.00122446412
  vehicle_reference.orb_init.inclination  = trick.attach_units( "degree",51.649034566)
  vehicle_reference.orb_init.ascending_node  = trick.attach_units( "degree",311.090372692)
  vehicle_reference.orb_init.mean_anomaly  = trick.attach_units( "degree",297.384626828)
  vehicle_reference.orb_init.semi_major_axis  = trick.attach_units( "km",6732.16361166)



  dynamics.dyn_manager.add_body_action (vehicle_reference.orb_init)

  return
