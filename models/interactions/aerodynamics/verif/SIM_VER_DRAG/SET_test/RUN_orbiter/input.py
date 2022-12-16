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

# Configure the plate model
exec(compile(open("Modified_data/shuttle_plate_orbiter.py", "rb").read(), "Modified_data/shuttle_plate_orbiter.py", 'exec'))
six_plate_orbiter( calc_coeff = trick.AeroDragEnum.Calc_coef)

# aero_test.center_grav[0] {in} = 1098.0, 0.0, 372.0
# center of grav is now no longer a trick headered value.
# so we need to do the (perfect, by definition) conversion here
aero_test.center_grav  =[ 1098.0 * .0254, 0.0, 372.0 * .0254]
aero_test.mass = 91589.71
