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


#  ["models/environment/spice", [
#      ["verif/SIM_spice", [
#          ["RUN_01", ["input.py", "*.csv", "*.trk"]]]],
#      ["verif/SIM_de4xx", [
#          ["RUN_01", ["input.py", "*.csv", "*.trk"]]]]]
#  ],

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
  ]

]
