#=============================================================================
# Notices:
#
# Copyright © 2023 United States Government as represented by the Administrator
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
          ["RUN_comp*", ["input.py", "*.csv"]],
          ["RUN_FAIL_bad_reference_name", "input.py",1]]]]
  ],

  ["models/dynamics/dyn_body", [
      ["verif/SIM_verif_attach_detach", [
          ["RUN*", ["input.py", "*.csv", "*.trk"]]]],
      ["verif/SIM_verif_shutdown", [
          ["RUN*", ["input.py"]]]],
      ["verif/SIM_force_torque", [
          ["RUN_test", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/dynamics/rel_kin", [
      ["verif/SIM_RELKIN_VERIF", [
          ["RUN_relkin_test", ["input.py", "*.csv", "*.trk"]]]]]
  ]

]
