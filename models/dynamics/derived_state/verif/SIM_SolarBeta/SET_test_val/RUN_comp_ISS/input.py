exec(compile(open( "input_common.py", "rb").read(), "input_common.py", 'exec'))
veh.grav_control.spherical = False

jeod_time.time_utc.calendar_year   = 2005
jeod_time.time_utc.calendar_month  =    7
jeod_time.time_utc.calendar_day    =    26
jeod_time.time_utc.calendar_hour   =    14
jeod_time.time_utc.calendar_minute =    39
jeod_time.time_utc.calendar_second =   0.0

use_trans_init()
veh.trans_init.position = [-972896.02,    -6014900.38,    -2866287.24]
veh.trans_init.velocity = [   5585.497069,    1508.108071,   -5072.582182]

use_lvlh_rot_init()

trick.stop(0)
