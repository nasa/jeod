# UTC - TAI = -32.0 s
# TT  - UTC = 64.184 s
# UT1 - UTC = 0.649232 s
#
# (Polar Offset
#   xp = 0".06740
#   yp = 0".24173)
#
# A.Jackson / ESCG / 2005 /

exec(compile(open( "Modified_data/common_input.py", "rb").read(), "Modified_data/common_input.py", 'exec'))

# Set up parameters to record
log_rnp_verif(1.0)

set_utc_time( 1999, 3, 4, 0, 0, 0.0)

jeod_time.time_converter_tai_ut1.override_data_table = True
# tai to ut1 is (UT1 - UTC) - (Leap Seconds)
jeod_time.time_converter_tai_ut1.tai_to_ut1_override_val = 0.64932 - 32.0

jeod_time.time_converter_tai_utc.override_data_table = True
jeod_time.time_converter_tai_utc.leap_sec_override_val = 32.0
