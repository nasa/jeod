exec(compile(open( "input_common.py", "rb").read(), "input_common.py", 'exec'))
veh.grav_control.spherical = False

jeod_time.time_utc.calendar_year   = 2006
jeod_time.time_utc.calendar_month  =    7
jeod_time.time_utc.calendar_day    =    5
jeod_time.time_utc.calendar_hour   =    21
jeod_time.time_utc.calendar_minute =    30
jeod_time.time_utc.calendar_second =   0.0

use_trans_init()
veh.trans_init.position = [-2807126.738101881, -4882102.882196192, 3658506.094003358]
veh.trans_init.velocity = [3526.764777573051, -5301.838703156986, -4341.532531357011]

use_lvlh_rot_init()

trick.stop(0)
