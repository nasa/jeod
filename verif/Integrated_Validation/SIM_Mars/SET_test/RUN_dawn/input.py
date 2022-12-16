#   Model Setup -
#      Gravity Model: On
#      Mars: MRO110B2: On
#      Planetary ephemeris = On
#      Third body perturbations = On
#      Atmosphere = Off
#      Radiation = Off
#      Gravity Gradient = Off
#
#   Simulation Duration - 3 hrs    
#
#   Data Collection Rate - 60 sec.


# Log data
exec(compile(open( "Log_data/log_data.py", "rb").read(), "Log_data/log_data.py", 'exec'))
log_ascii_rec( 60 , "dawn")
log_orb_rec(   60 , "dawn")

# Setup general parameters.
exec(compile(open( "Modified_data/common_settings.py", "rb").read(), "Modified_data/common_settings.py", 'exec'))

dawn.dyn_body.set_name("dawn")
set_dawn_mass()
set_dawn_trans_state()
set_dawn_epoch()

#Set up simulation termination time (3 hours). 
trick.stop(10800)
