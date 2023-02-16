################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_PAD_39A_mass
####################################################################################

#Auto-generated from the data file with this header:
#/*
#Purpose:
#  (Orbital body properties for test case.)
#
#*/

def set_PAD_39A_mass(vehicle_reference, dynamics) :


  vehicle_reference.mass_init.set_subject_body( vehicle_reference.dyn_body.mass )
  vehicle_reference.mass_init.action_name = "PAD_39A.mass"

  vehicle_reference.mass_init.properties.mass  = trick.attach_units( "kg",1.0)
  vehicle_reference.mass_init.properties.position  = trick.attach_units( "m",[ 0.0, 0.0, 0.0])

  vehicle_reference.mass_init.properties.inertia_spec = trick.MassPropertiesInit.Body
  vehicle_reference.mass_init.properties.inertia[0]  = trick.attach_units( "kg*m2",[  1.0,  0.0,  0.0])
  vehicle_reference.mass_init.properties.inertia[1]  = trick.attach_units( "kg*m2",[  0.0,  1.0,  0.0])
  vehicle_reference.mass_init.properties.inertia[2]  = trick.attach_units( "kg*m2",[  0.0,  0.0,  1.0])

  vehicle_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputMatrix


  vehicle_reference.mass_init.properties.pt_frame_spec =    trick.MassPointInit.StructToBody


  vehicle_reference.mass_init.properties.pt_orientation.trans[0]  = [  1.0,  0.0,  0.0]
  vehicle_reference.mass_init.properties.pt_orientation.trans[1]  = [  0.0,  1.0,  0.0]
  vehicle_reference.mass_init.properties.pt_orientation.trans[2]  = [  0.0,  0.0,  1.0]

  vehicle_reference.mass_init.num_points = 1
  vehicle_reference.mass_init.points = trick.sim_services.alloc_type(1, "jeod::MassPointInit")
  vehicle_reference.mass_init.points[0].set_name("attach_point")
  vehicle_reference.mass_init.points[0].position  = trick.attach_units( "m",[ 1.0, 0.0, 0.0])
  vehicle_reference.mass_init.points[0].pt_orientation.trans[0]  = [ 1.0, 0.0, 0.0]
  vehicle_reference.mass_init.points[0].pt_orientation.trans[1]  = [ 0.0, 1.0, 0.0]
  vehicle_reference.mass_init.points[0].pt_orientation.trans[2]  = [ 0.0, 0.0, 1.0]
  vehicle_reference.mass_init.points[0].pt_orientation.data_source =    trick.Orientation.InputMatrix


  vehicle_reference.mass_init.points[0].pt_frame_spec =    trick.MassPointInit.StructToPoint




  dynamics.dyn_manager.add_body_action (vehicle_reference.mass_init)

  return
