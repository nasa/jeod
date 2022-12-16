sv_dyn.earth_spherical_grav_ctrl.source_name     = "Earth"
sv_dyn.earth_spherical_grav_ctrl.active          = True
sv_dyn.earth_spherical_grav_ctrl.spherical       = True
sv_dyn.earth_spherical_grav_ctrl.gradient        = True

sv_dyn.dyn_body.grav_interaction.add_control(sv_dyn.earth_spherical_grav_ctrl)

sv_dyn.earth_grav_ctrl.perturbing_only = True

exec(compile(open( "SET_test/RUN_01/input.py", "rb").read(), "SET_test/RUN_01/input.py", 'exec'))
