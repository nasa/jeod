exec(compile(open( "Modified_data/common_input.py", "rb").read(), "Modified_data/common_input.py", 'exec'))

# Set up parameters to record
log_rnp_verif(1.0)

# Configure base date
set_utc_time( 1991, 4, 6, 7, 51, 28.386009)

jeod_time.time_converter_tai_ut1.override_data_table = True
# old DUT: .402521    old leap seconds: 26
# tai to ut1 override value = DUT - leap seconds
jeod_time.time_converter_tai_ut1.tai_to_ut1_override_val = 0.402521 - 26.0

jeod_time.time_converter_tai_utc.override_data_table = True
jeod_time.time_converter_tai_utc.leap_sec_override_val = 26.0

earth.input_vector = [5102.5096, 6123.01152, 6378.1363]
