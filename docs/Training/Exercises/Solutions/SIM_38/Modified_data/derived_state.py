vehicleA.lvlh.reference_name = "Earth"
vehicleB.lvlh.reference_name = "Earth"

vehicleA.orb_elem.reference_name = "Earth"
vehicleB.orb_elem.reference_name = "Earth"

vehicleA.pfix.reference_name = "Earth"
vehicleB.pfix.reference_name = "Earth"

relative.vehicleA_wrt_vehicleB.subject_frame_name = "vehicleA.composite_body"
relative.vehicleA_wrt_vehicleB.target_frame_name = "vehicleB.Earth.lvlh"
relative.vehicleA_wrt_vehicleB.name = "A_wrt_B"
relative.vehicleA_wrt_vehicleB.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget

relative.vehicleB_wrt_vehicleA.subject_frame_name = "vehicleB.composite_body"
relative.vehicleB_wrt_vehicleA.target_frame_name = "vehicleA.Earth.lvlh"
relative.vehicleB_wrt_vehicleA.name = "B_wrt_A"
relative.vehicleB_wrt_vehicleA.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget

relative.pointA_wrt_vehicleB_lvlhB.subject_frame_name = "vehicleA.point"
relative.pointA_wrt_vehicleB_lvlhB.target_frame_name = "vehicleB.Earth.lvlh"
relative.pointA_wrt_vehicleB_lvlhB.name = "ptA_wrt_vehB_lvlh"
relative.pointA_wrt_vehicleB_lvlhB.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget

relative.pointA_wrt_pointB_strucB.subject_frame_name = "vehicleA.point"
relative.pointA_wrt_pointB_strucB.target_frame_name = "vehicleB.point"
relative.pointA_wrt_pointB_strucB.name = "ptA_wrt_ptB_struc"
relative.pointA_wrt_pointB_strucB.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget

relative.pointB_wrt_vehicleB_lvlhB.subject_frame_name = "vehicleB.point"
relative.pointB_wrt_vehicleB_lvlhB.target_frame_name = "vehicleB.Earth.lvlh"
relative.pointB_wrt_vehicleB_lvlhB.name = "ptB_wrt_vehB_lvlh"
relative.pointB_wrt_vehicleB_lvlhB.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget
