vehicleA.lvlh_frame.subject_name = "vehicleA.composite_body"
vehicleB.lvlh_frame.subject_name = "vehicleB.composite_body"

vehicleA.lvlh_frame.planet_name = "Earth"
vehicleB.lvlh_frame.planet_name = "Earth"

vehicleA.lvlh.name               = "vehicleA_in_vehicleB_LVLH"
vehicleA.lvlh.subject_frame_name = "vehicleA.composite_body"
vehicleA.lvlh.target_frame_name  = "vehicleB.composite_body.Earth.lvlh"
vehicleA.lvlh.lvlh_type          = trick.LvlhType.Rectilinear

vehicleB.lvlh.name               = "vehicleB_in_vehicleA_LVLH"
vehicleB.lvlh.subject_frame_name = "vehicleB.composite_body"
vehicleB.lvlh.target_frame_name  = "vehicleA.composite_body.Earth.lvlh"
vehicleB.lvlh.lvlh_type          = trick.LvlhType.Rectilinear
