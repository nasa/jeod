#=============================================================================
# Notices:
#
# Copyright 2025 United States Government as represented by the Administrator
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
  ]

]
