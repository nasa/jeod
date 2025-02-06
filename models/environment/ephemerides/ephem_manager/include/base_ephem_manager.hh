//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemManager
 * @{
 *
 * @file models/environment/ephemerides/ephem_manager/include/base_ephem_manager.hh
 * Define the BaseEphemManager class, which defines the interfaces to the
 * class EphemManager.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/


#ifndef JEOD_BASE_EPHEM_MANAGER_HH
#define JEOD_BASE_EPHEM_MANAGER_HH


// System includes
#include <vector>

// JEOD includes
#include "utils/ref_frames/include/base_ref_frame_manager.hh"
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

// Forward declarations
class BasePlanet;
class Planet;
class EphemerisInterface;
class EphemerisRefFrame;
class EphemerisItem;
class EphemerisOrientation;
class EphemerisPoint;


/**
 * The EphemManager class augments the RefFrameManager with ephemeris-related
 * items. This class defines the external interfaces to that class.
 */
class BaseEphemeridesManager :
   virtual public BaseRefFrameManager {

JEOD_MAKE_SIM_INTERFACES(BaseEphemeridesManager)

public:

   // Member functions
   // Note: The default constructor, copy constructor, and assignment operator
   // for this class are not declared. The C++ defaults are in force.

   /**
    * Destructor.
    */
   virtual ~BaseEphemeridesManager () {}


   // Ref frame tree methods

   /**
    * Denote that the tree needs to be rebuilt.
    */
   virtual void ephem_note_tree_status_change () = 0;


   // Planet methods

   /**
    * Add a planet to the list of such.
    * @param planet  Planet to be added.
    */
   virtual void add_planet (BasePlanet & planet) = 0;

   /**
    * Add a planet to the list of such.
    * @param planet  Planet to be added.
    */
   virtual void add_planet (Planet & planet) = 0;

   /**
    * Find a planet.
    * @param name  Planet name.
    * @return Pointer to found planet.
    */
   virtual BasePlanet * find_base_planet (const char * name) const = 0;

   /**
    * Find a planet.
    * @param name  Planet name.
    * @return Pointer to found planet.
    */
   virtual Planet * find_planet (const char * name) const = 0;

   /**
    * Return number of registered planets.
    * @return Number of planets.
    */
   virtual unsigned int get_num_planets (void) const = 0;


   // Ephemeris model methods

   /**
    * Add an ephemeris model to the list of such.
    * @param ephem_if  Ephemeris model to be added.
    */
   virtual void add_ephemeris (EphemerisInterface & ephem_if) = 0;

   /**
    * Deactivate all registered ephemeris models.
    */
   virtual void clear_added_ephemerides (void) = 0;

   /**
    * Disable registration of new ephemeris models.
    */
   virtual void disable_add_ephemeris (void) = 0;


   // EphemerisItem methods

   /**
    * Add an ephemeris item to the list of such.
    * @param ephem_item  Item to be added.
    */
   virtual void add_ephem_item (EphemerisItem & ephem_item) = 0;

   /**
    * Find an ephemeris item.
    * @param name  Item to be found.
    * @return Found item.
    */
   virtual EphemerisItem * find_ephem_item (const char * name) const = 0;

   /**
    * Find an ephemeris orientation.
    * @param name  Item to be found.
    * @return Found item.
    */
   virtual EphemerisOrientation * find_ephem_angle (const char * name)
   const = 0;

   /**
    * Find an ephemeris point.
    * @param name  Item to be found.
    * @return Found item.
    */
   virtual EphemerisPoint * find_ephem_point (const char * name) const = 0;


   // Integration frame methods

   /**
    * Add an integration frame to the list of such.
    * @param ref_frame  Frame to be added.
    */
   virtual void add_integ_frame (EphemerisRefFrame & ref_frame) = 0;

   /**
    * Find an integration frame.
    * @param name  Frame to be found.
    * @return Found frame.
    */
   virtual EphemerisRefFrame * find_integ_frame (const char * name) const = 0;

   /**
    * Check whether a reference frame is an integration frame.
    * @param ref_frame  Frame to be checked.
    * @return True if ref_frame is an integration frame, false otherwise.
    */
   virtual bool is_integ_frame (const RefFrame & ref_frame) const = 0;

   /**
    * Find a reference frame's index in the list of integration frames.
    * @param ref_frame  Frame to be checked.
    * @return Frame index.
    */
   virtual unsigned int find_integ_frame_index (
      const EphemerisRefFrame & ref_frame) const = 0;

   /**
    * Get the vector of integration frames.
    * @return Vector of reference frame pointers.
    */
   virtual const std::vector<EphemerisRefFrame *> & get_integ_frames (void)
   const = 0;
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
