################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_clem/ 
####################################################################################

#Auto-generated from the data file with this header:
#/*****************************************************************************
#Integrated test for Clementine Lunar+ orbit fit to JEOD generated orbit.
#
#Description:
#The purpose of this test is to verify that with gravity model, planetary
#ephemeris, third body perturbations and drag generate measurement criteria
#for the JEOD simulation for a LEO orbit.
#
#Aerodynamic and Atmosphere Parameters
#None
#
#Run Scenario:
#NonSpherical Moon 60x60
#Third body perturbations
#Planetary ephemeris 
#Solid Body Tides
#
#Simulation duration
#1 day duration
#Data collection rate
#30 min.
#
#A.A.Jackson /ESCG/April 2009/
# G Turner Cleanup May 2017
#*****************************************************************************/


exec(compile(open("SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))
unique_input(30.0*60.0, "clementine")

vehicle.moon_grav_control.spherical   = False
vehicle.moon_grav_control.degree = 60
vehicle.moon_grav_control.order  = 60


#/* Configure vehicle integration information. */
vehicle.dyn_body.integ_frame_name = "Moon.inertial"

#/* Configure the dervied reference frames. */
vehicle.pfix.reference_name     = "Moon"
vehicle.lvlh.reference_name     = "Moon"
vehicle.orb_elem.reference_name = "Moon"
