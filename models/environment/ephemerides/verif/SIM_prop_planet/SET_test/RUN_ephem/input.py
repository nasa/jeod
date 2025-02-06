################################################################################
# PURPOSE:
#   (To define the input data for simulation run SET_test/RUN_ephem/)
################################################################################


# Set up the simulation.
# The default setup uses the ephemeris model to set translational state.
LOG_CYCLE = 60*86400
exec(compile(open("Modified_data/setup.py", "rb").read(), "Modified_data/setup.py", 'exec'))
set_setup()

# Run the sim for 150 years and 30 days.
trick.stop(86400*(365*150+30))
