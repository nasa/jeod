################################################################################
# PURPOSE:
#   (To define the input data for simulation run SET_test/RUN_switch_integ/)
################################################################################


# Import the JEOD checkpoint/restart module.
import sys
import os
sys.path.append ('/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]))
import jeod_checkpoint_restart

# Set up the simulation.
# The default setup uses the ephemeris model to set translational state.
LOG_CYCLE = 60*86400
exec(compile(open("Modified_data/setup.py", "rb").read(), "Modified_data/setup.py", 'exec'))
set_setup()

# Use Runge Kutta Fehlberg 7/8.
integ_constructor.switch_integ_to_RKF78()

# Run the sim for 400 days.
trick.stop(86400*400);

slow_integ_loop.integ_loop.set_integ_cycle (864000.00);

# Switch to propagated mode immediately after initialization.
trick.add_read(0 , """
sun.switch_to_propagated();
jupiter.switch_to_propagated();
saturn.switch_to_propagated();
""")

# Switch Saturn to the slow integration group a bit past one year into the run.
# Note that 371 days requires one more step at 864000.00 before switching to
# a 1728000.00 second step size.
trick.add_read(371*86400 , """
slow_integ_loop.integ_loop.set_integ_cycle (1728000.00);
slow_integ_loop.integ_loop.add_sim_object(saturn);
""")


# Drop a checkpoint at post-initialization time ...
jeod_sys.jeod_sim_interface.set_checkpoint_file_name ("")
trick.checkpoint_post_init(1)
# trick_mm.mmw.set_post_init_checkpoint(1)

# ... and at 380 days.
# Note that the end time in the checkpoint file is 150 years and 30 days.
jeod_checkpoint_restart.create_checkpoint (380*86400, 86400*(365*150+30))
