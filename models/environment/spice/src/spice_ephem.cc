/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Spice
 * @{
 *
 * @file models/environment/spice/src/spice_ephem.cc
 * Define the methods for the SPICE ephemeris model class.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
((The JPL SPICE system uses TDB time.))

Library Dependencies:
((environment/ephemerides/ephem_interface/src/ephem_messages.cc)
 (environment/time/src/time_manager.cc)
 (utils/named_item/src/named_item.cc)
 (utils/memory/src/memory_manager_static.cc)
 (utils/message/src/message_handler.cc))



*******************************************************************************/


// System includes.
#include <cstdarg>
#include <set>
#include <fstream>
#include <algorithm>

// Include SPICE library header for access to its functions.
#include "SpiceUsr.h"

// JEOD includes.
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "environment/time/include/time_manager.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes.
#include "../include/spice_ephem.hh"


// Set sizes of SPICE character strings and upper bound on total number of IDs.
const static int MAX_PATH_LENGTH = 129;
const static int MAX_NAME_LENGTH = 33;
const static int MAX_MSG_LENGTH = 1841;
const static int MAX_IDS = 1000;


//! Namespace jeod
namespace jeod {

/**
 * SpiceEphemeris default constructor.
 */
SpiceEphemeris::SpiceEphemeris (
   void)
:
   inactive(false),
   force_update(false),
   ident("SPICE"),
   update_time(-99e99),
   root_item(nullptr),
   tdb_seconds(nullptr),
   dyn_seconds(nullptr)
{


   JEOD_REGISTER_CLASS (SpiceEphemeris);
   JEOD_REGISTER_CHECKPOINTABLE (this, planet_names);
   JEOD_REGISTER_CHECKPOINTABLE (this, orientation_names);
   JEOD_REGISTER_CHECKPOINTABLE (this, loaded_spk);
   JEOD_REGISTER_CHECKPOINTABLE (this, planetary_orientations);
}


/**
 * SpiceEphemeris destructor.
 */
SpiceEphemeris::~SpiceEphemeris (
   void)
{
   JEOD_DEREGISTER_CHECKPOINTABLE (this, planet_names);
   JEOD_DEREGISTER_CHECKPOINTABLE (this, orientation_names);

   // Free dynamically allocated SpiceEphemPoints.
   unsigned int size_loaded_spk = loaded_spk.size();
   for (unsigned int ii = 0; ii < size_loaded_spk; ++ii) {

      // Note: macro expects a pointer to object to be freed.
      JEOD_DELETE_OBJECT(loaded_spk[ii]);
   }

   // Free dynamically allocated SpiceEphemOrientations.
   unsigned int size_loaded_orientations = planetary_orientations.size();
   for (unsigned int ii = 0; ii < size_loaded_orientations; ++ii) {

      // Note: macro expects a pointer to object to be freed.
      JEOD_DELETE_OBJECT(planetary_orientations[ii]);
   }

   JEOD_DEREGISTER_CHECKPOINTABLE (this, loaded_spk);
   JEOD_DEREGISTER_CHECKPOINTABLE (this, planetary_orientations);
}


/**
 * Nominally, activate the object. In the case of a SpiceEphemeris object,
 * an inactive object cannot be activated once the simulation starts.
 * Note that a SpiceEphemeris is active by default.
 */
void
SpiceEphemeris::activate (
   void)
{
   if (inactive) {
      MessageHandler::error (
         __FILE__, __LINE__, EphemeridesMessages::internal_error,
         "A previously inactive SpiceEphemeris cannot be re-activated.");
   }

   return;
}


/**
 * Deactivate the SpiceEphemeris object.
 */
void
SpiceEphemeris::deactivate (
   void)
{
   inactive = true;

   return;
}


/**
 * Return time of last update.
 * @return Timestamp\n Units: day
 */
double
SpiceEphemeris::timestamp (
   void)
const
{
   return update_time;
}


/**
 * Return model name.
 * @return Name
 */
const char *
SpiceEphemeris::get_name (
   void)
const
{
   return ident.c_str();
}


/**
 * Initialize the SpiceEphemeris model.
 * This method is called before the planets have been registered
 * with the reference frame manager, so we don't know whether the
 * ephemeris items should be enabled or disabled.
 * \param[in] time_manager Time manager
 * \param[in] ephem_manager Incoming ephem manager
 */
void
SpiceEphemeris::initialize_model (
   const TimeManager & time_manager,
   EphemeridesManager & ephem_manager)
{

   // Nothing to do if the model is inactive.
   if (inactive) {
      return;
   }

   // Assign JEOD names to all the barycenter frames.
   name_barycenter_frames ();

   // Prevent SPICE from reporting error messages to instead route them to JEOD
   // for use in the MessageHandler model.
   mute_spice_errors ();

   // Get pointers to the ephemeris time and dynamic time time objects.
   initialize_time (time_manager);

   // Tell the EphemeridesManager that this is an active ephemeris model.
   ephem_manager.add_ephemeris (*this);

   // Set the local ephemeris manager pointer.
   ephem_mngr_local = &ephem_manager;

   // Load ephemeris items from SPICE.
   load_spice_files ();

   // Process the list of desired spk objects.
   process_spk ();

   // Process all planetary orientation frames.
   process_orientations();

   // Initialize the ephemeris items.
   initialize_items ();

   return;
}


/**
 * Set the SPICE model for a restart.
 */
void
SpiceEphemeris::simple_restore (
   void)
{
   return; // Nothing yet.
}


/**
 * Initialize SpiceEphemeris timing.
 * \param[in] time_manager Time manager
 */
void
SpiceEphemeris::initialize_time (
   const TimeManager & time_manager)
{

   // Set pointer to dynamic time seconds; note that TimeDyn is an actual
   // data member of the TimeManager class, simplifying the task of finding it.
   dyn_seconds = &(time_manager.dyn_time.seconds);

   // Test that an ephemeris time (TDB) object exists; if it does not, then
   // method time_manager.get_time_ptr("TDB") will return a NULL pointer.
   if ((time_manager.get_time_ptr ("TDB")) == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
         "Could not find the TDB Time object.");

      // Not reached.
      return;
   }

   // Time TDB does exist; set the pointer to its seconds field.
   tdb_seconds = &((time_manager.get_time_ptr ("TDB"))->seconds);

   return;
}


/**
 * Load SPICE kernel files containing ephemeris data.
 */
void
SpiceEphemeris::load_spice_files (
   void)
{

   // Call SPICE kernel file loader routine.
   furnsh_c (metakernel_filename.c_str());

   // Check whether SPICE returned an error message.
   if (failed_c()) {

      // Error message returned; obtain it from SPICE.
      char err_msg[MAX_MSG_LENGTH];
      getmsg_c ("long", MAX_MSG_LENGTH, err_msg);

      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::file_error,
         "While loading metakernels, furnsh_c reports the following error: %s\n",
         err_msg);


      // Not reached.
      return;
   }

   return;
}


/**
 * Process spk objects and store array of loaded spk IDs
 */
void
SpiceEphemeris::process_spk (
   void)
{
   // Get 'count', the number of SPICE spk kernels.
   int count = 0;
   ktotal_c ( "spk", &count);

   // If list of files to load was empty, notify user and exit.
   if (count == 0) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::item_not_in_file,
         "File of SPICE kernels to load is empty or incorrectly formatted.\n");

      // Not reached.
      return;
   }

   // Declare 'found_ids', a so-called cell -- a SPICE-defined variable type --
   // of integers which will contain an unsorted, cumulative list of all SPICE
   // IDs loaded. The IDs will then be extracted and sorted for use by JEOD.
   SPICEINT_CELL (found_ids, MAX_IDS);

   // Walk the list of all loaded SPICE files and from it populate
   // spk_object_ids, a sorted array of loaded spk IDs.
   std::list<int> spk_object_ids;
   unsigned int num_spk_objs_loaded = 0; // Will contain num of loaded objects
   int file_handle;
   int found;
   char file_name[MAX_PATH_LENGTH];
   char file_type[MAX_NAME_LENGTH];
   char loaded_from[MAX_PATH_LENGTH];

   for (int which = 0; which < count; which++) {

      // Get the next spk file in the set of loaded ones.
      kdata_c (which, "spk",
         MAX_PATH_LENGTH, MAX_NAME_LENGTH, MAX_PATH_LENGTH,
         file_name, file_type, loaded_from, &file_handle, &found);

      // Accumulate the list of all available ephemeris objects across all
      // spk files; spkobj_c() automatically appends to found_ids if it
      // already contains data.
      spkobj_c (file_name, &found_ids);
   }

   // Check how many objects were loaded from the kernels.
   num_spk_objs_loaded = card_c (&found_ids);

   // Extract the loaded IDs into a sortable list.
   for (unsigned int ii = 0; ii < num_spk_objs_loaded; ++ii) {
      SpiceInt which_id = SPICE_CELL_ELEM_I (&found_ids, ii);
      spk_object_ids.push_back(which_id);

      // Print extracted ID if debugging messages requested.
      MessageHandler::debug (
         __FILE__, __LINE__, EphemeridesMessages::debug,
         "Found object %d in SPICE file.\n", which_id);
   }

   spk_object_ids.sort();

   // Now check the list of planet names to make sure each has a corresponding
   // spk ID.

   // Print number of given planet names if debugging requested.
   MessageHandler::debug (
      __FILE__, __LINE__, EphemeridesMessages::debug,
      "Number of names to load: %lu\n", planet_names.size());

   // Walk list of specified objects to be loaded and try to find them in the
   // loaded SPICE files. If successful, load them.
   for (unsigned int ii = 0; ii < planet_names.size(); ++ii) {

      // Convert current name to uppercase for searching SPICE object list.
      std::string search_name = planet_names[ii];
      std::transform(search_name.begin(), search_name.end(), search_name.begin(), toupper);

      // Have SPICE convert the name to a standard SPICE ID number.
      int obj_id = 0; // The ID found, if one is found.
      int bool_found = 0; // Flag indicating success or failure of the search.
      bodn2c_c (search_name.c_str(), &obj_id, &bool_found);

      // Check whether SPICE recognized the specified object.
      if (!bool_found) {
         MessageHandler::fail (
            __FILE__, __LINE__, EphemeridesMessages::item_not_in_file,
            "Name %s is not known to SPICE\n", search_name.c_str());

         // Not reached.
         return;
      }

      // Now try to find the SPICE-provided ID in the list of loaded objects.
      bool found_id_in_list = false;
      for (std::list<int>::const_iterator it = spk_object_ids.begin();
           it != spk_object_ids.end();
           ++it) {
         if (obj_id == *it) {
            found_id_in_list = true;
            break;
         }
      }

      // Report failure and exit sim if ID not found.
      if (!found_id_in_list) {
         MessageHandler::fail (
            __FILE__, __LINE__, EphemeridesMessages::item_not_in_file,
            "ID %d corresponding to name %s was never loaded\n",
            obj_id, search_name.c_str());

         // Not reached.
         return;
      }

      // Print the object found if debugging messages requested.
      MessageHandler::debug (
         __FILE__, __LINE__, EphemeridesMessages::debug,
         "Current object to load: %s\n", planet_names[ii].c_str());


      // Found it! Create a corresponding SpiceEphemPoint and add it
      // to this model's list of points.
      loaded_spk.push_back (create_new_ephem_point (
         planet_names[ii], search_name));
   }

   return;
}


/**
 * Load and locate all planetary orientation frames.
 */
void
SpiceEphemeris::process_orientations (
   void)
{
   if (orientation_names.size() == 0) {
      return;
   }

   for (unsigned int ii = 0; ii < orientation_names.size(); ++ii) {
      planetary_orientations.push_back (create_new_ephem_orientation (
         orientation_names[ii]));
   }

   return;
}


/**
 * Populate basic attributes of a new SpiceEphemPoint.
 * \param[in,out] item Pointer to item to populate
 * \param[in] object_name Name of the item
 */
void
SpiceEphemeris::populate_item (
   EphemerisItem & item,
   const std::string & object_name)
{
   item.set_name(object_name.c_str(), item.default_suffix());
   item.set_owner(this);
   item.enable();

   return;
}


/**
 * Create a new SpiceEphemPoint.
 * @return Pointer to object
 * \param[in] object_name Name of new object
 * \param[in] spice_name SPICE lookup name
 */
SpiceEphemPoint *
SpiceEphemeris::create_new_ephem_point (
   std::string object_name,
   const std::string & spice_name)
{

   // Dynamically allocate new object.
   SpiceEphemPoint * new_point = JEOD_ALLOC_CLASS_OBJECT (SpiceEphemPoint, ());

   // Look up the SPICE ID number for the found object and store it.
   int found_id = 0; // ID number for the found object.
   int found_spicebool = 0; // Flag indicating search success or failure.

   bodn2c_c (spice_name.c_str(), &found_id, &found_spicebool);

   if (found_spicebool == 0) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::invalid_name,
         "Unable to find a SPICE ID for specified SpiceEphemPoint name.");

      // Not reached.
      return nullptr;
      }

   new_point->set_spice_id (found_id);
   new_point->set_parent_id (find_parent_id(found_id));

   // Set up other attributes of new SpiceEphemPoint.
   populate_item (*new_point, object_name);

   return new_point;
}


/**
 * Create a new SpiceEphemOrientation.
 * @return Pointer to object
 * \param[in] jeod_name Name of new object
 */
SpiceEphemOrientation *
SpiceEphemeris::create_new_ephem_orientation (
   std::string jeod_name)
{

   // Dynamically allocate and populate new object.
   SpiceEphemOrientation * new_orientation =
      JEOD_ALLOC_CLASS_OBJECT (SpiceEphemOrientation, ());
   populate_item (*new_orientation, jeod_name);
   new_orientation->set_spice_frame_name ( jeod_2_spice_pfix (jeod_name));
   new_orientation->validate (*tdb_seconds);

   return new_orientation;
}


/**
 * Initialize the SpiceEphemeris item data.
 */
void
SpiceEphemeris::initialize_items (
   void)
{

   // Create any barycenters needed based on SPICE objects requested.
   create_barycenters ();

   // Tell the EphemeridesManager about all of the ephemeris items.
   for (unsigned int ii = 0; ii < loaded_spk.size(); ++ii) {
      introduce_item (*(loaded_spk[ii]));
   }
   for (unsigned int ii = 0; ii < planetary_orientations.size(); ++ii) {
      introduce_item (*(planetary_orientations[ii]));
   }

   // Determine root point of the reference frame tree after all items loaded.
   determine_root_node ();

   // Protect for inability to find a root frame. This means no spk files have
   // been loaded, so no reference frame tree can be built.
   if (root_item == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::null_pointer,
         "Unable to find a frame tree root; try loading a SPICE spk file.");

      // Not reached.
      return;
   }

   return;
}


/**
 * Introduce an EphemerisItem to the EphemeridesManager.
 * \param[in] item Item to introduce
 */
void
SpiceEphemeris::introduce_item (
   EphemerisItem & item)
{

   // Request that the Ephemeris Manager add the item to its list of such.
   ephem_mngr_local->add_ephem_item (item);

   // Protect for inability to find target frame, just in case.
   if (item.get_target_frame() == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
         "Unable to find target frame for EphemerisItem '%s'",
         item.get_name());

      // Not reached.
      return;
   }

   return;
}


/**
 * Convert SPICE names to JEOD nomenclature.
 * @return JEOD-friendly name of a SPICE name
 * \param[in] spice_name Name of a SPICE object.
 */
std::string
SpiceEphemeris::spice_2_jeod (
   std::string spice_name)
{
   std::size_t cut_loc = spice_name.find_first_of(" ");
   std::string jeod_name = spice_name.substr(0,cut_loc);
   std::string::iterator it = jeod_name.begin();
   std::transform(it+1, jeod_name.end(), it+1, tolower);

   return jeod_name;
}


/**
 * Convert JEOD body name to SPICE pfix frame name.
 * @return name of pfix frame that SPICE associates with the given body. Handles high precision frames for Earth and Moon unless IAU is specified by user
 * \param[in] jeod_name Name of the JEOD body.
 */
std::string
SpiceEphemeris::jeod_2_spice_pfix (
   std::string jeod_name)
{

   std::transform (jeod_name.begin(), jeod_name.end(), jeod_name.begin(), toupper);
   if (jeod_name == "EARTH") {
      return "ITRF93";
   }
   if (jeod_name == "MOON") {
      return "MOON_PA";
   }

   return "IAU_" + jeod_name;
}


/**
 * Name all the planetary barycenters with their JEOD identifiers.
 */
void
SpiceEphemeris::name_barycenter_frames (
   void)
{

   // Set barycenter names for Solar System and Earth-Moon barycenters.
   barycenter_frames[0].set_name ("SSBary.inertial");
   barycenter_frames[3].set_name ("EMBary.inertial");

   // Note: barycenters for Mercury and Venus, which are numbered [1] and [2],
   // will never be needed. Skip them.

   // Name the other planetary system barycenters.
   for (unsigned int ii = 4; ii < 10; ++ii) {
      int found;
      char spice_name[MAX_NAME_LENGTH];
      std::string jeod_name;

      bodc2n_c (ii, MAX_NAME_LENGTH, spice_name, &found);
      jeod_name = spice_2_jeod( spice_name) + "Bary.inertial";
      barycenter_frames[ii].set_name (jeod_name.c_str());
   }

   return;
}


/**
 * Add a barycenter corresponding to the given SPICE ID.
 * \param[in] id id of barycenter to add
 */
void
SpiceEphemeris::add_barycenter (
   int id)
{

   // Find the JEOD name for the specified barycenter.
   char spice_name[MAX_NAME_LENGTH];
   std::string frame_name = barycenter_frames[id].get_name();
   std::size_t end_indx = frame_name.find_first_of (".");
   std::string jeod_name = frame_name.substr(0, end_indx);

   // Use SPICE to convert the given ID to a SPICE name.
   int found = 0;
   bodc2n_c (id, MAX_NAME_LENGTH, spice_name, &found);

   // Create the new barycenter and add it to the array of such.
   loaded_spk.push_back (create_new_ephem_point (jeod_name,
      spice_name));

   // Register associated frame with the ephemeris manager.
   if (ephem_mngr_local->find_ref_frame (frame_name.c_str()) == nullptr) {
      ephem_mngr_local->add_integ_frame (barycenter_frames[id]);
   }

   return;
}


/**
 * Check what if any barycenters need creating and do so. Any new
 * barycenters are added to the all_loaded_items list and registered
 * with the dynamics manager.
 */
void
SpiceEphemeris::create_barycenters (
   void)
{

   // Handle the trivial case in which zero or one object(s) have been loaded,
   // meaning that no barycenters need to be created and the sim will run
   // in either single-planet or empty-space mode.
   if (loaded_spk.size() < 2) {
      return;
   }

   // Assemble an std::set<int> of parent IDs from the loaded items.
   std::set<int> parents;

   for (unsigned int ii = 0; ii < loaded_spk.size(); ++ii) {
      parents.insert (loaded_spk[ii]->get_parent_id());
   }

   // Do we need SSBary?
   std::set<int>::iterator f = parents.find(0);

   if (f != parents.end()) {
      add_barycenter (0);
      parents.erase (f);
   }
   else {
      if (parents.size () > 1) {

         // SSBary needed because there is more than 1 planetary system.
         add_barycenter (0);
      }
   }

   // Add everything else.
   for (std::set<int>::iterator it = parents.begin();
          it != parents.end(); ++it) {
      add_barycenter (*it);
   }

   return;
}


/**
 * Determine which item should be the root of the ref frame tree.
 */
void
SpiceEphemeris::determine_root_node (
   void)
{

   // Determine the root point of the reference frame tree.

   // Nothing is extant: there is no root.
   if (loaded_spk.size() == 0) {
      root_item = nullptr;
      return;
   }

   // One item exists: That item is the root.
   else if (loaded_spk.size() == 1) {
      root_item = loaded_spk[0];
   }

   // Non-trivial root, i.e., a barycenter; find it.
   else {

      // Based on how function create_barycenters() works, if SSBary
      // exists in the list it has to be the root. If it doesn't, and
      // there is more than one loaded item (which resulted in a
      // barycenter having been created), then the single barycenter
      // extant has to be the root. Since the barycenters have SPICE IDs
      // 0 through 9, with SSBary being zero, the search can be neatly
      // accomplished by a single search loop which stops on the first
      // barycenter it finds.
      SpiceEphemPoint * found_bary = nullptr;
      for (int ii = 0; ii < 10; ++ii) {

         found_bary = find_spice_id (ii);
         if (found_bary != nullptr) {
            root_item = found_bary;
            break;
         }
      }
   }

   // Print the name of the root item if debugging requested.
   MessageHandler::debug (
      __FILE__, __LINE__, EphemeridesMessages::debug,
      "Root item determined to be %s.\n", root_item->get_name());

   // The root item does not need ephemeris; its state is
   // tautologically zero. This will also be enforced each timestep.
   root_item->initialize_state ();

   return;
}


/**
 * Complete the initialization process.
 * This method should be called after all other ephemeris models have
 * completed their basic initialization and after all planets have
 * registered themselves with the ephemeris manager.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
SpiceEphemeris::ephem_initialize (
   EphemeridesManager&)
{

   // This EphemerisInterface does not track EphemerisItem existence and
   // activation/selection as separate concepts, since it only creates
   // EphemerisItems when specified to do so by the user. Thus, this method
   // is empty for this model.

   return;
}


/**
 * Mark appropriate items in the model as active.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
SpiceEphemeris::ephem_activate (
   EphemeridesManager&)
{

   // This EphemerisInterface does not track EphemerisItem existence and
   // activation/selection as separate concepts, since it only creates
   // EphemerisItems when specified to do so by the user. Thus, this method
   // is empty for this model.

   return;
}


/**
 * Add all offspring from a given node of the ref frame tree.
 * \param[in,out] parent Frame to attach to
 */
void
SpiceEphemeris::add_descendants_r (
   SpiceEphemPoint * parent)
{

   for (unsigned int ii = 0; ii < loaded_spk.size(); ++ii) {

      // Skip SSBary since it has no parent.
      if (loaded_spk[ii]->get_spice_id() == 0) {
         continue;
      }

      // Recursively add the offspring of the given node to the tree.
      if (loaded_spk[ii]->get_parent_id() == parent->get_spice_id()) {

         // Indicate offspring currently being added if debugging requested.
         MessageHandler::debug (
            __FILE__, __LINE__, EphemeridesMessages::debug,
            "Adding %s to %s.\n",
            loaded_spk[ii]->get_target_frame()->get_name(),
            parent->get_target_frame()->get_name());

         // Do the add for the current frame.
         ephem_mngr_local->add_frame_to_tree (
            *(loaded_spk[ii]->get_target_frame()),
            parent->get_target_frame());

         // Request the children of the current node be added.
         add_descendants_r (loaded_spk[ii]);
      }
   }

   return;
}


/**
 * Construct the ephemeris model portions of the reference frame tree.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
SpiceEphemeris::ephem_build_tree (
   EphemeridesManager & ephem_manager)
{

   // Nothing to do if the model is inactive.
   if (inactive) {

      return;
   }

   // Remove all owned frames from the tree.
   for (unsigned int ii = 0; ii < loaded_spk.size(); ++ii) {
      EphemerisRefFrame * target_frame = loaded_spk[ii]->get_target_frame();
      target_frame->remove_from_parent();
   }

   // Determine root of the reference frame tree if don't already know it.
   // Note that should already know it.
   determine_root_node ();

   // Protect for inability to find a root frame. This means no spk files have
   // been loaded, so no reference frame tree can be built.
   if (root_item == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::null_pointer,
         "Unable to find a frame tree root; try loading a SPICE spk file.");

      // Not reached.
      return;
   }

   // Print the name of the root frame if debugging requested.
   MessageHandler::debug (
      __FILE__, __LINE__, EphemeridesMessages::debug,
      "Adding root %s.\n", root_item->get_target_frame()->get_name());

   // Build the tree with root_item as the root.
   ephem_manager.add_frame_to_tree (*(root_item->get_target_frame()), nullptr);
   add_descendants_r (root_item);

   return;
}


/**
 * Update ephemerides for subscribed items.
 */
void
SpiceEphemeris::ephem_update (
   void)
{


   // Nothing to do if
   // - the model isn't active or
   // - neither time nor the tree has changed.
   if ((inactive) ||
         ((! force_update) && (Numerical::compare_exact(update_time, *dyn_seconds)))) {
      return;
   }

   update_time = *dyn_seconds;
   force_update = false;

   update_trans ();
   update_rot ();

   return;
}


/**
 * Find a SPICE ID in the loaded list of SPICE objects.
 * @return Pointer to object
 * \param[in] id_to_find SPICE lookup name
 */
SpiceEphemPoint *
SpiceEphemeris::find_spice_id (
   int id_to_find)
{

   SpiceEphemPoint * found_object = nullptr;
   for (unsigned int ii = 0; ii < loaded_spk.size(); ++ii) {

      if (loaded_spk[ii]->get_spice_id() == id_to_find) {

         found_object = loaded_spk[ii];
      }
   }

   return found_object;
}


/**
 * Find the ID of the parent of a given SPICE object. Here "parent"
 * means both:
 * 1. the frame to which the object is connected in the JEOD reference
 *    frame tree (the "parent" frame).
 * 2. the object that will be used as the "observer" in the calls to
 *    SPICE to obtain the state of the given object.
 * @return of parent\n Units: ID
 * \param[in] obj_id child ID of which parent is to be found
 */
int
SpiceEphemeris::find_parent_id (
   int obj_id)
{

   // Objects with SPICE IDs greater than 999 are minor planets,
   // comets, asteroids, etc. Their parent should be SSBary.
   if (obj_id > 999) {
      return 0;
   }

   // Mercury and Venus are moonless, so their planetary systems have no
   // distinct barycenter. Thus, their parent in the JEOD ref frame tree
   // sense is the SSBary.
   if (obj_id == 199 || obj_id == 299) {
      return 0;
   }

   // Everything else must be part of a non-trivial planetary system. Return
   // the planetary barycenter. This also works for the sun.
   return obj_id/100;
}


/**
 * Update ephemerides of inertial frames supplied by spk files.
 */
void
SpiceEphemeris::update_trans (
   void)
{
   const double null_vec[3] = {0.0, 0.0, 0.0};

   // Update the root if it is owned by this model.
   if (root_item != nullptr) {
      root_item->update (null_vec, null_vec, update_time);
   }

   // Perform updates of nodes with respect to their parents.
   for (unsigned int ii = 0; ii < loaded_spk.size(); ++ii) {

      // Skip root
      if (loaded_spk[ii] == root_item) {
         continue;
      }

      // Call to SPICE update function.
      double state[6], position[3], velocity[3], light_time;
      spkez_c (loaded_spk[ii]->get_spice_id(),
               *tdb_seconds,
               "J2000", "NONE",
               loaded_spk[ii]->get_parent_id(),
               state,
               &light_time);

      // Check whether SPICE returned an error message.
      if (failed_c()) {

         // Error message returned; obtain it from SPICE.
         char err_msg[MAX_MSG_LENGTH];
         getmsg_c ("long", MAX_MSG_LENGTH, err_msg);

         MessageHandler::fail (
            __FILE__, __LINE__, EphemeridesMessages::item_not_in_file,
            "Regarding ref frame %s, spkez_c reports the following error: %s\n",
            loaded_spk[ii]->get_target_frame()->get_name(), err_msg);

         // Not reached
         return;
      }

      // Store off state for reference frame update.
      for (unsigned int jj = 0; jj < 3; ++jj) {
         position[jj] = state[jj];
         velocity[jj] = state[3+jj];
      }

      // Tell ephemeris item to update its target frame. Note that the scale
      // factor is to convert the output from SPICE into meters.
      loaded_spk[ii]->update_scaled(position, velocity, 1000.0, update_time);
   }

   return;
}


/**
 * Update planetary orientations.
 */
void
SpiceEphemeris::update_rot (
   void)
{

   for (unsigned int ii = 0; ii < planetary_orientations.size(); ++ii) {
      planetary_orientations[ii]->update (*tdb_seconds, *dyn_seconds);
   }

   return;
}


/**
 * Mute SPICE errors so that they can be handled by the MessageHandler.
 */
void
SpiceEphemeris::mute_spice_errors (
   void)
{

   // Set the default SPICE action upon encountering an error to be to
   // return the error but not abort.
   char new_value [MAX_NAME_LENGTH];
   strcpy (new_value, "return");
   erract_c ("set", MAX_NAME_LENGTH, new_value);

   // Set the default SPICE error report target to NULL rather than the
   // screen. The SPICE-internal message will then be fed exclusively to
   // the JEOD MessageHandler.
   strcpy (new_value, "null");
   errdev_c ("set", MAX_NAME_LENGTH, new_value);

   // Set the default SPICE error report length to be middle-sized. These
   // will then be fed to the JEOD MessageHandler.
   strcpy (new_value, "explain");
   errprt_c ("set", MAX_NAME_LENGTH, new_value);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
