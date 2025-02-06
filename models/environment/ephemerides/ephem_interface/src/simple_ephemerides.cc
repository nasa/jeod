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
  ((simple_ephemerides.o)
   (ephem_messages.o)
   (ephem_ref_frame.o)
   (environment/ephemerides/ephem_item/ephem_item.o)
   (environment/ephemerides/ephem_item/ephem_point.o)
   (environment/ephemerides/ephem_manager/ephem_manager.o)
   (utils/message/message_handler.o))



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
namespace jeod {

/*******************************************************************************
 * Base class methods
 ******************************************************************************/


/**
 * Construct a SinglePointEphemeris object.
 */


/*
Purpose:
  (Construct a SinglePointEphemeris object.)
*/
SinglePointEphemeris::SinglePointEphemeris (
   void)
:
   identifier(NULL),
   update_time(0.0),
   active(true)
{
   ; // Empty
}


/**
 * Destruct a SinglePointEphemeris object.
 */
SinglePointEphemeris::~SinglePointEphemeris (
   void)
{
   if ((identifier != NULL) && (JEOD_IS_ALLOCATED (identifier))) {
      JEOD_DELETE_ARRAY (identifier);
   }
}


/**
 * Nominally, activate the model. Here, reject the request.
 */
void
SinglePointEphemeris::activate (
   void)
{
   if (! active) {
      MessageHandler::error (
         __FILE__, __LINE__, EphemeridesMessages::internal_error,
         "A previously inactive simple ephemeris cannot be re-activated.");
   }
}


/**
 * Deactivate the model.
 */
void
SinglePointEphemeris::deactivate (
   void)
{
   active = false;
}



/**
 * Set the name of a SinglePointEphemeris object.
 * \param[in,out] new_name Central point name
 */
void
SinglePointEphemeris::set_name (
   const char * new_name)
{

   // Trying to change the name is an error.
   if (identifier != NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
         "Erroneous call the SinglePointEphemeris::set_name(\"%s\")\n"
         "The model already has a name \"%s\".\n"
         "Rename request rejected.",
         new_name, identifier);
      return;
   }

   // Set the identifier as a copy (strdup) of the supplied name.
   identifier = JEOD_STRDUP (new_name);
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
EmptySpaceEphemeris::EmptySpaceEphemeris (
   void)
{
   central_point.set_owner (this);
   central_point.enable ();
}


/**
 * Destruct an EmptySpaceEphemeris object.
 */
EmptySpaceEphemeris::~EmptySpaceEphemeris (
   void)
{
   ; // Empty
}


/**
 * Set the name of an EmptySpaceEphemeris object.
 * \param[in,out] new_name Ephemeris name
 */
void
EmptySpaceEphemeris::set_name (
   const char * new_name)
{
   std::string sname(new_name);
   static const char dotInrtlName[] = ".inertial";
   static const char inrtlName[] = "inertial";

   // Set the name of the central point and central frame.
   // The central point and frame must be named "xxx.inertial".
   if ((sname.length() >= 10) &&
       (sname.compare(sname.length() - 9, sizeof(dotInrtlName), dotInrtlName) == 0)) {
      central_point.set_name (new_name);
      central_frame.set_name (new_name);
   }
   else {
      central_point.set_name (new_name, inrtlName);
      central_frame.set_name (new_name, inrtlName);
   }

   // Pass the message to the parent.
   SinglePointEphemeris::set_name (new_name);
}


/**
 * Initialize an EmptySpaceEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
EmptySpaceEphemeris::initialize_model (
   EphemeridesManager & ephem_manager)
{

   // Nothing to do if the model is inactive.
   if (! active) {
      return;
   }

   ephem_manager.add_ephemeris (*this);
   ephem_manager.add_ephem_item (central_point);
   ephem_manager.add_integ_frame (central_frame);
}


/**
 * Initialize an EmptySpaceEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
EmptySpaceEphemeris::ephem_initialize (
   EphemeridesManager & ephem_manager JEOD_UNUSED)
{
   // At this point, the central point must have a target frame.
   if (central_point.get_target_frame() == NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
         "Could not find planet named \"%s\".\n"
         "Shutting down the EmptySpaceEphemeris.",
         identifier);
      central_point.disable ();
      deactivate ();

      return;
   }
}


/**
 * Activate an EmptySpaceEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
EmptySpaceEphemeris::ephem_activate (
   EphemeridesManager & ephem_manager JEOD_UNUSED)
{
   ; // Empty
}


/**
 * Build the reference frame tree with the central frame as the root.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
EmptySpaceEphemeris::ephem_build_tree (
   EphemeridesManager & ephem_manager)
{

   // Nothing to do if the model is inactive.
   if (! active) {
      return;
   }

   ephem_manager.add_frame_to_tree (central_frame, NULL);
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
SinglePlanetEphemeris::SinglePlanetEphemeris (
   void)
{
   central_point.set_owner (this);
   central_point.enable ();
}


/**
 * Destruct an SinglePlanetEphemeris object.
 */
SinglePlanetEphemeris::~SinglePlanetEphemeris (
   void)
{
   ; // Empty
}


/**
 * Set the name of a SinglePlanetEphemeris object.
 * \param[in,out] new_name Central point name
 */
void
SinglePlanetEphemeris::set_name (
   const char * new_name)
{
   std::string sname(new_name);
   static const char dotInrtlName[] = ".inertial";
   static const char inrtlName[] = "inertial";

   // Set the name of the central point and central frame.
   // The central point and frame must be named "xxx.inertial".
   if ((sname.length() >= 10) &&
       (sname.compare(sname.length() - 9, sizeof(dotInrtlName), dotInrtlName) == 0)) {
      central_point.set_name (new_name);
   }
   else {
      central_point.set_name (new_name, inrtlName);
   }

   SinglePointEphemeris::set_name (new_name);
}



/**
 * Initialize a SinglePlanetEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
SinglePlanetEphemeris::initialize_model (
   EphemeridesManager & ephem_manager)
{

   // Nothing to do if the model is inactive.
   if (! active) {
      return;
   }

   ephem_manager.add_ephemeris (*this);
   ephem_manager.add_ephem_item (central_point);
}


/**
 * Initialize a SinglePlanetEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
SinglePlanetEphemeris::ephem_initialize (
   EphemeridesManager & ephem_manager)
{

   // Nothing to do if the model is inactive.
   if (! active) {
      return;
   }

   unsigned int nplanets = ephem_manager.get_num_planets();
   if (nplanets != 1) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
         "%s planets registered with the EphemeridesManager; "
         "exactly one is needed in single planet mode",
         nplanets == 0 ? "No" : "More than one");

      central_point.disable ();
      deactivate ();

      return;
   }

   // At this point, the planet with same name as the model must exist and
   // the central point must have a target frame.
   if ((ephem_manager.find_base_planet (identifier) == NULL) ||
       (central_point.get_target_frame() == NULL)) {
      MessageHandler::error (
         __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
         "Could not find planet named \"%s\".\n"
         "Shutting down the SinglePlanetEphemeris.",
         identifier);
      central_point.disable ();
      deactivate ();

      return;
   }
}


/**
 * Activate a SinglePlanetEphemeris object.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
SinglePlanetEphemeris::ephem_activate (
   EphemeridesManager & ephem_manager JEOD_UNUSED)
{
   ; // Empty
}


/**
 * Build the reference frame tree with the central frame as the root.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
SinglePlanetEphemeris::ephem_build_tree (
   EphemeridesManager & ephem_manager)
{

   // Nothing to do if the model is inactive.
   if (! active) {
      return;
   }

   ephem_manager.add_frame_to_tree (
      *(central_point.get_target_frame()), NULL);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
