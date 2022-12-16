#/*************************** Copyright Statement ******************************
#* Rendezvous, Proximity Operations and Capture (RPOC) Model Suite             *
#*                                                                             *
#* This model suite has been developed under NASA Government Contracts and     *
#* access to it may be granted for Government work by the following contact:   *
#*                                                                             *
#* Contact: Allan L. DuPont, EG4                                               *
#*          Aeroscience and Flight Mechanics Division                          *
#*          NASA - Johnson Space Center, Houston, TX                           *
#*******************************************************************************/
exec(compile(open("Modified_data/input_common.py", "rb").read(), "Modified_data/input_common.py", 'exec'))
aero_test.aero_drag.use_default_behavior = True
aero_test.aero_drag.ballistic_drag.option = trick.DefaultAero.DRAG_OPT_CD
aero_test.aero_drag.ballistic_drag.area = 100
aero_test.aero_drag.ballistic_drag.Cd = 2
