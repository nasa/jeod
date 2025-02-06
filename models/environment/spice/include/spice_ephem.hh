//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
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
 * @addtogroup Spice
 * @{
 *
 * @file models/environment/spice/include/spice_ephem.hh
 * Define class for the SPICE ephemeris model.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/spice_ephem.cc))



*******************************************************************************/

#ifndef JEOD_SPICE_EPHEM_HH
#define JEOD_SPICE_EPHEM_HH


// System includes
#include <string>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_interface.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"
#include "environment/time/include/class_declarations.hh"
#include "utils/container/include/object_vector.hh"
#include "utils/container/include/pointer_vector.hh"
#include "utils/container/include/simple_checkpointable.hh"
#include "utils/ref_frames/include/ref_frame_interface.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "spice_ephem_orient.hh"
#include "spice_ephem_point.hh"


//! Namespace jeod
namespace jeod {

/**
 * The S_define-level class that provides planetary ephemerides.
 * The SpiceEphemeris class constructs the ephemeris reference frame tree and
 * updates the states of the planets based on data from a SPICE file or files.
 */
class SpiceEphemeris :
   public EphemerisInterface, public RefFrameOwner, public SimpleCheckpointable {
JEOD_MAKE_SIM_INTERFACES(SpiceEphemeris)

public:

   // Member functions
   // Note: The copy constructor and assignment operator are deleted.

   // Default constructor
   SpiceEphemeris (void);

   // Destructor
   ~SpiceEphemeris (void) override;


   // S_define-level call to initialize the model (not inherited)
   void initialize_model (
      const TimeManager & time_manager, EphemeridesManager & ephem_manager);


   // EphemInterface-inherited housekeeping and accessor methods
   void activate (void) override;
   void deactivate (void) override;
   double timestamp (void) const override;
   const char * get_name (void) const override;


   // Inherited methods for interacting with EphemeridesManager
   void ephem_initialize (EphemeridesManager & ephem_manager) override;
   void ephem_activate (EphemeridesManager & ephem_manager) override;
   void ephem_build_tree (EphemeridesManager & ephem_manager) override;
   void ephem_update (void) override;


   // Ensure object is checkpointable/restartable (is inherited)
   void simple_restore (void) override;

   // Interface method for Trick input deck to add objects to load from SPICE
   void add_planet_name (std::string planet_name)
   { planet_names.push_back(planet_name); }

   // Interface method for Trick input deck to add objects for which
   // orientation is required.
   void add_orientation (std::string object_name)
   { orientation_names.push_back(object_name); }

   // Find a given SPICE ID in the list of loaded SPICE objects
   SpiceEphemPoint * find_spice_id (int id_to_find);

   // Member data

   /**
    * The name of a text file containing the list of SPICE files to be loaded.
    * Must be in format needed by SPICE furnsh_c() function. See comments for
    * method load_spice_files() in spice_ephem.cc for format details.
    */
   std::string metakernel_filename; //!< trick_units(--)


protected:

   /**
    * If set to true, makes the model inactive.
    * This is set to false (i.e., model is active) by the constructor.
    * Setting this flag to true prior to initialization time will result in the
    * model never doing anything. Setting this flag to true after the model has
    * been active for some time turns it off from that point forward. Turning on
    * the model after it has been inactive for some time is not supported.
    */
   bool inactive; //!< trick_units(--)


private:

   // Member data

   /**
    * Is an update needed even if the time hasn't changed?
    */
   bool force_update; //!< trick_units(--)

   /**
    * The names of all planets to load into the simulation.
    */
   JeodObjectVector<std::string>::type planet_names; //!< trick_io(**)

   /**
    * The names of all objects for which orientation is required.
    */
   JeodObjectVector<std::string>::type orientation_names; //!< trick_io(**)

   /**
    * All the spk objects tracked by this ephemeris
    */
   JeodPointerVector<SpiceEphemPoint>::type loaded_spk; //!< trick_io(**)

   /**
    * Objects defining planet-fixed reference frames
    */
   JeodPointerVector<SpiceEphemOrientation>::type planetary_orientations; //!< trick_io(**)

   /**
    * Identifier for this model, set by the constructor.
    */
   std::string ident; //!< trick_units(--)

   /**
    * Time of last update, dynamic time seconds
    */
   double update_time; //!< trick_units(s)

   /**
    * Array of barycenter frames corresponding to SSBary and the planets
    */
   EphemerisRefFrame barycenter_frames[10]; //!< trick_units(--)

   /**
    * The root point in the reference frame tree
    */
   SpiceEphemPoint * root_item; //!< trick_units(--)

   /**
    * The source of ephemeris time information
    */
   const double * tdb_seconds; //!< trick_units(--)

   /**
    * The source of dynamic time information
    */
   const double * dyn_seconds; //!< trick_units(--)

   /**
    * Local ephem manager pointer to eliminate cascading passes in methods
    * where possible (note some in inherited methods must remain).
    */
   EphemeridesManager * ephem_mngr_local; //!< trick_units(--)


   // Member functions

   // Initialize timing
   void initialize_time (const TimeManager & time_manager);

   // Load specified SPICE files
   void load_spice_files (void);

   // Locate and list spk objects to be used in simulation
   void process_spk (void);

   // Locate and process planetary orientation frames
   void process_orientations (void);

   // Introduce an EphemerisItem to the EphemeridesManager
   void introduce_item (EphemerisItem & item);

   // Populate a generic EphemerisItem
   void populate_item (EphemerisItem & item, const std::string & name);

   // Create and populate basic attributes of a new SPICE ephemeris point
   SpiceEphemPoint * create_new_ephem_point (
      std::string object_name, const std::string & spice_name);

   // Create and populate basic attributes of a SpiceEphemOrientation
   SpiceEphemOrientation * create_new_ephem_orientation (std:: string jeod_name);

   // Initialize the SPICE ephemeris items
   void initialize_items ();

   // Transform SPICE body name to JEOD naming conventions
   std::string spice_2_jeod (std::string spice_name);

   // Transform JEOD body name to SPICE pfix frame name
   std::string jeod_2_spice_pfix (std::string jeod_name);
   // Name the barycenter frames in the barycenter_frames array
   void name_barycenter_frames ();

   // Add a new barycenter. Creates the spice ephem point and registers the frame.
   void add_barycenter (int spice_id);

   // Check what barycenters are needed based on SPICE objects requested
   // and create any needed
   void create_barycenters ();

   // Determine which node should be the root of the ref frame tree
   void determine_root_node (void);

   // Recursively add all descendants from the target frame of a SpiceEphemPoint
   void add_descendants_r (SpiceEphemPoint * parent);

   // Find the SPICE parent ID for a given ID
   // Input should be the ID of a natural object.
   // Returns a planetary or solar system barycenter
   int find_parent_id (int obj_id);

   // Update purely translational ephemerides
   void update_trans ();

   // Update rotational state of body-fixed frames
   void update_rot ();

   // Disable error handling in SPICE
   void mute_spice_errors ();

   // Member data

   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   /**
    * Not implemented.
    */
   SpiceEphemeris (const SpiceEphemeris &);

   /**
    * Not implemented.
    */
   SpiceEphemeris & operator= (const SpiceEphemeris &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
