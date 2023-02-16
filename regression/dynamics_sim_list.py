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
