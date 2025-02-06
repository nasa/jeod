################################################################################
# PURPOSE:
#   (To define the input data for simulation run SET_test/RUN_ephem_checkpoint/)
################################################################################


# Set up the simulation.
# The default setup uses the ephemeris model to set translational state.
LOG_CYCLE = 60*86400
exec(compile(open("Modified_data/setup.py", "rb").read(), "Modified_data/setup.py", 'exec'))
set_setup()

# Use Runge Kutta Fehlberg 7/8.
integ_constructor.switch_integ_to_RKF78()

# Run the sim for 150 years and 30 days.
trick.stop(86400*(365*150+30))

# Switch to propagated mode immediately after initialization.
trick.add_read(0 , """
sun.switch_to_propagated();
jupiter.switch_to_propagated();
saturn.switch_to_propagated();
""")
