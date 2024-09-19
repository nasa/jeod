/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup RNPMars
 * @{
 *
 * @file models/environment/RNP/RNPMars/src/rnp_mars.cc
 * Implementation of RNPMars
 */

/*******************************************************************************

Purpose:
()

Reference:
(((Konopliv, Alex S., et al)
(A global solution for the Mars static and seasonal gravity, Mars
orientation, Phobos and Deimos masses, and Mars ephemeris)
(Icarus, Volume 182, Issue 1, Pages 23-50) (May 2006))
((Konopliv, Alex S., et al)
(Mars high resolution gravity fields from MRO, Mars season gravity, and
other dynamical parameters)
(Icarus, Volume 211, Issue 1, Pages 401-428) (January 2011)))

Assumptions and limitations:
((Mars specific))

Library dependencies:
((rnp_mars.cc)
(nutation_mars.cc)
(precession_mars.cc)
(rotation_mars.cc)
(environment/RNP/GenericRNP/src/planet_rnp.cc)
(environment/RNP/GenericRNP/src/RNP_messages.cc)
(environment/time/src/time_tt.cc)
(utils/message/src/message_handler.cc))


*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/RNP/GenericRNP/include/RNP_messages.hh"
#include "environment/planet/include/planet.hh"
#include "environment/time/include/time_dyn.hh"
#include "environment/time/include/time_manager.hh"
#include "environment/time/include/time_tt.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/rnp_mars.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Default constructor
 */
RNPMars::RNPMars()
{
    // Disable polar motion to reverse default inherited setting; "Pathfinder"
    // Mars RNP model does not provide polar motion as of 2010
    enable_polar = false;

    // Assign pointers for polymorphic functionality; note that the Pathfinder
    // Mars RNP model does not include polar motion as of 2010 due to
    // difficulties in determining the coefficient values.
    nutation = &this->NMars;
    precession = &this->PMars;
    rotation = &this->RMars;
    polar_motion = nullptr;
}

/**
 * Perform setup of the object, then call the base class initializer.
 * \param[in,out] dyn_manager Ref to dynamics manager
 */
void RNPMars::initialize(DynManager & dyn_manager)
{
    // Copy value from higher up the polymorphic inheritance chain
    RMars.planet_rotational_velocity = planet_omega;

    // Set the rotational option consistent with user selection
    if((rnp_type == RotationOnly) || (rnp_type == ConstantNP))
    {
        RMars.use_full_rnp = false;
    }
    else
    {
        RMars.use_full_rnp = true;
    }

    // Set up nutation, set pointers to it and initialize fixed matrices,
    // only if needed
    if(rnp_type == FullRNP)
    {
        RMars.nutation = &NMars;
        PMars.nutation = &NMars;
        PMars.compute_fixed_matrices();
    }

    // Call parent-class initializer
    PlanetRNP::initialize(dyn_manager);
}

/**
 * Update the complete RNP and supply it to the Dynamics Manager.
 * \param[in] time_tt Current Terrestrial Time
 */
void RNPMars::update_rnp(TimeTT & time_tt)
{
    // Do nothing if model inactive
    if(!active)
    {
        return;
    }

    // If the DynTime pointer is empty, connect it
    if(time_dyn_ptr == nullptr)
    {
        get_dyn_time_ptr(time_tt);
    }

    // If the TimeTT pointer hasn't been cached off yet, do so
    if(tt_ptr == nullptr)
    {
        tt_ptr = &time_tt;
    }

    // Check if RNP needs update; if it has been previously updated and the
    // timestamp still matches, then it doesn't
    if(!never_updated_full && Numerical::compare_exact(last_updated_time_full, time_dyn_ptr->seconds))
    {
        return;
    }

    // Proceed with full RNP update, including rotational portion
    never_updated_full = false;
    never_updated_rotational = false;
    last_updated_time_full = last_updated_time_rotational = time_dyn_ptr->seconds;

    double time = 0.0;
    time = time_tt.seconds;

    // Pass in TT to update all if user requested full RNP, or call rotational
    // update only and pass in simulation time to it if only rotation requested
    if(rotation != nullptr)
    {
        if(rnp_type == FullRNP)
        {
            rotation->update_time(time);
            nutation->update_time(time);
            precession->update_time(time);
        }
        else
        {
            time = time_tt.time_manager->dyn_time.seconds;
            rotation->update_time(time);
        }
    }

    // Update the timestamp of the controlled reference frame
    planet->pfix.set_timestamp(time_dyn_ptr->seconds);

    // Pass the function call to the parent object
    PlanetRNP::update_rnp();
}

/**
 * Update rotation portion of RNP and supply RNP to dynamics manager.
 * \param[in] time_tt Current TT time
 */
void RNPMars::update_axial_rotation(TimeTT & time_tt)
{
    // Do nothing if model inactive
    if(!active)
    {
        return;
    }

    // If the DynTime pointer is empty, connect it
    if(time_dyn_ptr == nullptr)
    {
        get_dyn_time_ptr(time_tt);
    }

    // Check if RNP needs update; if it has been previously updated and the
    // timestamp still matches, then it doesn't
    if(!never_updated_rotational && Numerical::compare_exact(last_updated_time_rotational, time_dyn_ptr->seconds))
    {
        return;
    }

    // Proceed with rotational RNP update only
    never_updated_rotational = false;
    last_updated_time_rotational = time_dyn_ptr->seconds;

    double time = 0.0;
    time = time_tt.seconds;

    // Pass in TT if user requested full RNP (meaning this is an intermediate
    // update), or pass in simulation time if only rotation being used
    if(rotation != nullptr)
    {
        if(rnp_type == FullRNP)
        {
            rotation->update_time(time);
        }
        else
        {
            time = time_tt.time_manager->dyn_time.seconds;
            rotation->update_time(time);
        }
    }

    // Update the timestamp of the controlled reference frame
    planet->pfix.set_timestamp(time_dyn_ptr->seconds);

    // Pass the function call to the parent object
    PlanetRNP::update_axial_rotation();
}

/**
 * Return the last time at which the RNP was updated.
 */
double RNPMars::timestamp() const
{
    return last_updated_time_rotational;
}

/**
 * Return the internal name of the object.
 */
std::string RNPMars::get_name() const
{
    return internal_name;
}

/**
 * Define 'alias' to call axial update function, for polymorphism.
 */
void RNPMars::ephem_update()
{
    if(active && orient_interface.is_active())
    {
        if(tt_ptr == nullptr)
        {
            MessageHandler::inform(__FILE__,
                                   __LINE__,
                                   RNPMessages::setup_error,
                                   "RNPMars::ephem_update was called without a valid "
                                   "tt_ptr being set. It is normally set in the "
                                   "RNPMars::update_rnp function. RNP was not updated.\n");
            return;
        }

        // If everything is ok, then pass call through to axial update routine
        update_axial_rotation(*tt_ptr);
    }
}

/**
 * Get simulation time via a two-step pointer trail.
 * \param[in] time_tt Current TT time
 */
void RNPMars::get_dyn_time_ptr(TimeTT & time_tt)
{
    // Check if pointer needs to be set; exit if not
    if(time_dyn_ptr != nullptr)
    {
        return;
    }

    // Set the pointer by following the trail
    time_dyn_ptr = dynamic_cast<TimeDyn *>(time_tt.time_manager->get_time_ptr("Dyn"));

    if(time_dyn_ptr == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RNPMessages::setup_error,
                             "The TimeManager associated with the given TimeTT "
                             "did not contain a TimeDyn object.\n");
        return;
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
