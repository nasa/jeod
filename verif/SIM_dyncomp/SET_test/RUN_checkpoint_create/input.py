exec(compile(open( "SET_test/RUN_checkpoint_baseline/input.py", "rb").read(), "SET_test/RUN_checkpoint_baseline/input.py", 'exec'))

#Set up simulation termination time when comparison data has been collected.
trick.stop( checkpoint_time + comparison_window)

# Drop a checkpoint. NOTE - this line must go after the stop setting
jeod_checkpoint_restart.create_checkpoint (checkpoint_time,
                                           stop_time)
