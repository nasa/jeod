def set_target_rel_state(target_name) :

  target_rel_state.tstruct_wrt_pad39a.subject_frame_name =    target_name + ".structure"
  target_rel_state.tstruct_wrt_pad39a.target_frame_name =    "Earth.pad_39a.ned"
  target_rel_state.tstruct_wrt_pad39a.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  target_rel_state.tbody_wrt_pfix.subject_frame_name =    target_name + ".composite_body"
  target_rel_state.tbody_wrt_pfix.target_frame_name =    "Earth.pfix"
  target_rel_state.tbody_wrt_pfix.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  target_rel_state.tbody_wrt_tlvlh.subject_frame_name =    target_name + ".composite_body"
  target_rel_state.tbody_wrt_tlvlh.target_frame_name =    target_name + ".Earth.lvlh"
  target_rel_state.tbody_wrt_tlvlh.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  target_rel_state.tbody_wrt_tned.subject_frame_name =    target_name + ".composite_body"
  target_rel_state.tbody_wrt_tned.target_frame_name =    target_name + ".Earth.ned"
  target_rel_state.tbody_wrt_tned.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget
