dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_Ephemerides
# dynamics.dyn_manager_init.central_point_name = "Earth"
abm_integrator = trick.ABM4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = abm_integrator
