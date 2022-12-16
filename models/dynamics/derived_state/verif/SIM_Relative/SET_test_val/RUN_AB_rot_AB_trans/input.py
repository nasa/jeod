exec(compile(open( "input_common.py", "rb").read(), "input_common.py", 'exec'))
# Adjust nominal states:
vehA.trans_init.velocity   = [  2.0, 1.0, 0.0]
vehA.rot_init.ang_velocity = trick.attach_units( "degree/s",[ 0.0, 9.0, 0.0])

vehB.trans_init.velocity   = [ 0.0, 0.0, 1.0]
vehB.rot_init.ang_velocity = trick.attach_units( "degree/s",[ 0.0, 0.0, 4.5])
