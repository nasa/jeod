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
exec(compile(open("Modified_data/two_sided_plate.py", "rb").read(), "Modified_data/two_sided_plate.py", 'exec'))
two_sided_plate( epsilon = 0.5)
