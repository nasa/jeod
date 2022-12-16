#UTC - TAI = -32.0 s
#UT1 - UTC = 0.650161 s
#
#(Polar Offset
#  xp = 0".06806
#  yp = 0".24156)

exec(compile(open( "Modified_data/common_input.py", "rb").read(), "Modified_data/common_input.py", 'exec'))

# Set up parameters to record
log_rnp_verif(1.0)
set_utc_time( 1999, 3, 4, 0, 0, 0.0)

# Disable polar motion
earth.rnp.enable_polar = False
earth.rnp.PMJ2000.override_table = False
