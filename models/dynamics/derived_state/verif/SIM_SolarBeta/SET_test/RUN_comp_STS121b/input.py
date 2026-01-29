exec(compile(open( "input_common.py", "rb").read(), "input_common.py", 'exec'))
veh.grav_control.spherical = False

jeod_time.time_utc.set_date_and_time(2006, 7, 5, 21, 30, 0.0)

use_trans_init()
veh.trans_init.position = [-2807126.738101881, -4882102.882196192, 3658506.094003358]
veh.trans_init.velocity = [3526.764777573051, -5301.838703156986, -4341.532531357011]

use_lvlh_rot_init()

trick.stop(0)
