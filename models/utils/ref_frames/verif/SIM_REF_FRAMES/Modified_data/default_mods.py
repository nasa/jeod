#
# file purpose: Default input data for all RefFrame unit test verification
# test: all RefFrame validation tests
# tester: Andrew Spencer, Odyssey Space Research LLC
#

#include "Log_data/LIGHT_ver_rec.d"

exec(compile(open("Log_data/REF_FRAME_ver_rec.py", "rb").read(), "Log_data/REF_FRAME_ver_rec.py", 'exec'))

log_lighting_verif(0.1)

trick.sim_services.exec_set_trap_sigfpe(1)
