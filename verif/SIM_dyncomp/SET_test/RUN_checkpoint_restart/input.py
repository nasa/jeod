# Configure the sim the same way
exec(compile(open( "SET_test/RUN_checkpoint_baseline/input.py", "rb").read(), "SET_test/RUN_checkpoint_baseline/input.py", 'exec'))

filename = "chkpnt_"+str(checkpoint_time)+"00000"

jeod_checkpoint_restart.restore_from_checkpoint('SET_test/RUN_checkpoint_create', filename)
