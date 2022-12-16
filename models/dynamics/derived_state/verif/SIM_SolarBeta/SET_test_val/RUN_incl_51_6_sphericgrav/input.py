exec(compile(open( "input_common.py", "rb").read(), "input_common.py", 'exec'))

use_orb_init()
veh.init_orbit.inclination =  trick.attach_units( "degree", 51.6)
veh.init_orbit.ascending_node = 0.0
veh.init_orbit.true_anomaly   = 0.0

use_lvlh_rot_init()
