exec(compile(open("Log_data/LIGHT_verif_rec.py", "rb").read(), "Log_data/LIGHT_verif_rec.py", 'exec'))
trick.sim_services.exec_set_trap_sigfpe(1)

mngr.dyn_manager_init.central_point_name = "Earth"

light.r_bottom = 0.0
light.r_top = 0.0
light.d_centers = 0.0

mngr.sun.set_name("Sun")
mngr.earth.set_name("Earth")
mngr.moon.set_name("Moon")
mngr.sun_grav.name = "Sun"
mngr.earth_grav.name = "Earth"
mngr.moon_grav.name = "Moon"

mngr.sun.inertial.state.trans.position =  [ 1,  0.75, 0.0 ]
mngr.moon.inertial.state.trans.position = [ 1, -0.75, 0.0 ]

mngr.sun.r_eq   = 1.0
mngr.moon.r_eq  = 1.0
mngr.earth.r_eq = 1.0

light.position = [-2.0, 0.0, 0.0]

trick.stop(5.0)
