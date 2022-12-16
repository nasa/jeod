def add_3vec( drg, var):
  for ii in range(3):
    drg.add_variable( var + "[" + str(ii) + "]")

def add_plate_var( drg, var):
  for ii in range(1,9):
    add_3vec( drg, "articulation.flat_plate_" + str(ii) + "." + var)

drg = trick.DRBinary("articulation_test")
drg.set_freq(trick.DR_Always)
drg.enable()
drg.set_cycle(1.0)

add_plate_var(drg, "normal")
add_plate_var(drg, "local_normal")
add_plate_var(drg, "position")
add_plate_var(drg, "local_position")

# Add the data recording group to Trick's data recording.
trick.add_data_record_group(drg, trick.DR_Buffer)
