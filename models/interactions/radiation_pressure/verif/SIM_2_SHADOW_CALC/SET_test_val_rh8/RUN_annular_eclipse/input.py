################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_annular_eclipse/ 
####################################################################################

#Auto-generated from the data file with this header:
#/* Logging rate */


trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4;

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_radiation_rec.py", "rb").read(), "Log_data/log_radiation_rec.py", 'exec'))
log_radiation_rec( LOG_CYCLE )


#/******************************************************************************/
#/* Set up the time object:                                                    */
#/******************************************************************************/
exec(compile(open( "Modified_data/date_and_time.py", "rb").read(), "Modified_data/date_and_time.py", 'exec'))


#/******************************************************************************/
#/* Set up the vehicle object:                                                 */
#/******************************************************************************/

vehicle.dyn_body.set_name("vehicle")
vehicle.dyn_body.integ_frame_name = "Earth.inertial"
vehicle.dyn_body.composite_body.state.trans.position  = [ 0.0, 0.0, 0.0]
vehicle.dyn_body.structure.state.trans.position =  [0.3648654*1.3E9, 0.85422932*1.3E9, 0.37035885*1.3E9]



vehicle.dyn_body.structure.state.rot.T_parent_this[0]  = [ 1, 0, 0]
vehicle.dyn_body.structure.state.rot.T_parent_this[1]  = [ 0, 1, 0]
vehicle.dyn_body.structure.state.rot.T_parent_this[2]  = [ 0, 0, 1]



#/******************************************************************************/
#/* Set up the radiation object                                                */
#/******************************************************************************/
exec(compile(open( "Modified_data/radiation_surface.py", "rb").read(), "Modified_data/radiation_surface.py", 'exec'))
exec(compile(open( "Modified_data/third_bodies.py", "rb").read(), "Modified_data/third_bodies.py", 'exec'))

#//turn off moon
radiation.rad_pressure.set_third_body_inactive ("Moon")

trick.add_read(1,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.4E9, 0.85422932*1.4E9, 0.37035885*1.4E9]
""")

trick.add_read(2,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.45E9, 0.85422932*1.45E9, 0.37035885*1.45E9]
""")

trick.add_read(3,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.5E9, 0.85422932*1.5E9, 0.37035885*1.5E9]
""")

trick.add_read(4,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*2E9, 0.85422932*2E9, 0.37035885*2E9]
""")

trick.add_read(5,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*3E9, 0.85422932*3E9, 0.37035885*3E9]
""")

trick.add_read(6,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*5E9, 0.85422932*5E9, 0.37035885*5E9]
""")

trick.add_read(7,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*10E9, 0.85422932*10E9, 0.37035885*10E9]
""")

trick.add_read(8,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*100E9, 0.85422932*100E9, 0.37035885*100E9]
""")

trick.sim_services.exec_set_terminate_time(9)
