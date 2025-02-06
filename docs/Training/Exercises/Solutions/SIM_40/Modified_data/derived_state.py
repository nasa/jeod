vehicleA.lvlh_frame.subject_name = "vehicleA.composite_body"
vehicleA.lvlh_frame.planet_name = "Earth"

vehicleB.lvlh_frame.subject_name = "vehicleB.composite_body"
vehicleB.lvlh_frame.planet_name = "Earth"

vehicleA.lvlh_frame_struc.subject_name = "vehicleA.structure"
vehicleA.lvlh_frame_struc.planet_name = "Earth"

vehicleB.lvlh_frame_struc.subject_name = "vehicleB.structure"
vehicleB.lvlh_frame_struc.planet_name = "Earth"



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


relative.bodyB_wrt_bodyA_in_bodyA.subject_frame_name = "vehicleB.composite_body"
relative.bodyB_wrt_bodyA_in_bodyA.target_frame_name = "vehicleA.composite_body"
relative.bodyB_wrt_bodyA_in_bodyA.name = "B_wrt_A"
relative.bodyB_wrt_bodyA_in_bodyA.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget


relative.bodyA_wrt_bodyB_in_lvlh_bodyB.subject_frame_name = "vehicleA.composite_body"
relative.bodyA_wrt_bodyB_in_lvlh_bodyB.target_frame_name = "vehicleB.composite_body.Earth.lvlh"
relative.bodyA_wrt_bodyB_in_lvlh_bodyB.name = "A_wrt_B_lvlh"
relative.bodyA_wrt_bodyB_in_lvlh_bodyB.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget


relative.bodyB_wrt_bodyA_in_lvlh_bodyA.subject_frame_name = "vehicleB.composite_body"
relative.bodyB_wrt_bodyA_in_lvlh_bodyA.target_frame_name = "vehicleA.composite_body.Earth.lvlh"
relative.bodyB_wrt_bodyA_in_lvlh_bodyA.name = "B_wrt_A_lvlh"
relative.bodyB_wrt_bodyA_in_lvlh_bodyA.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget


relative.bodyA_wrt_strucB_in_curvi_lvlh_strucB.subject_frame_name = "vehicleA.composite_body"
relative.bodyA_wrt_strucB_in_curvi_lvlh_strucB.target_frame_name = "vehicleB.structure.Earth.lvlh"
relative.bodyA_wrt_strucB_in_curvi_lvlh_strucB.name = "A_wrt_B_clvlh"
relative.bodyA_wrt_strucB_in_curvi_lvlh_strucB.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget
relative.bodyA_wrt_strucB_in_curvi_lvlh_strucB.lvlh_type = trick.LvlhType.CircularCurvilinear


relative.bodyB_wrt_strucA_in_curvi_lvlh_strucA.subject_frame_name = "vehicleB.composite_body"
relative.bodyB_wrt_strucA_in_curvi_lvlh_strucA.target_frame_name = "vehicleA.structure.Earth.lvlh"
relative.bodyB_wrt_strucA_in_curvi_lvlh_strucA.name = "B_wrt_A_clvlh"
relative.bodyB_wrt_strucA_in_curvi_lvlh_strucA.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget
relative.bodyB_wrt_strucA_in_curvi_lvlh_strucA.lvlh_type = trick.LvlhType.CircularCurvilinear


relative.bodyB_wrt_strucA_in_lvlh_strucA.subject_frame_name = "vehicleB.composite_body"
relative.bodyB_wrt_strucA_in_lvlh_strucA.target_frame_name = "vehicleA.structure.Earth.lvlh"
relative.bodyB_wrt_strucA_in_lvlh_strucA.name = "B_wrt_A_lvlh_struc"
relative.bodyB_wrt_strucA_in_lvlh_strucA.direction_sense = trick.RelativeDerivedState.ComputeSubjectStateinTarget
