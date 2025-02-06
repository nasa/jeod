################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_chaser_rel_state 
####################################################################################

#Auto-generated from the data file with this header:
#/* Initialize relative state computations */

def set_chaser_rel_state(chaser_name, target_name) :

  chaser_rel_state.cbody_wrt_tbody.subject_frame_name = chaser_name + ".composite_body"
  chaser_rel_state.cbody_wrt_tbody.target_frame_name =  target_name + ".composite_body"
  chaser_rel_state.cbody_wrt_tbody.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  chaser_rel_state.cbody_wrt_tlvlh.subject_frame_name = chaser_name + ".composite_body"
  chaser_rel_state.cbody_wrt_tlvlh.target_frame_name =  target_name + ".Earth.lvlh"
  chaser_rel_state.cbody_wrt_tlvlh.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  chaser_rel_state.cbody_wrt_tned.subject_frame_name = chaser_name + ".composite_body"
  chaser_rel_state.cbody_wrt_tned.target_frame_name =  target_name + ".Earth.ned"
  chaser_rel_state.cbody_wrt_tned.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  chaser_rel_state.cstruct_wrt_tbody.subject_frame_name = chaser_name + ".structure"
  chaser_rel_state.cstruct_wrt_tbody.target_frame_name =  target_name + ".composite_body"
  chaser_rel_state.cstruct_wrt_tbody.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  chaser_rel_state.cstruct_wrt_tstruct.subject_frame_name = chaser_name + ".structure"
  chaser_rel_state.cstruct_wrt_tstruct.target_frame_name =  target_name + ".structure"
  chaser_rel_state.cstruct_wrt_tstruct.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  chaser_rel_state.cstruct_wrt_tned.subject_frame_name = chaser_name + ".structure"
  chaser_rel_state.cstruct_wrt_tned.target_frame_name =  target_name + ".Earth.ned"
  chaser_rel_state.cstruct_wrt_tned.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget

  chaser_rel_state.cpoint_wrt_tpoint.subject_frame_name = chaser_name + ".attach_point"
  chaser_rel_state.cpoint_wrt_tpoint.target_frame_name =  target_name + ".attach_point"
  chaser_rel_state.cpoint_wrt_tpoint.direction_sense =    trick.RelativeDerivedState.ComputeSubjectStateinTarget
