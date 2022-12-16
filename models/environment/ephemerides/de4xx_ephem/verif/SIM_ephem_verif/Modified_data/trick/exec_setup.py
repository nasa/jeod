#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (Setup the common Trick execution test run parameters.)
#
#REFERENCE:
#   (Trick User's Guide)
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((Usual Trick simulation assumptions and limitations.))
#
#
#*/

def set_trick_exec_setup() :

  #/* Simulation control panel. */
#FIXME:  sys.exec.sim_com.monitor_on = No

  #/* Don't really need the variable server for batch simulations. */
#FIXME:  sys.exec.in.enable_var_server = No

  #/* Checkpoint files. */
#FIXME:  sys.exec.in.pre_init_checkpoint = No
#FIXME:  sys.exec.in.post_init_checkpoint = No
#FIXME:  sys.exec.in.end_checkpoint       = No
#FIXME:  sys.exec.in.reduced_checkpoint   = Yes


  return
