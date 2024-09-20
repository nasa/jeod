#=============================================================================
# Notices:
#
# Copyright 2023 United States Government as represented by the Administrator
# of the National Aeronautics and Space Administration.  All Rights Reserved.
#
#
# Disclaimers:
#
# No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
# ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
# TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
# FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
# FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
# SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
# ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
# RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
# RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
# DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
# IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
#
# Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
# UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
# PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
# LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
# INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
# USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
# UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
# PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
# ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
# AGREEMENT.
#
#=============================================================================

# format:
# [ [ model_1, [ [sim_11, [ [ RUN_111, <file_comparison>, <outcome>],
#                           [ RUN_112, <file_comparison>, <outcome>]
#                         ] #end of runs for sim_11
#                ], #end of sim_11
#                [ sim_12, [ RUN_121, <file_comparison>, <outcome>]]
#              ], #end of sims for model_1
#   ] #end of model_1
#   [ model_2, ...

# <file_comparison> can be:
#     "" (empty)            -- no comparison needed
#     ["file1","file2",...] -- list of specific files.  Note that a single file
#                              must still be in a list ["file1"]
#     "*"                   -- compare all committed files
#     any combination of filename and wildcards.

# <outcome> is optional.  Where specified, it provides the expected return
# code if the test is not expected to return 0.  Lack of specification is
# assumed a proxy for a return code 0.




verif_sim_list = [
  ["models/dynamics/body_action", [
      ["verif/SIM_lvlh_init", [
          ["RUN*",["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_orbinit", [
          ["RUN*",["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_verif_attach_mass", [
          ["RUN*",["input.py","mass.out"]]]],
      ["verif/SIM_verif_frame_switch", [
          ["RUN*",["input.py","log_V_1_State.trk"]]]]]
  ],


  ["models/dynamics/derived_state", [
      ["verif/SIM_Euler", [
          ["RUN*", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_LVLH", [
          ["RUN*", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_LvlhRelative", [
          ["RUN*", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_Relative", [
          ["RUN*", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_Planetary", [
          ["RUN*", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_OrbElem", [
          ["RUN*", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_NED", [
          ["RUN*", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_SolarBeta", [
          ["RUN_incl*", ["input.py", "*.csv"]],
          ["RUN_comp*", ["input.py", "*.csv"]]]]]
  ],

  ["models/dynamics/dyn_body", [
      ["verif/SIM_verif_attach_detach", [
          ["RUN*", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_verif_shutdown", [
          ["RUN*", ["input.py"]]]],
      ["verif/SIM_force_torque", [
          ["RUN_test", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_dyncomp_structure", [
          ["RUN_7C", ["input.py", "*.csv", "*.trk"]],
          ["RUN_7A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_4", ["input.py", "*.csv", "*.trk"]],
          ["RUN_10D", ["input.py", "*.csv", "*.trk"]],
          ["RUN_8A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_7B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_10A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_2", ["input.py", "*.csv", "*.trk"]],
          ["RUN_7D", ["input.py", "*.csv", "*.trk"]],
          ["RUN_10B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_6B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_8B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_5B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_3A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_3B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_9B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_6C", ["input.py", "*.csv", "*.trk"]],
          ["RUN_9D", ["input.py", "*.csv", "*.trk"]],
          ["RUN_9A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_1", ["input.py", "*.csv", "*.trk"]],
          ["RUN_9C", ["input.py", "*.csv", "*.trk"]],
          ["RUN_5C", ["input.py", "*.csv", "*.trk"]],
          ["RUN_6A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_6D", ["input.py", "*.csv", "*.trk"]],
          ["RUN_10C", ["input.py", "*.csv", "*.trk"]],
          ["RUN_5A", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/dynamics/rel_kin", [
      ["verif/SIM_RELKIN_VERIF", [
          ["RUN_relkin_test", ["input.py", "*.csv", "*.trk"]]]]]
  ],



  ["sims", [
      ["SIM_Apollo", [
          ["RUN_test", ["input.py", "*.csv", "*.trk", "*.out"]]]]]],

  ["verif/Integrated_Validation", [
      ["SIM_Earth_Moon", [
          ["RUN_clem", ["input.py", "*.csv", "*.trk"]],
          ["RUN_rosetta", ["input.py", "*.csv", "*.trk"]]]],
      ["SIM_Mars", [
          ["RUN_dawn", ["input.py", "*.csv", "*.trk"]],
          ["RUN_orb_init_phobos", ["input.py", "*.csv", "*.trk"]],
          ["RUN_phobos", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["verif", [
      ["SIM_dyncomp", [
          ["RUN_checkpoint_baseline", ["input.py", "*.csv", "*.trk"]],
          ["RUN_7C", ["input.py", "*.csv", "*.trk"]],
          ["RUN_7A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_4", ["input.py", "*.csv", "*.trk"]],
          ["RUN_10D", ["input.py", "*.csv", "*.trk"]],
          ["RUN_8A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_7B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_10A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_2", ["input.py", "*.csv", "*.trk"]],
          ["RUN_7D", ["input.py", "*.csv", "*.trk"]],
          ["RUN_10B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_6B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_8B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_5B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_3A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_3B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_9B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_6C", ["input.py", "*.csv", "*.trk"]],
          ["RUN_9D", ["input.py", "*.csv", "*.trk"]],
          ["RUN_9A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_1", ["input.py", "*.csv", "*.trk"]],
          ["RUN_9C", ["input.py", "*.csv", "*.trk"]],
          ["RUN_5C", ["input.py", "*.csv", "*.trk"]],
          ["RUN_6A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_6D", ["input.py", "*.csv", "*.trk"]],
          ["RUN_10C", ["input.py", "*.csv", "*.trk"]],
          ["RUN_5A", ["input.py", "*.csv", "*.trk"]]]]]],


  ["docs/Training/Exercises/Solutions/", [
      ["SIM_05", [
          ["RUN*", ""]]],
      ["SIM_06", [
          ["RUN*", ""]]],
      ["SIM_07", [
          ["RUN*", ""]]],
      ["SIM_08", [
          ["RUN*", ""]]],
      ["SIM_09", [
          ["RUN*", "",1]]],
      ["SIM_10", [
          ["RUN*", "",1]]],
      ["SIM_11", [
          ["RUN*", ""]]],
      ["SIM_12", [
          ["RUN*", ""]]],
      ["SIM_13", [
          ["RUN*", ""]]],
      ["SIM_14", [
          ["RUN*", ""]]],
      ["SIM_15", [
          ["RUN*", ""]]],
      ["SIM_16", [
          ["RUN*", ""]]],

      ["SIM_18", [
          ["RUN*", ""]]],
      ["SIM_19", [
          ["RUN*", ""]]],
      ["SIM_20", [
          ["RUN*", ""]]],
      ["SIM_21", [
          ["RUN*", ""]]],

      ["SIM_23", [
          ["RUN*", ""]]],
      ["SIM_24", [
          ["RUN*", ""]]],
      ["SIM_25", [
          ["RUN*", ""]]],
      ["SIM_26", [
          ["RUN*", ""]]],
      ["SIM_27", [
          ["RUN*", ""]]],
      ["SIM_28", [
          ["RUN*", ""]]],
      ["SIM_29", [
          ["RUN*", ""]]],
      ["SIM_30", [
          ["RUN*", ""]]],
      ["SIM_31", [
          ["RUN*", ""]]],
      ["SIM_32", [
          ["RUN*", ""]]],
      ["SIM_33", [
          ["RUN*", ""]]],
      ["SIM_34", [
          ["RUN*", ""]]],
      ["SIM_35", [
          ["RUN*", ""]]],
      ["SIM_36", [
          ["RUN*", ""]]],
      ["SIM_37", [
          ["RUN*", ""]]],
      ["SIM_38", [
          ["RUN*", ""]]],
      ["SIM_39", [
          ["RUN*", ""]]],
      ["SIM_40", [
          ["RUN*", ""]]],
      ["SIM_41", [
          ["RUN*", ""]]],
      ["SIM_42", [
          ["RUN*", ""]]]]],

  ["models/environment/gravity", [
      ["verif/SIM_tide_verif", [
          ["RUN_02", ["input.py", "*.csv", "*.trk"]],
          ["RUN_01", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_grav_accel_verif", [
          ["RUN_01", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_csr_compare", [
          ["RUN_01", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_mercury", [
          ["RUN_relativistic_sun", ["input.py", "*.csv", "*.trk"]],
          ["RUN_ephemeris", ["input.py", "*.csv", "*.trk"]],
          ["RUN_newtonian", ["input.py", "*.csv", "*.trk"]]]]]],


  ["models/environment/atmosphere/MET", [
      ["verif/SIM_MET", [
          ["RUN_T01_JAC_COMP", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T02_GRAM_MET", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T01_GRAM_MET", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T02_MET_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T03_GRAM_MET", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T01_MET_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T02_JAC_COMP", ["input.py", "*.csv", "*.trk"]],
          ["RUN_data_compare", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_wind", [
          ["RUN_test", ["input.py", "*.csv", "*.trk"]]]]]],


  ["models/environment/spice", [
      ["verif/SIM_spice", [
          ["RUN_01", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_de4xx", [
          ["RUN_01", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/environment/RNP/RNPMars", [
      ["verif/SIM_mars_orientation", [
          ["RUN_RNPMars", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/environment/RNP/RNPJ2000", [
      ["verif/SIM_RNP_J2000_prop", [
          ["RUN_J2000_RNP_prop", ["input.py", "*.csv", "*.trk"]],
          ["RUN_J2000_RNP_Transform", ["input.py", "*.csv", "*.trk"]],
          ["RUN_J2000_RNP_init", ["input.py", "*.csv", "*.trk"]],
          ["RUN_J2000_RNP_Polar_off", ["input.py", "*.csv", "*.trk"]],
          ["RUN_J2000_RNP_prop_off", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  # checkpoint run need to be execute after the normal run is complete
  # To ensure this is true. Checkpoint runs were moved to a Separate list
  # and should be executed with less cpu thread
  ["models/environment/ephemerides", [
      ["verif/SIM_prop_planet", [
          ["RUN_ephem", ["input.py", "*.csv", "*.trk"]],
          ["RUN_switch_integ", ["input.py", "*.csv", "*.trk"]],
          ["RUN_prop", ["input.py", "*.csv", "*.trk"]],
          ["RUN_prop_gj", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/environment/ephemerides/de4xx_ephem", [
      ["verif/SIM_ephem_verif", [
          ["RUN_full", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/environment/time", [
      ["verif/SIM_6_extension", [
          ["RUN_tai_initialized", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_5_all_inclusive", [
          ["RUN_UTC_initialized", ["input.py", "*.csv", "*.trk"]],
          ["RUN_UDE_initialized", ["input.py", "*.csv", "*.trk"]],
          ["RUN_UTC_initialized_tdb", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_7_time_reversal", [
          ["RUN_4", ["input.py", "*.csv", "*.trk"]],
          ["RUN_10A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_8B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_3A", ["input.py", "*.csv", "*.trk"]],
          ["RUN_3B", ["input.py", "*.csv", "*.trk"]],
          ["RUN_9D", ["input.py", "*.csv", "*.trk"]],
          ["RUN_1", ["input.py", "*.csv", "*.trk"]],
          ["RUN_6A", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_4_common_usage", [
          ["RUN_JEOD2x", ["input.py", "*.csv", "*.trk"]],
          ["RUN_JEOD1x_compatible", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_2_dyn_plus_STD", [
          ["RUN_initialize_by_calendar", ["input.py", "*.csv", "*.trk"]],
          ["RUN_initialize_by_value", ["input.py", "*.csv", "*.trk"]],
          ["RUN_fail_dyn_initialized", ["input.py"],1],
          ["RUN_fail_uninitialized", ["input.py"],1],
          ["RUN_scale_factor_changes", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_3_dyn_plus_UDE", [
          ["RUN_FAIL_init_by_dyn_w_overconstrain", ["input.py"],1],
          ["RUN_init_by_ude", ["input.py", "*.csv", "*.trk"]],
          ["RUN_init_by_dyn_w_initialval", ["input.py", "*.csv", "*.trk"]],
          ["RUN_init_by_dyn_w_epoch", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_1_dyn_only", [
          ["RUN_dyn", ["input.py", "*.csv", "*.trk"]]]]]
  ],


  ["models/environment/planet", [
      ["verif/SIM_PLANET_VERIF", [
          ["RUN_initplanet_eellipsq", ["input.py", "*.csv", "*.trk"]],
          ["RUN_initplanet_rpol", ["input.py", "*.csv", "*.trk"]],
          ["RUN_initplanet_flatcoeff", ["input.py", "*.csv", "*.trk"]],
          ["RUN_initplanet_spherical", ["input.py", "*.csv", "*.trk"]],
          ["RUN_initplanet_invflat", ["input.py", "*.csv", "*.trk"]],
          ["RUN_initplanet_eellip", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/environment/earth_lighting", [
      ["verif/SIM_LIGHT_CIR", [
          ["RUN_T05_LIGHT_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T08_LIGHT_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T06_LIGHT_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T09_LIGHT_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T10_LIGHT_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T02_LIGHT_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T07_LIGHT_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T04_LIGHT_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T01_LIGHT_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T03_LIGHT_VER", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/ref_frames", [
      ["verif/SIM_REF_FRAMES", [
          ["RUN_ref_frame_ver2", ["input.py", "*.csv", "*.trk"]],
          ["RUN_ref_frame_ver3", ["input.py", "*.csv", "*.trk"]],
          ["RUN_ref_frame_ver1", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/planet_fixed/planet_fixed_posn", [
      ["verif/SIM_PFIXPOSN_VERIF", [
          ["RUN_pfixposn_test", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/planet_fixed/north_east_down", [
      ["verif/SIM_NED_VERIF", [
          ["RUN_nedtest", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/integration", [
      ["verif/SIM_integ_test", [
          ["RUN_lsode", ["input.py", "*.csv", "*.trk"]],
          ["RUN_gauss_jackson", ["input.py", "*.csv", "*.trk"]],
          ["RUN_rk2", ["input.py", "*.csv", "*.trk"]],
          ["RUN_abm4", ["input.py", "*.csv", "*.trk"]],
          ["RUN_symplectic_euler", ["input.py", "*.csv", "*.trk"]],
          ["RUN_beeman", ["input.py", "*.csv", "*.trk"]],
          ["RUN_euler", ["input.py", "*.csv", "*.trk"]],
          ["RUN_rk4", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_GJ_test", [
          ["RUN_GJ_step10_order8_noeval_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_RK4_step00", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step1_order16_noeval_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step1_order8_noeval_bs00", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step1_order4_noeval_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step0_order8_noeval_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_RK4_step1", ["input.py", "*.csv", "*.trk"]],
          ["RUN_RK4_step0", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step1_order8_noeval_bs0", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step1_order8_eval15_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step100_order8_noeval_bs1", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step1_order12_noeval_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step100_order8_eval15_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_RK4_step100", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step1_order8_noeval_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step100_order8_noeval_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step00_order8_noeval_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_GJ_step1_order2_noeval_nobs", ["input.py", "*.csv", "*.trk"]],
          ["RUN_RK4_step10", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/lvlh_frame", [
      ["verif/SIM_LVLH_Frame", [
          ["RUN_test", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/sim_interface", [
      ["verif/SIM_integ_loop", [
          ["RUN_switch", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_simulation_interface", [
          ["RUN_test", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/message", [
      ["verif/SIM_message_handler_verif", [
          ["RUN_test", ["input.py", "jeod_hs"],1]]]]
  ],

  ["models/utils/surface_model", [
      ["verif/SIM_SURFACE_MODEL", [
          ["RUN_surface_model_ver", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_ARTICULATION", [
          ["RUN_articulation_5", ["input.py", "*.csv", "*.trk"]],
          ["RUN_articulation_2", ["input.py", "*.csv", "*.trk"]],
          ["RUN_articulation_4", ["input.py", "*.csv", "*.trk"]],
          ["RUN_articulation_1", ["input.py", "*.csv", "*.trk"]],
          ["RUN_articulation_0", ["input.py", "*.csv", "*.trk"]],
          ["RUN_articulation_6", ["input.py", "*.csv", "*.trk"]],
          ["RUN_articulation_3", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/orbital_elements", [
      ["verif/SIM_orb_elem", [
          ["RUN_T06_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T56_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T16_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T40_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T09_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T57_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T19_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T35_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T55_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T50_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T34_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T42_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T14_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T11_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T24_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T27_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T54_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T08_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T31_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T04_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T53_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T13_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T48_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T36_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T46_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T07_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T05_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T17_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T23_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T25_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T49_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T45_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T18_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T37_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T22_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T51_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T28_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T39_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T52_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T29_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T38_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T33_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T02_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T01_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T26_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T20_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T44_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T03_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T30_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T21_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T12_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T41_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T10_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T47_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T32_OE_VER", ["input.py", "*.csv", "*.trk"]],
          ["RUN_T15_OE_VER", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/container", [
      ["verif/SIM_container", [
          ["RUN_full", ["input.py", "*.csv", "*.trk"]],
          ["RUN_empty", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/memory", [
      ["verif/SIM_memory", [
          ["RUN_checkpoint", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/math", [
      ["verif/SIM_math_verif", [
          ["RUN_test", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/interactions/aerodynamics", [
      ["verif/SIM_VER_DRAG", [
          ["RUN_aero_drag_BC", ["input.py", "*.csv", "*.trk"]],
          ["RUN_one_plate_accel_calc_coef_eps05", ["input.py", "*.csv", "*.trk"]],
          ["RUN_one_plate_accel_mixed_eps05_max_coef", ["input.py", "*.csv", "*.trk"]],
          ["RUN_one_plate_torque", ["input.py", "*.csv", "*.trk"]],
          ["RUN_one_plate_accel_diff_max_coef", ["input.py", "*.csv", "*.trk"]],
          ["RUN_aero_drag_const", ["input.py", "*.csv", "*.trk"]],
          ["RUN_aero_drag_CD", ["input.py", "*.csv", "*.trk"]],
          ["RUN_one_plate_accel_calc_coef_eps00", ["input.py", "*.csv", "*.trk"]],
          ["RUN_one_plate_accel_calc_coef_eps1", ["input.py", "*.csv", "*.trk"]],
          ["RUN_one_plate_accel_spec_max_coef", ["input.py", "*.csv", "*.trk"]],
          ["RUN_orbiter", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/interactions/gravity_torque", [
      ["verif/SIM_grav_torque_verif", [
          ["RUN_01", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_torque_compare_simple", [
          ["RUN_03", ["input.py", "*.csv", "*.trk"]],
          ["RUN_04", ["input.py", "*.csv", "*.trk"]],
          ["RUN_05", ["input.py", "*.csv", "*.trk"]],
          ["RUN_02", ["input.py", "*.csv", "*.trk"]],
          ["RUN_06", ["input.py", "*.csv", "*.trk"]],
          ["RUN_01", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/interactions/contact", [
      ["verif/SIM_contact", [
          ["RUN_line_point", ["input.py", "*.csv", "*.trk"]],
          ["RUN_line", ["input.py", "*.csv", "*.trk"]],
          ["RUN_line_inverse", ["input.py", "*.csv", "*.trk"]],
          ["RUN_line_arbitrary", ["input.py", "*.csv", "*.trk"]],
          ["RUN_point", ["input.py", "*.csv", "*.trk"]],
          ["RUN_line_side_to_side", ["input.py", "*.csv", "*.trk"]],
          ["RUN_point_off_center", ["input.py", "*.csv", "*.trk"]],
          ["RUN_point_inverse", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_ground_contact", [
          ["RUN_contact_ground", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/interactions/radiation_pressure", [
      ["verif/SIM_2A_SHADOW_CALC", [
          ["RUN_shadow_cooling", ["input.py", "*.csv", "*.trk"]],
          ["RUN_annular_eclipse", ["input.py", "*.csv", "*.trk"]],
          ["RUN_transverse_shadow", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_2_SHADOW_CALC", [
          ["RUN_ten_plates", ["input.py", "*.csv", "*.trk"]],
          ["RUN_shadow_cooling", ["input.py", "*.csv", "*.trk"]],
          ["RUN_annular_eclipse", ["input.py", "*.csv", "*.trk"]],
          ["RUN_transverse_shadow", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_3_ORBIT", [
          ["RUN_baseline", ["input.py", "*.csv", "*.trk"]],
          ["RUN_radiation", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_4_DEFAULT", [
          ["RUN_compare", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_1_BASIC", [
          ["RUN_basic", ["input.py", "*.csv", "*.trk"]],
          ["RUN_basic_cr", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_3_ORBIT_1st_ORDER", [
          ["RUN_baseline", ["input.py", "*.csv", "*.trk"]],
          ["RUN_radiation", ["input.py", "*.csv", "*.trk"]]]]]
  ]
]
