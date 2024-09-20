################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_test/
####################################################################################

#/*****************************************************************************
# Run Test: Demonstartion run of Apollo vehicle element attahcments.
#******************************************************************************
#
#Description:
#
#Run Scenario:
#
#
#*****************************************************************************/

trick.sim_services.exec_set_trap_sigfpe(1)

#/*---------------------------------------------
# * Set up the simulation executive parameters.
# *---------------------------------------------*/





#/*---------------------------------------------
# * Set up the dynamics manager.
# *---------------------------------------------*/
#/* Control epheris update. */
dynamics.dyn_manager.deriv_ephem_update = True

#/* Setup general numerical integration parameters. */
exec(compile(open( "Modified_data/state/integrator.py", "rb").read(), "Modified_data/state/integrator.py", 'exec'))
set_state_integrator(dynamics)


#/*---------------------------------------------
# * Set up simulation date and time.
# *---------------------------------------------*/
exec(compile(open( "Modified_data/date_n_time/UTC_init.py", "rb").read(), "Modified_data/date_n_time/UTC_init.py", 'exec'))
set_date_n_time_UTC_init(jeod_time)
exec(compile(open( "Modified_data/date_n_time/UTC_16Jul1969.py", "rb").read(), "Modified_data/date_n_time/UTC_16Jul1969.py", 'exec'))
set_date_n_time_UTC_16Jul1969(jeod_time)
exec(compile(open( "Modified_data/date_n_time/MET_start_16Jul1969.py", "rb").read(), "Modified_data/date_n_time/MET_start_16Jul1969.py", 'exec'))
set_date_n_time_MET_start_16Jul1969(jeod_time)


#/*---------------------------------------------
# * Set up planetary bodies.
# *---------------------------------------------*/
#/*..............................................
# * Configure Earth planetary parameters.
# */
#include "Modified_data/Earth/params.d"
exec(compile(open( "Modified_data/Earth/params.py", "rb").read(), "Modified_data/Earth/params.py", 'exec'))
set_Earth_params(earth)

#/* Override polar motion values. */
earth.pm_init.override_table = True
earth.pm_init.xp = trick.attach_units("arcsecond", 0.141254)
earth.pm_init.yp = trick.attach_units("arcsecond", 0.236254)

#/* Configure atmosphere. */
exec(compile(open( "Modified_data/Earth/soflx_mean.py", "rb").read(), "Modified_data/Earth/soflx_mean.py", 'exec'))
set_Earth_soflx_mean(earth)
exec(compile(open( "Modified_data/Earth/uniform_wind.py", "rb").read(), "Modified_data/Earth/uniform_wind.py", 'exec'))
set_Earth_uniform_wind(earth)


#/*---------------------------------------------
# * Configure the vehicles.
# *---------------------------------------------*/

#/*..............................................
# * Define the Launch Escape System (LES)
# */

#/* Configure gravitational affects of the planets. */
exec(compile(open( "Modified_data/vehicle/grav_controls.py", "rb").read(), "Modified_data/vehicle/grav_controls.py", 'exec'))
set_vehicle_grav_controls(les_dyn)

#/* Default Earth orbit space vehicle configuration. */
exec(compile(open( "Modified_data/vehicle/sv_at_earth.py", "rb").read(), "Modified_data/vehicle/sv_at_earth.py", 'exec'))
les_dyn.dyn_body.set_name("les")
set_vehicle_sv_at_earth(les_dyn, earth)

#/* Launch Escape System mass configuration properties. */
exec(compile(open( "Modified_data/mass/les.py", "rb").read(), "Modified_data/mass/les.py", 'exec'))

set_mass_les(les_dyn)
exec(compile(open( "Modified_data/attach/les.py", "rb").read(), "Modified_data/attach/les.py", 'exec'))
set_les(les_dyn)

dynamics.dyn_manager.add_body_action(les_dyn.mass_init)


#/*..............................................
# * Define the Command Module (CM)
# */

#/* Default Earth orbit space vehicle configuration. */
cm_dyn.dyn_body.set_name("cm")
set_vehicle_sv_at_earth(cm_dyn, earth)

#/* Command Module mass configuration properties. */
exec(compile(open( "Modified_data/mass/command_module.py", "rb").read(), "Modified_data/mass/command_module.py", 'exec'))
set_mass_command_module(cm_dyn)
exec(compile(open( "Modified_data/attach/command_module.py", "rb").read(), "Modified_data/attach/command_module.py", 'exec'))
set_command_module(cm_dyn)

dynamics.dyn_manager.add_body_action(cm_dyn.mass_init)


#/*..............................................
# * Define the Service Module (SM)
# */

#/* Default Earth orbit space vehicle configuration. */
sm_dyn.dyn_body.set_name("sm")
set_vehicle_sv_at_earth(sm_dyn, earth)

#/* Service Module mass configuration properties. */
exec(compile(open( "Modified_data/mass/service_module.py", "rb").read(), "Modified_data/mass/service_module.py", 'exec'))
set_mass_service_module(sm_dyn)
exec(compile(open( "Modified_data/attach/service_module.py", "rb").read(), "Modified_data/attach/service_module.py", 'exec'))
set_service_module(sm_dyn)

dynamics.dyn_manager.add_body_action(sm_dyn.mass_init)


#/*..............................................
# * Define the Lunar Module (LM)
# */

#/* Default Earth orbit space vehicle configuration. */
lm_dyn.dyn_body.set_name("lm")
set_vehicle_sv_at_earth(lm_dyn, earth)

#/* Lunar Module mass configuration properties. */
exec(compile(open( "Modified_data/mass/ascent_module.py", "rb").read(), "Modified_data/mass/ascent_module.py", 'exec'))
set_mass_ascent_module(lm_dyn)
exec(compile(open( "Modified_data/attach/ascent_module.py", "rb").read(), "Modified_data/attach/ascent_module.py", 'exec'))
set_ascent_module(lm_dyn)

dynamics.dyn_manager.add_body_action(lm_dyn.mass_init)

#/* Apollo Lunar Module Descent Module mass configuration properties. */
dm_mass.body.set_name("dm")
exec(compile(open( "Modified_data/mass/descent_module.py", "rb").read(), "Modified_data/mass/descent_module.py", 'exec'))
set_mass_descent_module(dm_mass)
exec(compile(open( "Modified_data/attach/descent_module.py", "rb").read(), "Modified_data/attach/descent_module.py", 'exec'))
set_descent_module(dm_mass)

dynamics.dyn_manager.add_body_action(dm_mass.mass_init)



#/*..............................................
# * Define the Saturn V third stage (S3).
# */

#/* Default Earth orbit space vehicle configuration. */
s3_dyn.dyn_body.set_name("s3")
set_vehicle_sv_at_earth(s3_dyn, earth)

#/* Saturn V third stage mass configuration properties. */
exec(compile(open( "Modified_data/mass/stage_3.py", "rb").read(), "Modified_data/mass/stage_3.py", 'exec'))
set_mass_stage_3(s3_dyn)
exec(compile(open( "Modified_data/attach/stage_3.py", "rb").read(), "Modified_data/attach/stage_3.py", 'exec'))
set_stage_3(s3_dyn)

dynamics.dyn_manager.add_body_action(s3_dyn.mass_init)


#/*..............................................
# * Define the Saturn V second stage (S2).
# */

#/* Default Earth orbit space vehicle configuration. */
s2_dyn.dyn_body.set_name("s2")
set_vehicle_sv_at_earth(s2_dyn, earth)

#/* Saturn V second stage mass configuration properties. */
exec(compile(open( "Modified_data/mass/stage_2.py", "rb").read(), "Modified_data/mass/stage_2.py", 'exec'))
set_mass_stage_2(s2_dyn)
exec(compile(open( "Modified_data/attach/stage_2.py", "rb").read(), "Modified_data/attach/stage_2.py", 'exec'))
set_stage_2(s2_dyn)

dynamics.dyn_manager.add_body_action(s2_dyn.mass_init)


#/*..............................................
# * Define the Saturn V first stage (S1).
# */

#/* Default Earth orbit space vehicle configuration. */
s1_dyn.dyn_body.set_name("s1")
set_vehicle_sv_at_earth(s1_dyn, earth)

#/* Saturn V first stage mass configuration properties. */
exec(compile(open( "Modified_data/mass/stage_1.py", "rb").read(), "Modified_data/mass/stage_1.py", 'exec'))
set_mass_stage_1(s1_dyn)
exec(compile(open( "Modified_data/attach/stage_1.py", "rb").read(), "Modified_data/attach/stage_1.py", 'exec'))
set_stage_1(s1_dyn)

dynamics.dyn_manager.add_body_action(s1_dyn.mass_init)

#/*..............................................
# * Specify the initial stack configuration.
# */
exec(compile(open( "Modified_data/attach/launch_stack.py", "rb" ).read(), "Modified_data/attach/launch_stack.py", 'exec'))
set_launch_stack(dynamics, dm_mass, lm_dyn, sm_dyn, cm_dyn, s3_dyn, s2_dyn, s1_dyn, les_dyn)

#/*---------------------------------------------
# * Set the vehicle initial state.
# *---------------------------------------------*/
#/* Set up the state. */
exec(compile(open( "Modified_data/state/sv_leo_lvlh.py", "rb" ).read(), "Modified_data/state/sv_leo_lvlh.py", 'exec'))
set_state_sv_leo_lvlh(cm_dyn)
dynamics.dyn_manager.add_body_action(cm_dyn.trans_init)
dynamics.dyn_manager.add_body_action(cm_dyn.lvlh_init)


#/* Set mass print parameters in case of error in initialization. */
cm_dyn.dyn_body.mass.print_tree( "SET_test/RUN_test/Initialization.out", 100 )


#/***************************************/
#/* Print out the full stack mass tree. */
#/***************************************/
trick.add_read(0.0 , """
cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/Full_Stack.out", 100 )
""")


#/***************************/
#/* Detach the first stage. */
#/***************************/
trick.add_read(1.0 , """
s1_dyn.dyn_body.detach()

cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/1st_Stage_Sep.out", 100 )
""")

#/****************************/
#/* Detach the second stage. */
#/****************************/
trick.add_read(2.0 , """
s2_dyn.dyn_body.detach()

cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/2nd_Stage_Sep.out", 100 )
""")


#/*********************/
#/* Jettison the LES. */
#/*********************/
trick.add_read(3.0 , """
les_dyn.dyn_body.detach()

cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/LES_Jettison.out", 100 )
""")

#/***************************/
#/* Detach the third stage. */
#/***************************/
trick.add_read(4.0 , """
s3_dyn.dyn_body.detach()

cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/3rd_Stage_Sep.out", 100 )
""")


#/*******************/
#/* Detach the LEM. */
#/*******************/
trick.add_read(5.0 , """
lm_dyn.dyn_body.detach()

lm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/LEM_Sep.out", 100 )

cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/Apollo.out", 100 )
""")


#/*********************/
#/* Attach LEM to CM. */
#/*********************/
trick.add_read(6.0 , """
cm_dyn.dyn_body.attach_child("CM docking port", "LM docking port", lm_dyn.dyn_body)

cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/Trans_Lunar.out", 100 )
""")


#/*******************/
#/* Detach the LEM. */
#/*******************/
trick.add_read(7.0 , """
lm_dyn.dyn_body.detach()

lm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/LM_Descent.out", 100 )

cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/Lunar_Orbit.out", 100 )
""")



#/*********************************/
#/* Detach the LEM descent stage. */
#/*********************************/
trick.add_read(8.0 , """
dm_mass.body.detach()

lm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/LM_Ascent.out", 100 )
""")

read = 9.0
#/*********************/
#/* Attach LEM to CM. */
#/*********************/
trick.add_read(9.0 , """
lm_dyn.dyn_body.attach_to("LM docking port", "CM docking port", cm_dyn.dyn_body)

cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/Lunar_Rendezvous.out", 100 )
""")

read = 10.0
#/*********************************/
#/* Detach the LEM for last time. */
#/*********************************/
trick.add_read(10.0 , """
lm_dyn.dyn_body.detach()

cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/Return.out", 100 )
""")

read = 11.0
#/******************************/
#/* Detach the Service Module. */
#/******************************/
trick.add_read(11.0 , """
sm_dyn.dyn_body.detach()

cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/Entry.out", 100 )
cm_dyn.dyn_body.mass.print_tree ( "SET_test/RUN_test/Final.out", 100 )
""")

trick.sim_services.exec_set_terminate_time(12.0)
