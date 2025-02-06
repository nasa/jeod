################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_state_integrator 
####################################################################################

#Auto-generated from the data file with this header:
#// Set the integration options.

def set_state_integrator(dynamics) :
  dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
#FIXME:  dynamics.integ.first_step_deriv = Yes
