/*******************************************************************************

Purpose:
  (Define methods for PlanetSite.)

Library dependencies:
  ((planet_site.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/planet/include/planet.hh"
#include "utils/planet_fixed/north_east_down/include/north_east_down.hh"
#include "utils/ref_frames/include/ref_frame_state.hh"
#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_messages.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/planet_site.hh"



//! Namespace jeod
namespace jeod {

/*******************************************************************************
Function: PlanetSite::PlanetSite
Purpose: (PlanetSite default constructor.)
*******************************************************************************/
PlanetSite::PlanetSite ( // Return: -- N/A
   void)
:
   ned(),
   site_id(NULL)
{
   altlatlong_type = NorthEastDown::elliptical;
   return;
}


/*******************************************************************************
Function: PlanetSite::~PlanetSite
Purpose: (PlanetSite destructor.)
*******************************************************************************/
PlanetSite::~PlanetSite ( // Return: -- N/A
   void)
{

   ned.ned_frame.remove_from_parent();

   // Recant the registration done by the initialize() method.
   if (0) {
      // FIXME: There is no way to do this yet.
   }

   return;
}


/*******************************************************************************
Function: PlanetSite::initialize
Purpose:  (Initialize a planet site.)
*******************************************************************************/
void
PlanetSite::initialize (              // Return: -- Void
   Planet & planet,                   // Inout:  -- Planet
   DynManager & dyn_manager)          // Inout:  -- Dynamics manager
{

   // Initialize the site.
   ned.initialize (&planet);
   ned.ned_frame.set_name (planet.name.c_str(), site_id, "ned");

   // Register the frame and connect it to the planet.
   dyn_manager.add_ref_frame (ned.ned_frame);
   planet.pfix.add_child (ned.ned_frame);


   // Set the state with the supplied point, interpreted as geocentric
   // or geodetic per the altlatlong_type enum.
   switch (altlatlong_type) {
   case NorthEastDown::spherical:
      ned.update_from_spher (loc);
      break;

   case NorthEastDown::elliptical:
      ned.update_from_ellip (loc);
      break;

   case NorthEastDown::undefined:
   default:
      MessageHandler::fail (
         __FILE__, __LINE__, PlanetFixedMessages::invalid_request,
         "Bad AltLatLong type");
      return;

      // Not reached
      return;
   }


   // Construct the N-E-D frame's orientation.
   ned.altlatlong_type = altlatlong_type;
   ned.build_ned_orientation();
}

} // End JEOD namespace
