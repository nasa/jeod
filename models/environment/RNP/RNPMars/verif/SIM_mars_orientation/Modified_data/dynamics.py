################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_Integrator_integrator 
####################################################################################

#Auto-generated from the data file with this header:
#// Set the integration options.

def set_Integrator_integrator(dynamics) :
  dynamics.manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4



  return