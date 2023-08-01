#PURPOSE:
#  (To illustrate the computation of child body translational accelerations)
trick.sim_services.exec_set_trap_sigfpe(1)

exec(compile(open( "Log_data/log_veh_state_rec.py", "rb").read(), "Log_data/log_veh_state_rec.py", 'exec'))

dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "central_point"
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

jeod_time.time_manager_init.initializer = "Dyn"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

exec(compile(open("Modified_data/veh1.py", "rb").read(), "Modified_data/veh1.py", 'exec'))
exec(compile(open("Modified_data/veh2.py", "rb").read(), "Modified_data/veh2.py", 'exec'))
exec(compile(open("Modified_data/veh3.py", "rb").read(), "Modified_data/veh3.py", 'exec'))

# turn on compute_point_derivative so that derivative are calculated even if the vehicle
# is a child body
veh1.dyn_body.compute_point_derivative = True
veh2.dyn_body.compute_point_derivative = True
veh3.dyn_body.compute_point_derivative = True

exec(compile(open("Modified_data/attach_detach.py", "rb").read(), "Modified_data/attach_detach.py", 'exec'))

#Attach 3 vehicles to one another
trick.add_read( 1, "veh1.attach_to_2.active = True")
trick.add_read( 2, "veh1.attach_to_3.active = True")

#Introduce force on vehicle 1
trick.add_read( 3, """
veh1.force.force   = [ 0.1, 0.0, 0.0]
""")
#Remove force on vehicle 1
trick.add_read( 5, """
veh1.force.force   = [ 0.0, 0.0, 0.0]
""")

#Introduce force on vehicle 2
trick.add_read( 7, """
veh2.force.force   = [ 0.0, 0.1, 0.0]
""")
#Remove force on vehicle 2
trick.add_read( 9, """
veh2.force.force   = [ 0.0, 0.0, 0.0]
""")

#Introduce force on vehicle 3
trick.add_read(11, """
veh3.force.force   = [ 0.0, 0.0, 0.1]
""")
#Remove force on vehicle 3
trick.add_read(13, """
veh3.force.force   = [ 0.0, 0.0, 0.0]
""")

#Separate vehicle 3
trick.add_read( 15    , "veh1.detach_from_3.active = True")

#Introduce force on vehicle 1
trick.add_read( 17, """
veh1.force.force   = [ -0.1, 0.0, 0.0]
""")
#Remove force on vehicle 1
trick.add_read( 19, """
veh1.force.force   = [ 0.0, 0.0, 0.0]
""")

#Introduce force on vehicle 2
trick.add_read( 21, """
veh2.force.force   = [ 0.0, -0.1, 0.0]
""")
#Remove force on vehicle 2
trick.add_read( 23, """
veh2.force.force   = [ 0.0, 0.0, 0.0]
""")

#Introduce force on vehicle 3
trick.add_read( 25, """
veh3.force.force   = [ 0.0, 0.0, -0.1]
""")
#Remove force on vehicle 3
trick.add_read( 27, """
veh3.force.force   = [ 0.0, 0.0, 0.0]
""")

#Introduce torque on vehicle 1
trick.add_read( 33, """
veh1.torque.torque   = [ 0.001, 0.0, 0.0]
""")
#Remove torque on vehicle 1
trick.add_read( 35, """
veh1.torque.torque   = [ 0.0, 0.0, 0.0]
""")

#Introduce torque on vehicle 2
trick.add_read( 37, """
veh2.torque.torque   = [ 0.0, 0.001, 0.0]
""")
#Remove torque on vehicle 2
trick.add_read( 39, """
veh2.torque.torque   = [ 0.0, 0.0, 0.0]
""")

#Introduce torque on vehicle 3
trick.add_read(41, """
veh3.torque.torque   = [ 0.0, 0.0, 0.001]
""")
#Remove torque on vehicle 3
trick.add_read(43, """
veh3.torque.torque   = [ 0.0, 0.0, 0.0]
""")

#Separate vehicle 3
trick.add_read( 45    , "veh1.detach_from_3.active = True")

#Introduce torque on vehicle 1
trick.add_read( 47, """
veh1.torque.torque   = [ -0.001, 0.0, 0.0]
""")
#Remove torque on vehicle 1
trick.add_read( 49, """
veh1.torque.torque   = [ 0.0, 0.0, 0.0]
""")

#Introduce torque on vehicle 2
trick.add_read( 51, """
veh2.torque.torque   = [ 0.0, -0.001, 0.0]
""")
#Remove torque on vehicle 2
trick.add_read( 53, """
veh2.torque.torque   = [ 0.0, 0.0, 0.0]
""")

#Introduce torque on vehicle 3
trick.add_read( 55, """
veh3.torque.torque   = [ 0.0, 0.0, -0.001]
""")
#Remove torque on vehicle 3
trick.add_read( 57, """
veh3.torque.torque   = [ 0.0, 0.0, 0.0]
""")

trick.stop(65)
