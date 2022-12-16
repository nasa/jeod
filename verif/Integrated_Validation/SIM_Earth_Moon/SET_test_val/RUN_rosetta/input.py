################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_rosetta/ 
####################################################################################

#Auto-generated from the data file with this header:
#/*****************************************************************************
#Integrated test for JPL rosetta orbit fit to JEOD generated orbit.
#
#Description:
#The purpose of this test is to verify that with gravity model, planetary
#ephemeris, third body perturbations and drag generate measurement criteria
#for the JEOD simulation for a LEO orbit.
#
#Aerodynamic and Atmosphere Parameters
#Coefficient of drag: 
#F10.7:               0.0
#F10.7B:              0.0
#Geomagnetic Index:   0.0
#Encounter distance about 12,000 km, no atmosphere.
#
#Run Scenario:
#NonSpherical Earth GGM02C 36x36
#Atmospheric model
#Aerodynamic drag 
#Third body perturbations
#Planetary ephemeris 
#Solid Body Tides
#
#Simulation duration
#4 hr
#Data collection rate
#60 sec.
#
#A.A.Jackson /ESCG/2010/
# G Turner Cleanup May 2017
#*****************************************************************************/

exec(compile(open("SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))
unique_input(60.0, "rosetta")

earth_grav(2,0)

trick.sim_services.exec_set_terminate_time(15000)
