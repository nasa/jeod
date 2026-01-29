# This file defines the common input aspects shared between all runs in the
# SIM_dyncomp suite.

trick.sim_services.exec_set_trap_sigfpe(1)

# Setup integration parameters.
exec(compile(open( "Modified_data/integration.py", "rb").read(), "Modified_data/integration.py", 'exec'))

# * Set up simulation date and time.
exec(compile(open( "Modified_data/time.py", "rb").read(), "Modified_data/time.py", 'exec'))

# Configure gravitational affects of the planets.
exec(compile(open( "Modified_data/grav_controls.py", "rb").read(), "Modified_data/grav_controls.py", 'exec'))

# Turn off polar motion.
earth.rnp.enable_polar = False 

# Configure atmosphere.
exec(compile(open( "Modified_data/uniform_wind.py", "rb").read(), "Modified_data/uniform_wind.py", 'exec'))
exec(compile(open( "Modified_data/solar_flux.py", "rb").read(), "Modified_data/solar_flux.py", 'exec'))


# Set the vehicle name.
vehicle.dyn_body.set_name("iss")

# Configuration vehicle mass properties.
exec(compile(open( "Modified_data/mass.py", "rb").read(), "Modified_data/mass.py", 'exec'))

# Configuration vehicle initial state.
# Must specify how to initailize rotational state.
exec(compile(open( "Modified_data/state.py", "rb").read(), "Modified_data/state.py", 'exec'))

# Configure aerodynamics.
exec(compile(open( "Modified_data/aero_drag.py", "rb").read(), "Modified_data/aero_drag.py", 'exec'))

# The following models default to active:
# grav_torque
# atmos
# aero_drag
# Turn off grav-torque and aero-drag for most cases
vehicle.grav_torque.active = False
vehicle.aero_drag.active = False


trick.stop(28800)
