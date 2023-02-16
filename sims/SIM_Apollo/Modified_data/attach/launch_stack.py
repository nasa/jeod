################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_launch_stack 
####################################################################################

#Auto-generated from the data file with this header:
#/* Attach list for nominal launch stack configuration. */

def set_launch_stack(dynamics, dm_mass, lm_dyn, sm_dyn, cm_dyn, s3_dyn, s2_dyn, s1_dyn, les_dyn) :

  #/**********************************************************/
  #/* Attach Lunar Module descent stage to the ascent stage. */
  #/**********************************************************/
  dm_mass.mass_attach.set_subject_body( dm_mass.body )
  dm_mass.mass_attach.set_parent_body( lm_dyn.dyn_body.mass )
  dm_mass.mass_attach.subject_point_name = "Ascent Module interface"
  dm_mass.mass_attach.parent_point_name  = "Descent Module interface"

  dynamics.dyn_manager.add_body_action(dm_mass.mass_attach)

  #/**********************************/
  #/* Configure launch system stack. */
  #/**********************************/

  #// Specify that Service Module is attached to the Command Module.
  sm_dyn.mass_attach.set_subject_body( sm_dyn.dyn_body.mass )
  sm_dyn.mass_attach.set_parent_body( cm_dyn.dyn_body.mass )
  sm_dyn.mass_attach.subject_point_name = "CM interface"
  sm_dyn.mass_attach.parent_point_name  = "SM interface"

  dynamics.dyn_manager.add_body_action(sm_dyn.mass_attach)

  #// Specify that Stage 3 is attached to the Service Module.
  s3_dyn.mass_attach.set_subject_body( s3_dyn.dyn_body.mass )
  s3_dyn.mass_attach.set_parent_body( sm_dyn.dyn_body.mass )
  s3_dyn.mass_attach.subject_point_name = "LEM/SM/CM interface"
  s3_dyn.mass_attach.parent_point_name  = "Stage 3 interface"

  dynamics.dyn_manager.add_body_action(s3_dyn.mass_attach)


  #// Specify that Lunar Module is attached to Stage 3.
  lm_dyn.mass_attach.set_subject_body( lm_dyn.dyn_body.mass )
  lm_dyn.mass_attach.set_parent_body( s3_dyn.dyn_body.mass )
  lm_dyn.mass_attach.subject_point_name = "Stage 3 interface"
  lm_dyn.mass_attach.parent_point_name  = "LEM/SM/CM interface"

  dynamics.dyn_manager.add_body_action(lm_dyn.mass_attach)


  #// Specify that Stage 2 is attached to Stage 3.
  s2_dyn.mass_attach.set_subject_body( s2_dyn.dyn_body.mass )
  s2_dyn.mass_attach.set_parent_body( s3_dyn.dyn_body.mass )
  s2_dyn.mass_attach.subject_point_name = "Stage 3 interface"
  s2_dyn.mass_attach.parent_point_name  = "Stage 2 interface"

  dynamics.dyn_manager.add_body_action(s2_dyn.mass_attach)


  #// Specify that Stage 1 is attached to Stage 2.
  s1_dyn.mass_attach.set_subject_body( s1_dyn.dyn_body.mass )
  s1_dyn.mass_attach.set_parent_body( s2_dyn.dyn_body.mass )
  s1_dyn.mass_attach.subject_point_name = "Stage 2 interface"
  s1_dyn.mass_attach.parent_point_name  = "Stage 1 interface"

  dynamics.dyn_manager.add_body_action(s1_dyn.mass_attach)


  #// Specify that Launch Escape System is attached to the Command Module.
  les_dyn.mass_attach.set_subject_body( les_dyn.dyn_body.mass )
  les_dyn.mass_attach.set_parent_body( cm_dyn.dyn_body.mass )
  les_dyn.mass_attach.subject_point_name = "CM interface"
  les_dyn.mass_attach.parent_point_name  = "CM docking port"

  dynamics.dyn_manager.add_body_action(les_dyn.mass_attach)
