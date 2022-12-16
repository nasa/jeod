################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_transverse_shadow/ 
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

#############  ASSIGNMENTS, CALLS, and CONTROL  #################
#define LOG_CYCLE 1.0 

#/* Default data generated from S_define. */
#include "S_default.dat"

#/* Parameters to data record. */
#include "Log_data/radiation_rec.d"

#FIXME:  sys.exec.in.trap_sigfpe  = Yes
#FIXME:  sys.exec.in.trap_sigsegv = Yes
#FIXME:  sys.exec.in.pre_init_checkpoint  = No
#FIXME:  sys.exec.in.post_init_checkpoint = No
#FIXME:  sys.exec.in.end_checkpoint       = No
#FIXME:  sys.exec.in.reduced_checkpoint   = No
#FIXME:  sys.exec.sim_com.monitor_on      = No
#FIXME:  sys.exec.rt_log.group[0].record  = No


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
vehicle.dyn_body.structure.state.trans.position =  [0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*1.4E7, 0.3703588*1.35E9-0.84173*1.4E7]



vehicle.dyn_body.structure.state.rot.T_parent_this[0]  = [ 1, 0, 0]
vehicle.dyn_body.structure.state.rot.T_parent_this[1]  = [ 0, 1, 0]
vehicle.dyn_body.structure.state.rot.T_parent_this[2]  = [ 0, 0, 1]



#/******************************************************************************/
#/* Set up the radiation object                                                */
#/******************************************************************************/
exec(compile(open( "Modified_data/radiation_surface.py", "rb").read(), "Modified_data/radiation_surface.py", 'exec'))
exec(compile(open( "Modified_data/third_bodies.py", "rb").read(), "Modified_data/third_bodies.py", 'exec'))

trick.add_read(1,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*1.3E7 , 0.3703588*1.35E9-0.84173*1.3E7 ]
""")

trick.add_read(2,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*1.2E7 , 0.3703588*1.35E9-0.84173*1.2E7 ]
""")

trick.add_read(3,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*1.1E7 , 0.3703588*1.35E9-0.84173*1.1E7]
""")

trick.add_read(4,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*1.0E7 , 0.3703588*1.35E9-0.84173*1.0E7 ]
""")

trick.add_read(5,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*9E6 , 0.3703588*1.35E9-0.84173*9E6 ]
""")

trick.add_read(6,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*8E6 , 0.3703588*1.35E9-0.84173*8E6 ]
""")

trick.add_read(7,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*7E6 , 0.3703588*1.35E9-0.84173*7E6 ]
""")

trick.add_read(8,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*6E6 , 0.3703588*1.35E9-0.84173*6E6 ]
""")

trick.add_read(9,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*5E6 , 0.3703588*1.35E9-0.84173*5E6 ]
""")

trick.add_read(10,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*4E6 , 0.3703588*1.35E9-0.84173*4E6 ]
""")

trick.add_read(11,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*3E6 , 0.3703588*1.35E9-0.84173*3E6 ]
""")

trick.add_read(12,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*2E6 , 0.3703588*1.35E9-0.84173*2E6 ]
""")

trick.add_read(13,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9+0.36499*1E6 , 0.3703588*1.35E9-0.84173*1E6 ]
""")

trick.add_read(14,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9, 0.3703588*1.35E9]
""")

trick.add_read(15,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*1E6 , 0.3703588*1.35E9+0.84173*1E6 ]
""")

trick.add_read(16,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*2E6 , 0.3703588*1.35E9+0.84173*2E6 ]
""")

trick.add_read(17,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*3E6 , 0.3703588*1.35E9+0.84173*3E6 ]
""")

trick.add_read(18,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*4E6 , 0.3703588*1.35E9+0.84173*4E6 ]
""")

trick.add_read(19,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*5E6 , 0.3703588*1.35E9+0.84173*5E6 ]
""")

trick.add_read(20,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*6E6 , 0.3703588*1.35E9+0.84173*6E6 ]
""")

trick.add_read(21,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*7E6 , 0.3703588*1.35E9+0.84173*7E6 ]
""")

trick.add_read(22,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*8E6 , 0.3703588*1.35E9+0.84173*8E6 ]
""")

trick.add_read(23,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*9E6 , 0.3703588*1.35E9+0.84173*9E6 ]
""")

trick.add_read(24,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*10E6 , 0.3703588*1.35E9+0.84173*10E6 ]
""")

trick.add_read(25,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*11E6 , 0.3703588*1.35E9+0.84173*11E6 ]
""")

trick.add_read(26,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*12E6 , 0.3703588*1.35E9+0.84173*12E6 ]
""")

trick.add_read(27,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*13E6, 0.3703588*1.35E9+0.84173*13E6 ]
""")

trick.add_read(28,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*14E6, 0.3703588*1.35E9+0.84173*14E6 ]
""")

trick.add_read(29,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*50E6, 0.3703588*1.35E9+0.84173*50E6 ]
""")

trick.add_read(30,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*1E8 , 0.3703588*1.35E9+0.84173*1E8 ]
""")

trick.add_read(31,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*1E9 , 0.3703588*1.35E9+0.84173*1E9 ]
""")

trick.add_read(32,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*1E10 , 0.3703588*1.35E9+0.84173*1E10 ]
""")

trick.add_read(33,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*1E11 , 0.3703588*1.35E9+0.84173*1E11 ]
""")

trick.add_read(34,  """
vehicle.dyn_body.structure.state.trans.position= [ 0.3648654*1.35E9, 0.85422932*1.35E9-0.36499*1E12 , 0.3703588*1.35E9+0.84173*1E12 ]
""")

trick.sim_services.exec_set_terminate_time(35)
