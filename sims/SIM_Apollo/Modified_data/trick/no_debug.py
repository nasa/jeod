################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_trick_no_debug 
####################################################################################

#Auto-generated from the data file with this header:
#/* ========================================================================= -
# -                                                                           -
# - Name : no_debug.d                                                         -
# -                                                                           -
# - Description:  Trick debugging parameters                                  -
# -                                                                           -
# - Reference:   Trick User's Guide                                           -
# -                                                                           -
# - Created by:  Patrick McCartney/NASA/Johnson Space Center  7/8/97          -
# - Updated by:  Edwin Z. Crues / NASA / Johnson Space Center March 2006      -
# -                                                                           -
# - ========================================================================= -
#*/

def set_trick_no_debug() :


  #/* ECHO FLAGS */
#FIXME:  sys.exec.ip.echo_input = No
#FIXME:  sys.exec.in.echo_job   = No


  #/* Trick executive initialization job phase stepping. */
#FIXME:  sys.exec.in.enable_init_stepping = No
  #//sys.exec.sim_com.monitor_on      = 1


  #/* CHECKPOINTS */
#FIXME:  sys.exec.in.pre_init_checkpoint  = No
#FIXME:  sys.exec.in.post_init_checkpoint = No
#FIXME:  sys.exec.in.end_checkpoint       = No
#FIXME:  sys.exec.in.reduced_checkpoint   = Yes
#FIXME:  sys.exec.in.hexflt_checkpoint    = No


  #/* SUMMARIES */
#FIXME:  sys.exec.in.enable_cpu_stats     = No


  #/* SIGNAL TRAP FLAGS */
#FIXME:  sys.exec.in.trap_sigbus   = Yes
#FIXME:  sys.exec.in.trap_sigfpe   = No
#FIXME:  sys.exec.in.trap_sigsegv  = Yes


  #/* Debugging environment options. */
#FIXME:  sys.exec.in.attach_debugger = No
#FIXME:  sys.exec.in.stack_trace     = No


  #/* Trick executive variable server activation.                 */
  #/* Except for high performance sims this should always be Yes. */
#FIXME:  sys.exec.in.enable_var_server   = Yes


  #/* Time reference options. */
  #/*
  #sys.exec.in.gmt_ref
  #sys.exec.in.met_ref
  #*/

  #/* Trick executive control options. */
  #/*
  #sys.exec.in.initialization_passes
  #sys.exec.in.shutdown_wait_async
  #sys.exec.in.time_tic
  #*/

  #/* Freeze control flags. */
  #/*
  #sys.exec.in.enable_freeze
  #sys.exec.in.freeze_cycle
  #sys.exec.work.freeze_command
  #*/


  return