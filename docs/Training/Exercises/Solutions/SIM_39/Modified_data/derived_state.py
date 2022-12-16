vehicleA.lvlh_frame.subject_name = "vehicleA.composite_body"
vehicleB.lvlh_frame.subject_name = "vehicleB.composite_body"

vehicleA.lvlh_frame.planet_name = "Earth"
vehicleB.lvlh_frame.planet_name = "Earth"

relative.pointA_wrt_bodyB_in_lvlh_bodyB.subject_frame_name = "vehicleA.point"
relative.pointA_wrt_bodyB_in_lvlh_bodyB.target_frame_name = "vehicleB.composite_body.Earth.lvlh"
relative.pointA_wrt_bodyB_in_lvlh_bodyB.name = "ptA_wrt_vehB_lvlh"
relative.pointA_wrt_bodyB_in_lvlh_bodyB.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget

relative.pointB_wrt_bodyB_in_lvlh_bodyB.subject_frame_name = "vehicleB.point"
relative.pointB_wrt_bodyB_in_lvlh_bodyB.target_frame_name = "vehicleB.composite_body.Earth.lvlh"
relative.pointB_wrt_bodyB_in_lvlh_bodyB.name = "ptB_wrt_vehB_lvlh"
relative.pointB_wrt_bodyB_in_lvlh_bodyB.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget

relative.pointA_wrt_pointB_in_strucB.subject_frame_name = "vehicleA.point"
relative.pointA_wrt_pointB_in_strucB.target_frame_name = "vehicleB.point"
relative.pointA_wrt_pointB_in_strucB.name = "ptA_wrt_ptB_struc"
relative.pointA_wrt_pointB_in_strucB.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget
