exec(compile(open( "input_common.py", "rb").read(), "input_common.py", 'exec'))
veh.grav_control.spherical = False

jeod_time.time_utc.calendar_year   = 2006
jeod_time.time_utc.calendar_month  =    7
jeod_time.time_utc.calendar_day    =    5
jeod_time.time_utc.calendar_hour   =    14
jeod_time.time_utc.calendar_minute =    40
jeod_time.time_utc.calendar_second =   0.0

use_trans_init()
veh.trans_init.position = [3512516.569374406, -3707194.208245289, -4214066.417378583]
veh.trans_init.velocity = [2480.605851006597, 6429.537710015741, -3518.207867419592]

use_lvlh_rot_init()

trick.stop(0)
