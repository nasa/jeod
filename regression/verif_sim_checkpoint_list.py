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

# NOTE:
# This list only contain verification sims that has checkpoint/restart runs.
# These runs have to be execute in correct order therefore use less
# cpu thread (e.g -c 1) when executing these runs.


verif_sim_list = [

  # checkpoint run need to be execute after the normal run is complete
  # To ensure this is true. Checkpoint runs were moved to a Separate list
  # and should be executed with less cpu thread
  ["models/environment/ephemerides", [
      ["verif/SIM_prop_planet", [
          ["RUN_ephem", ["input.py", "*.csv", "*.trk"]],
          ["RUN_switch_integ", ["input.py", "*.csv", "*.trk"]],
          ["RUN_prop", ["input.py", "*.csv", "*.trk"]],
          ["RUN_prop_gj", ["input.py", "*.csv", "*.trk"]],
          ["RUN_ephem_checkpoint", ["input.py", "*.csv", "*.trk"]],
          ["RUN_switch_integ_checkpoint", ["input.py", "*.csv", "*.trk"]],
          ["RUN_prop_checkpoint", ["input.py", "*.csv", "*.trk"]],
          ["RUN_ephem_restart", ["input.py", "*.csv", "*.trk"]],
          ["RUN_switch_integ_restart", ["input.py", "*.csv", "*.trk"]],
          ["RUN_prop_restart", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/container", [
      ["verif/SIM_container", [
          ["RUN_full", ["input.py", "*.csv", "*.trk"]],
          ["RUN_empty", ["input.py", "*.csv", "*.trk"]],
          ["RUN_empty_restart", ["input.py", "*.csv", "*.trk"]],
          ["RUN_full_restart", ["input.py", "*.csv", "*.trk"]]]]]
  ],

  ["models/utils/memory", [
      ["verif/SIM_memory", [
          ["RUN_checkpoint", ["input.py", "*.csv", "*.trk"]],
          ["RUN_restart", ["input.py", "*.csv", "*.trk"]]]]]
  ],
  
  ["verif", [
      ["SIM_dyncomp", [
          ["RUN_checkpoint_baseline", ["input.py", "*.csv", "*.trk"]],
          ["RUN_checkpoint_create", ["input.py", "*.csv", "*.trk"]],
          ["RUN_checkpoint_restart", ["input.py", "*.csv", "*.trk"]]]]]
  ]


]
