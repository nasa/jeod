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

# Enable polar motion
earth.rnp.enable_polar = True
earth.rnp.PMJ2000.override_table = False
