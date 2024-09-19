/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_pressure.cc
 * Calculation of force and torque due to radiation pressure
 */

/******************************* TRICK HEADER *********************************
PURPOSE:
    ()
REFERENCE:
    (((None)))
ASSUMPTIONS AND LIMITATIONS:
    ((Orbital body modeled as a collection of facets)
     (Includes an integration of temperature; assumes that the next time step is
     equal to the previous time step.  If this is not true, it will be corrected
     at the next calculation.))
CLASS:
    (scheduled)
LIBRARY DEPENDENCY:
    ((radiation_pressure.cc)
     (radiation_pressure__surface_model.cc)
     (radiation_pressure__default_surface.cc)
     (radiation_source.cc)
     (radiation_third_body.cc))



******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "interactions/thermal_rider/include/thermal_facet_rider.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model structure includes
#include "../include/radiation_messages.hh"
#include "../include/radiation_pressure.hh"
#include "../include/radiation_source.hh"
#include "../include/radiation_third_body.hh"

//! Namespace jeod
namespace jeod
{

/******************************************************************************/
/******************************************************************************/

/**
 * Constructor for RadiationPressure
 */
/******************************************************************************/

/******************************************************************************
  Purpose:
    (Constructor for RadiationPressure)
******************************************************************************/
RadiationPressure::RadiationPressure()
{
    JEOD_REGISTER_CLASS(RadiationPressure);
    JEOD_REGISTER_CLASS(RadiationThirdBody);
    JEOD_REGISTER_CHECKPOINTABLE(this, third_bodies);
}

/**
 * Initializes the source and third bodies.
 * \param[in] dyn_mgr_ptr The dynamics manager
 */
void RadiationPressure::initialize_environment(DynManager * dyn_mgr_ptr)
{
    source.initialize(dyn_mgr_ptr);
    for(unsigned int ii_body = 0; ii_body < num_third_bodies; ++ii_body)
    {
        third_bodies[ii_body]->primary_source_ptr = &(source);
        third_bodies[ii_body]->initialize(dyn_mgr_ptr);
    }
    initialized = true;
}

/**
 * Updates the model at each time step.
 * \param[in] veh_struc_frame The vehicle structural reference frame.
 * \param[in] center_grav position of center of mass\n Units: M
 * \param[in] scale_factor the scale between the simulator time and the dynamic time.
 * \param[in] real_time The current time, e.g. TAI or UT1.\n Units: s
 */
void RadiationPressure::update(RefFrame & veh_struc_frame, double center_grav[3], double scale_factor, double real_time)
{
    if(!active)
    {
        return;
    }

    //  ThermalFacetRider cycle-time is the time needed for integration purposes,
    //  and includes "catchup" whenever the rate at which time advances changes.
    ThermalFacetRider::cycle_time = JeodSimulationInterface::get_job_cycle() * scale_factor;

    source.calculate_flux(veh_struc_frame, center_grav);

    if(third_bodies_active)
    {
        third_body_adjustments(real_time, veh_struc_frame);
    }

    // It appears that this would be better handled by polymorphism - that a call
    // to, say, update_surface, would branch to the appropriate method, which
    // should be in RadiationSurface, not RadiationPressure.  However, the
    // methods called here are more managerial that operational.  The operational
    // methods are in the appropriate Surface class, and are called by these
    // managerial methods.  They do belong in RadiationPressure.
    if(surface_ptr == nullptr)
    {
        update_default_surface();
    }
    else
    {
        update_facet_surface();
    }
}

/**
 * Used to modify the flux for shadowing.
 * \param[in] real_time The current time, e.g. TAI or UT1.\n Units: s
 * \param[in] veh_struc_frame Structure frame of vehicle
 */
void RadiationPressure::third_body_adjustments(double real_time, RefFrame & veh_struc_frame)
{
    illum_factor = 1.0;
    //    multi_shadows = false;
    for(unsigned int ii_body = 0; ii_body < num_third_bodies; ++ii_body)
    {
        if(third_bodies[ii_body]->active)
        {
            illum_factor *= third_bodies[ii_body]->process_third_body(real_time, veh_struc_frame);
        }
    }
    source.flux_mag = source.flux_mag * illum_factor;
    Vector3::scale(illum_factor, source.flux_inertial);
    Vector3::scale(illum_factor, source.flux_struc);
}

/**
 * Adds a third body to the vector storage of third bodies
 * \param[in] third_body_ptr pointer to ThirdBody
 */
void RadiationPressure::add_third_body(RadiationThirdBody * third_body_ptr)
{
    if(third_body_ptr == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::invalid_setup_error,
                             "\n"
                             "Attempted to push a NULL object onto the ThirdBody vector in "
                             "the Radiation Pressure model.\n");
        return;
    }
    // third_body_ptr is not NULL
    // If third body is not named, then fail at initialization, and flag a
    // warning that the adding has failed (but continue with the simulation)
    // otherwise.
    if(third_body_ptr->name.empty())
    {
        if(initialized)
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  RadiationMessages::incomplete_setup_error,
                                  "\n"
                                  "Attempting to add a ThirdBody with no defined name.\n"
                                  "This cannot be done, since the name is needed to identify the "
                                  "reference frame.\n"
                                  "Addition of ThirdBody has failed.  Simulation proceeding without it."
                                  "\n");
            return;
        }
        else
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 RadiationMessages::incomplete_setup_error,
                                 "\n"
                                 "Attempting to add a ThirdBody with no defined name.\n"
                                 "This cannot be done, since the name is needed to identify the "
                                 "reference frame.\n");
        }
    }
    // Check that the Third Body has not already been added.  The
    // RadiaitonThirdBody points to the RadiationSource, which provides
    // information about the local radiation environment.  Therefore, the
    // RadiationSource is model specific, and therefore the RadiationThirdBody
    // must also be model-specific.  They cannot be shared across models.
    if(third_body_ptr->get_added_to_model())
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::invalid_setup_error,
                             "\n"
                             "Attempting to add a ThirdBody to more than one Radiation Pressure "
                             "model.\n"
                             "Offending object is called(%s).\n"
                             "ThirdBody objects can only be added to one Radiation Pressure model.\n"
                             "The same entity (vehicle, planet) can be represented as a ThirdBody\n"
                             "in multiple RadiationPressure models, but each representation must\n"
                             "be a separate instance of a ThirdBody.\n"
                             "E.g., there can be two ThirdBody objects called(%s), each linked to the\n"
                             "appropriate object (Planet or DynBody) of the same name, so that two \n"
                             "vehicles can each have their own RadiationPressure model with a \n"
                             "representation of the effects that(%s) has on their local radiation "
                             "field.\n",
                             third_body_ptr->name.c_str(),
                             third_body_ptr->name.c_str(),
                             third_body_ptr->name.c_str());
    }

    // Don't try adding two ThirdBody objects with the same name to one model.
    // This is usually an error of duplicity in setting up the model, so
    // duplicates are just ignored.
    for(unsigned int ii_t_b = 0; ii_t_b < num_third_bodies; ii_t_b++)
    {
        if(third_body_ptr->name == third_bodies[ii_t_b]->name.c_str())
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  RadiationMessages::invalid_setup_error,
                                  "\n"
                                  "In the Radiation Pressure Model, two RadiationThirdBody "
                                  "objects \n were found with identical names (%s).\n "
                                  "Omitting one.\n",
                                  third_body_ptr->name.c_str());
            return;
        }
    }

    third_bodies.push_back(third_body_ptr);
    third_body_ptr->set_added_to_model(true);

    //  If the model has already been initialized, then initialize the third
    //  body (else, the third bodies will all be initialized with the
    //  model, so no action needed).

    if(initialized)
    {
        third_body_ptr->initialize(dyn_manager_ptr);
    }

    // If any ThirdBody is active, the model should be looking for it/them.
    if(third_body_ptr->active)
    {
        third_bodies_active = true;
    }

    num_third_bodies = third_bodies.size();
}

/**
 * Sets a Third Body to be active when it previously was not
 * \param[in] third_body_name Name of ThirdBody
 */
void RadiationPressure::set_third_body_active(const std::string & third_body_name)
{
    int body_index = find_third_body(third_body_name);
    if(body_index >= 0)
    {
        if(third_bodies[body_index]->active)
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 RadiationMessages::invalid_function_call,
                                 "\n"
                                 "Call made to set the RadiationThirdBody(%s) active, but it is "
                                 "already active.\n",
                                 third_body_name.c_str());
        }
        else
        {
            third_bodies_active = true;
            third_bodies[body_index]->active = true;
            if(third_bodies[body_index]->local_frame_ptr != nullptr)
            {
                dyn_manager_ptr->subscribe_to_frame(*third_bodies[body_index]->local_frame_ptr);
            }
        }
    }
    else
    {
        // Continuation from find_third_body error message
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RadiationMessages::invalid_function_call,
                              "\n"
                              "Failure to find third body by name means it cannot be activated.");
    }
}

/**
 * Sets a Third Body to be inactive when it previously was not
 * \param[in] third_body_name Name of ThirdBody
 */
void RadiationPressure::set_third_body_inactive(const std::string & third_body_name)
{
    int body_index = find_third_body(third_body_name);
    if(body_index >= 0)
    {
        if(!third_bodies[body_index]->active)
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 RadiationMessages::invalid_function_call,
                                 "\n"
                                 "Call made to set the RadiationThirdBody(%s) inactive, but it is "
                                 "already inactive.\n",
                                 third_body_name.c_str());
        }
        else
        {
            third_bodies[body_index]->active = false;
            if(third_bodies[body_index]->local_frame_ptr != nullptr)
            {
                dyn_manager_ptr->unsubscribe_to_frame(*third_bodies[body_index]->local_frame_ptr);
            }
            third_bodies_active = false;
            for(unsigned int ii = 0; ii < num_third_bodies; ii++)
            {
                if(third_bodies[ii]->active)
                {
                    third_bodies_active = true;
                }
            }
        }
    }
    else
    {
        // Continuation from find_third_body error message
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RadiationMessages::invalid_function_call,
                              "\n"
                              "Failure to find third body by name means it cannot be deactivated.");
    }
}

/**
 * Finds the index of a third body based on its name
 * @return void
 * \param[in] third_body_name Name of ThirdBody
 */
int RadiationPressure::find_third_body(const std::string & third_body_name)
{
    for(unsigned int ii_body = 0; ii_body < num_third_bodies; ++ii_body)
    {
        if(third_bodies[ii_body]->name == third_body_name)
        {
            return ii_body;
        }
    }
    MessageHandler::error(__FILE__,
                          __LINE__,
                          RadiationMessages::invalid_function_call,
                          "\n"
                          "Call made to identify a RadiationThirdBody called(%s), but no such "
                          "entity exists.\n",
                          third_body_name.c_str());

    return -1;
}

/**
 * Sets the value calculate_forces
 * \param[in] value whether forces are needed
 */
void RadiationPressure::set_calculate_forces(bool value)
{
    calculate_forces = value;
    if(!calculate_forces)
    {
        Vector3::initialize(force);
        Vector3::initialize(torque);
    }
}

/**
 * Destructor for RadiationPressure
 */
RadiationPressure::~RadiationPressure()
{
    JEOD_DEREGISTER_CHECKPOINTABLE(this, third_bodies);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
