################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_line/
####################################################################################

#Auto-generated from the data file with this header:
#/*****************************************************************************
#RUN_1
#Description: This run simulate the contact and tracking relative states between
#two vehicles.
#
#Run Scenario: contact surfaces start 10m apart and close at 2 m/s
#
#
#*****************************************************************************/


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 0.05

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_contact_data.py", "rb").read(), "Log_data/log_contact_data.py", 'exec'))
log_contact_data( LOG_CYCLE )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################

#/*---------------------------------------------
# * Set the globally defined names.
# *---------------------------------------------*/
#define dynamics.dyn_manager dynamics.dyn_manager
#define LOG_CYCLE     0.05

#/*---------------------------------------------
# * Default data generated from S_define.
# *---------------------------------------------*/
#include "S_default.dat"

#/*---------------------------------------------
# * Set up the simulation executive parameters.
# *---------------------------------------------*/





#/*---------------------------------------------
# * Set up general parameters to record.
# *---------------------------------------------*/
#include "Log_data/log_contact_data.d"

#/*---------------------------------------------
# * Set up the dynamics manager.
# *---------------------------------------------*/
#/* Configure the dynamics manager to operate in empty space mode */
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
dynamics.dyn_manager_init.central_point_name = "Earth"

#/* Setup general numerical integration parameters. */
exec(compile(open( "Contact_Modified_data/state/integrator.py", "rb").read(), "Contact_Modified_data/state/integrator.py", 'exec'))
set_state_integrator(dynamics)


#/*---------------------------------------------
# * Set up simulation date and time.
# *---------------------------------------------*/
#/* Set up time structure */
exec(compile(open( "Contact_Modified_data/date_n_time/epoch.py", "rb").read(), "Contact_Modified_data/date_n_time/epoch.py", 'exec'))
set_date_n_time_epoch(jeod_time)

#/*---------------------------------------------
# * Set up a pair interaction
# *---------------------------------------------*/
exec(compile(open( "Contact_Modified_data/contact/pair_interaction.py", "rb").read(), "Contact_Modified_data/contact/pair_interaction.py", 'exec'))
set_contact_interaction(contact)
exec(compile(open( "Modified_data/ground/pair_interaction.py", "rb").read(), "Modified_data/ground/pair_interaction.py", 'exec'))
set_contact_interaction(contact)

#/*---------------------------------------------
# * Configure the planet/terrain/ground.
# *---------------------------------------------*/
exec(compile(open( "Modified_data/ground/ground_facet.py", "rb").read(), "Modified_data/ground/ground_facet.py", 'exec'))
set_ground_facet(contact)

#/*---------------------------------------------
# * Configure the vehicles.
# *---------------------------------------------*/

#/* Default Free space vehicle configuration. */
exec(compile(open( "Modified_data/vehicle/sv_earth.py", "rb").read(), "Modified_data/vehicle/sv_earth.py", 'exec'))
set_vehicle_earth(veh1_dyn, "veh1")

#/* Veh1 mass configuration properties. */
exec(compile(open( "Contact_Modified_data/mass/veh_mass_line.py", "rb").read(), "Contact_Modified_data/mass/veh_mass_line.py", 'exec'))
set_mass_veh_mass(veh1_dyn)

#CALL
dynamics.dyn_manager.add_body_action(veh1_dyn.mass_init)


#/*Set up contact params.*/
#define veh1_params veh1_params
exec(compile(open( "Contact_Modified_data/contact/contact_params.py", "rb").read(), "Contact_Modified_data/contact/contact_params.py", 'exec'))
set_contact_params(veh1_dyn)

#/* Veh1 contact surface. */
#define veh1_surface veh1_surface
exec(compile(open( "Contact_Modified_data/contact/line_facet.py", "rb").read(), "Contact_Modified_data/contact/line_facet.py", 'exec'))
set_contact_line_facet(veh1_dyn, "veh1")


#/*..............................................
# * Define the vehicle 2.
# */

#/* Default Free space vehicle configuration. */
exec(compile(open( "Modified_data/vehicle/sv_earth.py", "rb").read(), "Modified_data/vehicle/sv_earth.py", 'exec'))
set_vehicle_earth(veh2_dyn, "veh2")

#/* Veh1 mass configuration properties. */
exec(compile(open( "Contact_Modified_data/mass/veh_mass_line.py", "rb").read(), "Contact_Modified_data/mass/veh_mass_line.py", 'exec'))
set_mass_veh_mass(veh2_dyn)

#CALL
dynamics.dyn_manager.add_body_action(veh2_dyn.mass_init)


#/*Set up params and interactions.*/
#define veh2_params veh2_params
exec(compile(open( "Contact_Modified_data/contact/contact_params.py", "rb").read(), "Contact_Modified_data/contact/contact_params.py", 'exec'))
set_contact_params(veh2_dyn)

#/* Veh2 contact surface. */
#define veh2_surface veh2_surface
exec(compile(open( "Contact_Modified_data/contact/point_facet.py", "rb").read(), "Contact_Modified_data/contact/point_facet.py", 'exec'))
set_contact_point_facet(veh2_dyn, "veh2")

#/*---------------------------------------------
# * Set the veh1_dyn initial state.
# *---------------------------------------------*/
veh1_dyn.trans_init.set_subject_body( veh1_dyn.body )
veh1_dyn.trans_init.reference_ref_frame_name = "Earth.inertial"
veh1_dyn.trans_init.body_frame_id            = "composite_body"
veh1_dyn.trans_init.position  = trick.attach_units( "m",[ 6378137.0,   0.0,   0.0])
veh1_dyn.trans_init.velocity  = trick.attach_units( "m/s",[ 0.0,   0.0,   0.0])

veh1_dyn.rot_init.set_subject_body( veh1_dyn.body )
veh1_dyn.rot_init.reference_ref_frame_name        = "Earth.inertial"
veh1_dyn.rot_init.body_frame_id                   = "composite_body"
veh1_dyn.rot_init.orientation.data_source         = trick.Orientation.InputEulerRotation
veh1_dyn.rot_init.orientation.euler_sequence      = trick.Orientation.Yaw_Pitch_Roll
veh1_dyn.rot_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0,   0.0,   0.0])
veh1_dyn.rot_init.ang_velocity            = trick.attach_units( "rad/s",[ 0.0,   0.0,   0.0])

#/* Set up state initialization. */
dynamics.dyn_manager.add_body_action(veh1_dyn.trans_init)
dynamics.dyn_manager.add_body_action(veh1_dyn.rot_init)


#/*---------------------------------------------
# * Set the veh2_dyn initial state.
# *---------------------------------------------*/
veh2_dyn.trans_init.set_subject_body( veh2_dyn.body )
veh2_dyn.trans_init.reference_ref_frame_name = "Earth.inertial"
veh2_dyn.trans_init.body_frame_id            = "composite_body"
veh2_dyn.trans_init.position  = trick.attach_units( "m",[ 6378147.0,   0.0,   0.0])
veh2_dyn.trans_init.velocity  = trick.attach_units( "m/s",[ 0.0,   0.0,   0.0])

veh2_dyn.rot_init.set_subject_body( veh2_dyn.body )
veh2_dyn.rot_init.reference_ref_frame_name        = "Earth.inertial"
veh2_dyn.rot_init.body_frame_id                   = "composite_body"
veh2_dyn.rot_init.orientation.data_source         = trick.Orientation.InputEulerRotation
veh2_dyn.rot_init.orientation.euler_sequence      = trick.Orientation.Yaw_Pitch_Roll
veh2_dyn.rot_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0,   0.0,   0.0])
veh2_dyn.rot_init.ang_velocity            = trick.attach_units( "rad/s",[ 0.0,   0.0,   0.0])

#/* Set up state initialization. */
dynamics.dyn_manager.add_body_action(veh2_dyn.trans_init)
dynamics.dyn_manager.add_body_action(veh2_dyn.rot_init)


trick.sim_services.exec_set_terminate_time(10)
