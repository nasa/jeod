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
 * @file models/environment/ephemerides/ephem_manager/include/ephem_manager.hh
 * Define the EphemManager class, which manages the ephemeris models in a
 * JEOD-based simulation.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((ephem_manager.o))


*******************************************************************************/


#ifndef JEOD_EPHEM_MANAGER_HH
#define JEOD_EPHEM_MANAGER_HH


// System includes

// JEOD includes
#include "utils/ref_frames/include/ref_frame_manager.hh"
#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "base_ephem_manager.hh"


//! Namespace jeod
namespace jeod {

/**
 * The EphemeridesManager class manages the ephemeris models in a simulation.
 * The primary functions of a EphemeridesManager are to:
 *   - Maintain lists of planets, ephemeris models, ephemeris items, and
 *     integration frames, and to provide lookup methods for these lists.
 *   - Dynamically determine which ephemerides are needed in a simulation.
 *   - Initialize ephemeris models and keep them in sync with the rest
 *     of the simulation.
 */
class EphemeridesManager :
   virtual public BaseEphemeridesManager,
   public RefFrameManager {

JEOD_MAKE_SIM_INTERFACES(EphemeridesManager)

public:

   // Member functions
   // Note: The copy constructor and assignment operator are deleted.


   // Constructor and destructor
   EphemeridesManager ();
   ~EphemeridesManager ();


   // Ref frame tree methods
   // Note that the first is *not* defined in BaseEphemeridesManager.
   // Making a derivative rate function virtual is too expensive.

   /**
    * Query if the reference frame tree needs to be rebuilt.
    * @return regenerate_ref_frame_tree data member.
    */
   bool ref_frame_tree_needs_rebuild ()
   const
   { return regenerate_ref_frame_tree; }


   // Denote that the tree needs to be rebuilt.
   virtual void ephem_note_tree_status_change ();


   // Planet methods

   // Add a planet to the list of such.
   virtual void add_planet (BasePlanet & planet);
   virtual void add_planet (Planet & planet);

   // Find a planet.
   virtual BasePlanet * find_base_planet (const char * name) const;
   virtual Planet * find_planet (const char * name) const;

   // Return number of registered planets.
   virtual unsigned int get_num_planets (void) const;


   // Ephemeris model methods

   // Add an ephemeris model to the list of such.
   virtual void add_ephemeris (EphemerisInterface & ephem_if);

   // Deactivate all registered ephemeris models
   virtual void clear_added_ephemerides (void);

   // Disable registration of new ephemeris models.
   virtual void disable_add_ephemeris (void);


   // EphemerisItem methods

   // Add an ephemeris item to the list of such.
   virtual void add_ephem_item (EphemerisItem & ephem_item);

   // Find an ephemeris item.
   virtual EphemerisItem * find_ephem_item (const char * name) const;

   // Find an ephemeris orientation.
   virtual EphemerisOrientation * find_ephem_angle (const char * name) const;

   // Find an ephemeris point.
   virtual EphemerisPoint * find_ephem_point (const char * name) const;


   // Integration frame methods

   // Add an integration frame to the list of such.
   virtual void add_integ_frame (EphemerisRefFrame & ref_frame);

   // Find an integration frame.
   virtual EphemerisRefFrame * find_integ_frame (const char * name) const;

   // Check whether a reference frame is an integration frame.
   virtual bool is_integ_frame (const RefFrame & ref_frame) const;

   // Find a reference frame's index in the list of integration frames.
   virtual unsigned int find_integ_frame_index (
      const EphemerisRefFrame & ref_frame) const;

   // Get the vector of integration frames.
   virtual const std::vector<EphemerisRefFrame *> & get_integ_frames (void)
   const;


   // Overridden reference frame methods

   // Add a reference frame to the list of such.
   virtual void add_ref_frame (RefFrame & ref_frame);

   void set_target_frame( RefFrame & ref_frame);


   // Top-level (e.g., S_define-level) methods
   // These are not inherited from BaseEphemeridesManager.

   // Initialize the registered ephemeris models
   void initialize_ephemerides (void);

   // Ask the ephemeris models to activate the ephemerides they control
   void activate_ephemerides (void);

   // Ask ephemeris models to update the ephemerides they control
   void update_ephemerides (void);


protected:

   // Member data
   // NOTE WELL: These are protected rather than private because of simulation
   // engine limitations. Inheriting classes should treat these as private
   // and use access methods.


   /**
    * Set via a call to disable_add_ephemeris, typically to allow a simple
    * ephemeris model to be active with all other models made inactive.
    */
   bool single_ephem_mode; //!< trick_units(--)

   /**
    * Set when the reference frame tree needs to be regenerated.
    */
   bool regenerate_ref_frame_tree; //!< trick_units(--)

   /**
    * Time of last update
    */
   double update_time; //!< trick_units(s)

   /**
    * The planets in a simulation, typically defined at the S_define level.
    */
   JeodPointerVector<BasePlanet>::type planets;  //!< trick_io(**)

   /**
    * The ephemerides models managed by this EphemeridesManager.
    */
   JeodPointerVector<EphemerisInterface>::type ephemerides; //!< trick_io(**)

   /**
    * The heads of the ephemeris item lists. All items in one of these sublists
    * share the same name. The members of the ephem_items list have distinct
    * names.
    */
   JeodPointerVector<EphemerisItem>::type ephem_items; //!< trick_io(**)

   /**
    * List of reference frames that are not rotating with respect to the
    * root node of the reference frame tree.
    */
   JeodPointerVector<EphemerisRefFrame>::type integ_frames; //!< trick_io(**)


private:

   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   ///
   /// Not implemented.
   EphemeridesManager (const EphemeridesManager &);
   ///
   /// Not implemented.
   EphemeridesManager & operator= (const EphemeridesManager &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
