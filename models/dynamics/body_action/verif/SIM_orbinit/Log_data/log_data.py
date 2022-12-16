#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for orbital state initialization data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
#
#*/


def log_data ( options ) :

   # Import the JEOD logger
   import sys
   import os
   sys.path.append ('/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]))
   import jeod_log

   # Create the logger.
   logger = jeod_log.Logger ()


   # parse out what does and does not need to be logged

   DO_LOG_BASE = False
   DO_LOG_TARGET = False
   DO_LOG_CHASER = False
   LOG_ORB_ELEM = False
   LOG_TARGET_TRANS = False
   LOG_TARGET_STRUCT = False
   LOG_TARGET_ATT = False
   LOG_TARGET_RATE = False
   LOG_CHASER_TRANS = False
   LOG_CHASER_STRUCT = False
   LOG_CHASER_ATT = False
   LOG_CHASER_RATE = False


   if "LOG_EARTH" in options :
      DO_LOG_BASE = True
      LOG_EARTH = True
   if "LOG_ORB_ELEM" in options :
      DO_LOG_BASE = True
      LOG_ORB_ELEM = True
   if "LOG_TARGET_TRANS" in options :
      DO_LOG_TARGET = True
      LOG_TARGET_TRANS = True
   if "LOG_TARGET_STRUCT" in options :
      DO_LOG_TARGET = True
      LOG_TARGET_STRUCT = True
   if "LOG_TARGET_ATT" in options :
      DO_LOG_TARGET = True
      LOG_TARGET_ATT = True
   if "LOG_TARGET_RATE" in options :
      DO_LOG_TARGET = True
      LOG_TARGET_RATE = True
   if "LOG_CHASER_TRANS" in options :
      DO_LOG_CHASER = True
      LOG_CHASER_TRANS = True
   if "LOG_CHASER_STRUCT" in options :
      DO_LOG_CHASER = True
      LOG_CHASER_STRUCT = True
   if "LOG_CHASER_ATT" in options :
      DO_LOG_CHASER = True
      LOG_CHASER_ATT = True
   if "LOG_CHASER_RATE" in options :
      DO_LOG_CHASER = True
      LOG_CHASER_RATE = True


   if DO_LOG_BASE :

      logger.open_group (1, "base")

      logger.log_matrix ("earth.planet.pfix.state.rot.T_parent_this")

      if LOG_ORB_ELEM :
         logger.log_scalar (
            ("target.orb_elem.semi_major_axis",
             "target.orb_elem.semiparam",
             "target.orb_elem.e_mag",
             "target.orb_elem.inclination",
             "target.orb_elem.arg_periapsis",
             "target.orb_elem.long_asc_node",
             "target.orb_elem.r_mag",
             "target.orb_elem.vel_mag",
             "target.orb_elem.true_anom",
             "target.orb_elem.mean_anom",
             "target.orb_elem.mean_motion",
             "target.orb_elem.orbital_anom"))

      logger.close_group()


   if DO_LOG_TARGET :

      logger.open_group (1, "target")

      if LOG_TARGET_TRANS :
         logger.log_trans_state (
            ("target.trans_init",
             "target.ned_init",
             "target.ned_trans_init",
             "target.dyn_body.composite_body.state.trans",
             "target.dyn_body.structure.state.trans",
             "target.expected_state.trans",
             "target_rel_state.tstruct_wrt_pad39a.rel_state.trans",
             "target_rel_state.tbody_wrt_pfix.rel_state.trans"))

      if LOG_TARGET_STRUCT :
         logger.log_matrix (
            ("target.mass_init.properties.pt_orientation.trans",
             "target.dyn_body.composite_properties.T_parent_this"))

      if LOG_TARGET_ATT :
         logger.log_set (
            ("target.att_init.orientation",
             "target.lvlh_init.orientation",
             "target.ned_init.orientation",
             "target.lvlh_att_init.orientation",
             "target.ned_att_init.orientation"),
            (("euler_sequence", 'scalar'),
             ("euler_angles", 'vector')))

         logger.log_matrix (
            ("target.dyn_body.composite_body.state.rot.T_parent_this",
             "target.dyn_body.structure.state.rot.T_parent_this",
             "target_rel_state.tstruct_wrt_pad39a.rel_state.rot.T_parent_this",
             "target_rel_state.tbody_wrt_pfix.rel_state.rot.T_parent_this",
             "target_rel_state.tbody_wrt_tlvlh.rel_state.rot.T_parent_this",
             "target_rel_state.tbody_wrt_tned.rel_state.rot.T_parent_this"))

      if LOG_TARGET_RATE :
         logger.log_set (
            ("target.att_init",
             "target.lvlh_init",
             "target.ned_init",
             "target.lvlh_att_init",
             "target.ned_att_init",
             "target.rate_init",
             "target.lvlh_rate_init",
             "target.ned_rate_init"),
            (("ang_velocity", 'vector'),
             ("rate_in_parent", 'scalar')))

         logger.log_vector (
            ("target.dyn_body.composite_body.state.rot.ang_vel_this",
             "target.dyn_body.structure.state.rot.ang_vel_this",
             "target_rel_state.tstruct_wrt_pad39a.rel_state.rot.ang_vel_this",
             "target_rel_state.tbody_wrt_pfix.rel_state.rot.ang_vel_this",
             "target_rel_state.tbody_wrt_tlvlh.rel_state.rot.ang_vel_this",
             "target_rel_state.tbody_wrt_tned.rel_state.rot.ang_vel_this"))

      logger.close_group ()


   if DO_LOG_CHASER :

      logger.open_group (1, "chaser")

      if LOG_CHASER_TRANS :
         logger.log_trans_state (
            ("chaser.trans_init",
             "chaser.lvlh_trans_init",
             "chaser.ned_trans_init",
             "chaser.lvlh_init",
             "chaser.ned_init",
             "chaser.dyn_body.composite_body.state.trans",
             "chaser.dyn_body.structure.state.trans",
             "chaser_rel_state.cbody_wrt_tbody.rel_state.trans",
             "chaser_rel_state.cbody_wrt_tlvlh.rel_state.trans",
             "chaser_rel_state.cbody_wrt_tned.rel_state.trans",
             "chaser_rel_state.cstruct_wrt_tbody.rel_state.trans",
             "chaser_rel_state.cstruct_wrt_tstruct.rel_state.trans",
             "chaser_rel_state.cstruct_wrt_tned.rel_state.trans",
             "chaser_rel_state.cpoint_wrt_tpoint.rel_state.trans"))

      if LOG_CHASER_STRUCT :
         logger.log_matrix (
            ("chaser.mass_init.properties.pt_orientation.trans",
             "chaser.dyn_body.composite_properties.T_parent_this"))

      if LOG_CHASER_ATT :
         logger.log_set (
            ("chaser.att_init.orientation",
             "chaser.lvlh_att_init.orientation",
             "chaser.ned_att_init.orientation",
             "chaser.lvlh_init.orientation",
             "target.ned_init.orientation"),
            (("euler_sequence", 'scalar'),
             ("euler_angles", 'vector')))

         logger.log_matrix (
            ("chaser.dyn_body.composite_body.state.rot.T_parent_this",
             "chaser.dyn_body.structure.state.rot.T_parent_this",
             "chaser_rel_state.cbody_wrt_tbody.rel_state.rot.T_parent_this",
             "chaser_rel_state.cbody_wrt_tlvlh.rel_state.rot.T_parent_this",
             "chaser_rel_state.cbody_wrt_tned.rel_state.rot.T_parent_this",
             "chaser_rel_state.cstruct_wrt_tbody.rel_state.rot.T_parent_this",
             "chaser_rel_state.cstruct_wrt_tstruct.rel_state.rot.T_parent_this",
             "chaser_rel_state.cstruct_wrt_tned.rel_state.rot.T_parent_this",
             "chaser_rel_state.cpoint_wrt_tpoint.rel_state.rot.T_parent_this"))

      if LOG_CHASER_RATE :
         logger.log_set (
            ("chaser.att_init",
             "chaser.lvlh_att_init",
             "chaser.ned_att_init",
             "chaser.rate_init",
             "chaser.lvlh_rate_init",
             "chaser.ned_rate_init",
             "chaser.lvlh_init",
             "chaser.ned_init"),
            (("ang_velocity", 'vector'),
             ("rate_in_parent", 'scalar')))

         logger.log_vector (
            ("chaser.dyn_body.composite_body.state.rot.ang_vel_this",
             "chaser.dyn_body.structure.state.rot.ang_vel_this",
             "chaser_rel_state.cbody_wrt_tbody.rel_state.rot.ang_vel_this",
             "chaser_rel_state.cbody_wrt_tlvlh.rel_state.rot.ang_vel_this",
             "chaser_rel_state.cbody_wrt_tned.rel_state.rot.ang_vel_this",
             "chaser_rel_state.cstruct_wrt_tbody.rel_state.rot.ang_vel_this",
             "chaser_rel_state.cstruct_wrt_tstruct.rel_state.rot.ang_vel_this",
             "chaser_rel_state.cstruct_wrt_tned.rel_state.rot.ang_vel_this",
             "chaser_rel_state.cpoint_wrt_tpoint.rel_state.rot.ang_vel_this"))

      logger.close_group ()
