/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemInterface
 * @{
 *
 * @file models/environment/ephemerides/ephem_interface/src/simple_ephemerides.cc
 * Define member functions for the SinglePointEphemeris class and subclasses.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((simple_ephemerides.cc)
   (ephem_messages.cc)
   (ephem_ref_frame.cc)
   (environment/ephemerides/ephem_item/src/ephem_item.cc)
   (environment/ephemerides/ephem_item/src/ephem_point.cc)
   (environment/ephemerides/ephem_manager/src/ephem_manager.cc)
   (utils/message/src/message_handler.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/ephem_messages.hh"
#include "../include/simple_ephemerides.hh"

//! Namespace jeod
namespace jeod
{

/*******************************************************************************
 * Base class methods
 ******************************************************************************/

/**
 * Nominally, activate the model. Here, reject the request.
 */
void SinglePointEphemeris::activate()
{
    if(!active)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              EphemeridesMessages::internal_error,
                              "A previously inactive simple ephemeris cannot be re-activated.");
    }
}

/**
 * Deactivate the model.
 */
void SinglePointEphemeris::deactivate()
{
    active = false;
}

/**
 * Set the name of a SinglePointEphemeris object.
 * \param[in,out] new_name Central point name
 */
void SinglePointEphemeris::set_name(const std::string & new_name)
{
    // Trying to change the name is an error.
    if(!identifier.empty())
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              EphemeridesMessages::inconsistent_setup,
                              "Erroneous call the SinglePointEphemeris::set_name(\"%s\")\n"
                              "The model already has a name \"%s\".\n"
                              "Rename request rejected.",
                              new_name.c_str(),
                              identifier.c_str());
        return;
    }

    // Set the identifier as a copy of the supplied name.
    identifier = new_name;
}

/*******************************************************************************
 * Empty space methods
 ******************************************************************************/

/**
 * Construct an EmptySpaceEphemeris object.
 */

/*
Purpose:
  (Construct an EmptySpaceEphemeris object.)
*/
EmptySpaceEphemeris::EmptySpaceEphemeris()
{
    central_point.set_owner(this);
    central_point.enable();
}

/**
 * Set the name of an EmptySpaceEphemeris object.
 * \param[in,out] new_name Ephemeris name
 */
void EmptySpaceEphemeris::set_name(const std::string & new_name)
{
    std::string sname(new_name);
    static const std::string dotInrtlName = ".inertial";
    static const std::string inrtlName = "inertial";

    // Set the name of the central point and central frame.
    // The central point and frame must be named "xxx.inertial".
    if((sname.length() >= 10) && (sname.compare(sname.length() - 9, dotInrtlName.length(), dotInrtlName) == 0))
    {
        central_point.set_name(new_name);
        central_frame.set_name(new_name);
    }
    else
    {
        central_point.set_name(new_name, inrtlName);
        central_frame.set_name(new_name, inrtlName);
    }

    // Pass the message to the parent.
    SinglePointEphemeris::set_name(new_name);
}

/**
 * Initialize an EmptySpaceEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void EmptySpaceEphemeris::initialize_model(EphemeridesManager & ephem_manager)
{
    // Nothing to do if the model is inactive.
    if(!active)
    {
        return;
    }

    ephem_manager.add_ephemeris(*this);
    ephem_manager.add_ephem_item(central_point);
    ephem_manager.add_integ_frame(central_frame);
}

/**
 * Initialize an EmptySpaceEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void EmptySpaceEphemeris::ephem_initialize(EphemeridesManager & ephem_manager JEOD_UNUSED)
{
    // At this point, the central point must have a target frame.
    if(central_point.get_target_frame() == nullptr)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              EphemeridesMessages::inconsistent_setup,
                              "Could not find planet named \"%s\".\n"
                              "Shutting down the EmptySpaceEphemeris.",
                              identifier.c_str());
        central_point.disable();
        deactivate();

        return;
    }
}

/**
 * Activate an EmptySpaceEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void EmptySpaceEphemeris::ephem_activate(EphemeridesManager & ephem_manager JEOD_UNUSED)
{
    ; // Empty
}

/**
 * Build the reference frame tree with the central frame as the root.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void EmptySpaceEphemeris::ephem_build_tree(EphemeridesManager & ephem_manager)
{
    // Nothing to do if the model is inactive.
    if(!active)
    {
        return;
    }

    ephem_manager.add_frame_to_tree(central_frame, nullptr);
}

/*******************************************************************************
 * Single planet methods
 ******************************************************************************/

/**
 * Construct an SinglePlanetEphemeris object.
 */

/*
Purpose:
  (Construct an SinglePlanetEphemeris object.)
*/
SinglePlanetEphemeris::SinglePlanetEphemeris()
{
    central_point.set_owner(this);
    central_point.enable();
}

/**
 * Set the name of a SinglePlanetEphemeris object.
 * \param[in,out] new_name Central point name
 */
void SinglePlanetEphemeris::set_name(const std::string & new_name)
{
    std::string sname(new_name);
    static const char dotInrtlName[] = ".inertial";
    static const char inrtlName[] = "inertial";

    // Set the name of the central point and central frame.
    // The central point and frame must be named "xxx.inertial".
    if((sname.length() >= 10) && (sname.compare(sname.length() - 9, sizeof(dotInrtlName), dotInrtlName) == 0))
    {
        central_point.set_name(new_name);
    }
    else
    {
        central_point.set_name(new_name, inrtlName);
    }

    SinglePointEphemeris::set_name(new_name);
}

/**
 * Initialize a SinglePlanetEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void SinglePlanetEphemeris::initialize_model(EphemeridesManager & ephem_manager)
{
    // Nothing to do if the model is inactive.
    if(!active)
    {
        return;
    }

    ephem_manager.add_ephemeris(*this);
    ephem_manager.add_ephem_item(central_point);
}

/**
 * Initialize a SinglePlanetEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void SinglePlanetEphemeris::ephem_initialize(EphemeridesManager & ephem_manager)
{
    // Nothing to do if the model is inactive.
    if(!active)
    {
        return;
    }

    unsigned int nplanets = ephem_manager.get_num_planets();
    if(nplanets != 1)
    {
        const char planetQuantityStrings[2][14] = {"No", "More than one"};
        int index = 1;
        if(nplanets == 0)
        {
            index = 0;
        }

        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EphemeridesMessages::inconsistent_setup,
                             "%s planets registered with the EphemeridesManager; "
                             "exactly one is needed in single planet mode",
                             planetQuantityStrings[index]);

        central_point.disable();
        deactivate();

        return;
    }

    // At this point, the planet with same name as the model must exist and
    // the central point must have a target frame.
    if((ephem_manager.find_base_planet(identifier) == nullptr) || (central_point.get_target_frame() == nullptr))
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              EphemeridesMessages::inconsistent_setup,
                              "Could not find planet named \"%s\".\n"
                              "Shutting down the SinglePlanetEphemeris.",
                              identifier.c_str());
        central_point.disable();
        deactivate();

        return;
    }
}

/**
 * Activate a SinglePlanetEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void SinglePlanetEphemeris::ephem_activate(EphemeridesManager & ephem_manager JEOD_UNUSED)
{
    ; // Empty
}

/**
 * Build the reference frame tree with the central frame as the root.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void SinglePlanetEphemeris::ephem_build_tree(EphemeridesManager & ephem_manager)
{
    // Nothing to do if the model is inactive.
    if(!active)
    {
        return;
    }

    ephem_manager.add_frame_to_tree(*(central_point.get_target_frame()), nullptr);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
