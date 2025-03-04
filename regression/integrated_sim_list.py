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

  ["sims", [
      ["SIM_Apollo", [
          ["RUN_test", ["input.py", "*.csv", "*.trk"]]]]]],

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
          ["RUN*", ""]]],
      ["SIM_10", [
          ["RUN*", ""]]],
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
          ["RUN*", ""]]]]]

]
