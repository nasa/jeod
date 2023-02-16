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
