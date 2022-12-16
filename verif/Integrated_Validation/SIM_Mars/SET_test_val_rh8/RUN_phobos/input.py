#   Model Setup -
#      Gravity Model: On
#      Mars: MRO 110B2: On
#      Planetary ephemeris = On
#      Third body perturbations = On
#      Atmosphere = Off
#      Radiation = Off
#      Gravity Gradient = Off
#
#   Simulation Duration - 24 hrs    
#
#   Data Collection Rate - 600 sec.


# Log data
exec(compile(open( "Log_data/log_data.py", "rb").read(), "Log_data/log_data.py", 'exec'))
log_ascii_rec( 600 , "phobos")
log_orb_rec(   600 , "phobos")

# Setup general parameters.
exec(compile(open( "Modified_data/common_settings.py", "rb").read(), "Modified_data/common_settings.py", 'exec'))

dawn.dyn_body.set_name("phobos")
set_grav_controls_8x8()
set_phobos_mass()
set_phobos_trans_state()
set_phobos_epoch()

#Set up simulation termination time (1 day). 
trick.stop(86400)
