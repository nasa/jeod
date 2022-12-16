################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation
####################################################################################

#Auto-generated from the data file with this header:
#/*---------------------------------------------
# * Default data generated from S_define.
# *---------------------------------------------*/


trick.sim_services.exec_set_trap_sigfpe(1)

math_test.A[0]  = [  7.0,  8.0,  2.0]
math_test.A[1]  = [ -1.0,  5.0, -3.0]
math_test.A[2]  = [  6.0,  4.0, -7.0]

math_test.B[0]  = [  5.0, -3.0,  6.0]
math_test.B[1]  = [ -3.0,  7.0,  4.0]
math_test.B[2]  = [  6.0,  4.0, -8.0]

math_test.C[0]  = [  1.0,  2.0,  3.0]
math_test.C[1]  = [  4.0,  5.0,  6.0]
math_test.C[2]  = [  7.0,  8.0,  9.0]

math_test.f  = [  2.0, -3.0,  8.0]

math_test.g  = [  7.0,  2.0, -1.0]

math_test.s = 3.0

#/* simulation termination time */

trick.exec_set_terminate_time(0)
