def add_3vec(drg, var):
  for ii in range(3):
     drg.add_variable(var + "[" + str(ii) + "]")

drg = trick.DRAscii("drag")
drg.set_freq(trick.DR_Always)
drg.enable()
drg.set_cycle(1.0)

drg.add_variable("aero_test.logging.plates[0].force_n")
drg.add_variable("aero_test.logging.plates[1].force_n")
drg.add_variable("aero_test.logging.plates[0].force_t")
drg.add_variable("aero_test.logging.plates[1].force_t")
drg.add_variable("aero_test.logging.plates[0].angle_attack")
drg.add_variable("aero_test.logging.plates[1].angle_attack")
drg.add_variable("aero_test.logging.accel_mag")
drg.add_variable("aero_test.logging.accel_ratio")

add_3vec( drg, "aero_test.inertial_vel")
add_3vec( drg, "aero_test.logging.plates[0].force")
add_3vec( drg, "aero_test.logging.plates[0].torque")
add_3vec( drg, "aero_test.logging.plates[1].force")
add_3vec( drg, "aero_test.logging.plates[1].torque")
add_3vec( drg, "aero_test.logging.accel")
add_3vec( drg, "aero_test.aero_drag.aero_force")
add_3vec( drg, "aero_test.aero_drag.aero_torque")

# Add the data recording group to Trick's data recording.
trick.add_data_record_group(drg, trick.DR_Buffer)
