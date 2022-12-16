/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup De4xxEphem
 * @{
 *
 * @file models/environment/ephemerides/de4xx_ephem/src/de4xx_ephem.cc
 * Define the methods of the classes defined in de4xx_ephem.hh.
 */

/*******************************************************************************

Purpose:
()

Reference:
(((TBS)))

Assumptions and limitations:
((The JPL ephemeris model uses a time frame of their own development as the
basis for time. This time scale exhibits the same mean tick rate as does
Terrestrial Time, and differs from Terrestrial Time by no more than two
milliseconds. This interface to the JPL ephemeris model uses Terrestrial
Time as input and ignores the small periodic variations between T_eph and
TT.))

Class:
(N/A)

LIBRARY DEPENDENCY:
((de4xx_ephem.o)
(de4xx_ephem_dynmanager.o)
(de4xx_file.o)
(de4xx_file_init.o)
(de4xx_file_update.o)
(environment/ephemerides/ephem_interface/ephem_messages.o)
(environment/ephemerides/ephem_interface/ephem_ref_frame.o)
(environment/ephemerides/ephem_item/ephem_item.o)
(environment/ephemerides/ephem_item/ephem_orient.o)
(environment/ephemerides/ephem_item/ephem_orient_zxz.o)
(environment/ephemerides/ephem_item/ephem_point.o)
(environment/ephemerides/ephem_manager/ephem_manager.o)
(environment/time/time.o)
(environment/time/time_dyn.o)
(environment/time/time_manager.o)
(environment/time/time_tt.o)
(utils/memory/memory_manager_static.o)
(utils/message/message_handler.o)
(utils/named_item/named_item.o)
(utils/ref_frames/ref_frame.o)
(utils/ref_frames/ref_frame_manager.o)
(utils/ref_frames/ref_frame_set_name.o)
(utils/ref_frames/subscription.o))


*******************************************************************************/


// System includes
#include <cstddef>
#include <cstdio>
#include <climits>
#include <sstream>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"
#include "environment/planet/include/base_planet.hh"
#include "environment/time/include/time_manager.hh"
#include "environment/time/include/time_tt.hh"
#include "environment/time/include/time_dyn.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/math/include/numerical.hh"

// Model includes
#include "../include/de4xx_ephem.hh"


//! Namespace jeod
namespace jeod {

/**
 * De4xxEphemItem default constructor.
 */
De4xxEphemItem::De4xxEphemItem (
   void)
:
   item(NULL),
   enabled_item(NULL),
   frame(NULL),
   name(),
   status(Deselected),
   index(UINT_MAX)
{
   ; // Empty (intialization list does all)
}


/**
 * De4xxEphemItem destructor.
 */
De4xxEphemItem::~De4xxEphemItem (
   void)
{
   ; // Empty
}



/**
 * De4xxEphemeris default constructor.
 */
De4xxEphemeris::De4xxEphemeris (
   void)
:
   active(true),
   file(),
   force_update(false),
   nactive_items(0),
   ident(NULL),
   update_time(-99e99),
   root_item(NULL),
   time_tt(NULL),
   time_dyn(NULL)
{
   item_data = JEOD_ALLOC_CLASS_ARRAY( De4xxBase::number_jeod_items(999), De4xxEphemItem );

   // Set the mapping from De4xxEphemBodies numbers to De4xxFileBodies numbers.
   body_to_file_idx = JEOD_ALLOC_PRIM_ARRAY(
                          De4xxBase::number_jeod_items(999),
                          int );
   body_to_file_idx[De4xxBase::De4xx_Ephem_Sun] =
      De4xxBase::De4xx_File_Sun;
   body_to_file_idx[De4xxBase::De4xx_Ephem_Mercury] =
      De4xxBase::De4xx_File_Mercury;
   body_to_file_idx[De4xxBase::De4xx_Ephem_Venus] =
      De4xxBase::De4xx_File_Venus;
   body_to_file_idx[De4xxBase::De4xx_Ephem_Mars] =
      De4xxBase::De4xx_File_Mars;
   body_to_file_idx[De4xxBase::De4xx_Ephem_Jupiter] =
      De4xxBase::De4xx_File_Jupiter;
   body_to_file_idx[De4xxBase::De4xx_Ephem_Saturn] =
      De4xxBase::De4xx_File_Saturn;
   body_to_file_idx[De4xxBase::De4xx_Ephem_Uranus] =
      De4xxBase::De4xx_File_Uranus;
   body_to_file_idx[De4xxBase::De4xx_Ephem_Neptune] =
      De4xxBase::De4xx_File_Neptune;
   body_to_file_idx[De4xxBase::De4xx_Ephem_Pluto] =
      De4xxBase::De4xx_File_Pluto;
   body_to_file_idx[De4xxBase::De4xx_Ephem_Moon] =
      De4xxBase::De4xx_File_Moon;
   body_to_file_idx[De4xxBase::De4xx_Ephem_EMbary] =
      De4xxBase::De4xx_File_EMbary;

   body_to_file_idx[De4xxBase::De4xx_Ephem_ENutation] =
      De4xxBase::De4xx_File_ENutation;
   body_to_file_idx[De4xxBase::De4xx_Ephem_LLibration] =
      De4xxBase::De4xx_File_LLibration;

   // Earth, the solar system barycenter, and the Earth-Moon L1 point are not
   // represented in a DE 4xx file, they must be derived.
   const De4xxBase::De4xxFileEntries out_of_bounds = De4xxBase::De4xx_File_MaxEntries;
   body_to_file_idx[De4xxBase::De4xx_Ephem_Earth]  = out_of_bounds;
   body_to_file_idx[De4xxBase::De4xx_Ephem_SSbary] = out_of_bounds;
   body_to_file_idx[De4xxBase::De4xx_Ephem_EML1]   = out_of_bounds;


   // Initialize the solar system and earth-moon barycenter reference frames.
   // ephemeris points, and item data.
   solar_system_barycenter_frame.set_name  ("SSBary.inertial");
   earth_moon_barycenter_frame.set_name  ("EMBary.inertial");

   // Initialize the EphemerisPoint objects that describe the sun, the planets,
   // and the solar system and earth-moon barycenter.
   points = JEOD_ALLOC_CLASS_ARRAY( De4xxBase::number_trans_points(999), EphemerisPoint );
   for (uint32_t ii = 0; ii < De4xxBase::number_trans_points(999); ++ii) {
      points[ii].set_name  (De4xxBase::point_names[ii], "inertial");
      points[ii].set_owner (this);
      points[ii].enable  ();

      item_data[ii].item = &points[ii];
      item_data[ii].name = points[ii].get_name();
      item_data[ii].index = ii;
   }

   // Initialize the lunar orientation.
   lunar_orientation.set_name  ("Moon", "pfix");
   lunar_orientation.set_owner (this);
   lunar_orientation.enable  ();
   item_data[De4xxBase::De4xx_Ephem_LLibration].item = &lunar_orientation;
   item_data[De4xxBase::De4xx_Ephem_LLibration].name =
      lunar_orientation.get_name();
   item_data[De4xxBase::De4xx_Ephem_LLibration].index =
      De4xxBase::De4xx_Ephem_LLibration;

   // Initialize the selection array.
   selected_items = JEOD_ALLOC_PRIM_ARRAY( De4xxBase::number_jeod_items(999), bool );
   for (uint32_t ii = 0; ii < De4xxBase::number_jeod_items(999); ++ii) {
      selected_items[ii] = true;
   }

   // Nutations and the Earth-Moon L1 point currently are not supported by JEOD
   selected_items[De4xxBase::De4xx_Ephem_ENutation] = false;
   selected_items[De4xxBase::De4xx_Ephem_EML1] = false;

}


/**
 * De4xxEphemeris destructor.
 */
De4xxEphemeris::~De4xxEphemeris (
   void)
{
   shutdown ();
   JEOD_DELETE_ARRAY(body_to_file_idx);
   JEOD_DELETE_ARRAY(selected_items);
   JEOD_DELETE_ARRAY(item_data);
   JEOD_DELETE_ARRAY(points);
}


/**
 * Free resources allocated by the De4xxEphemeris model.
 */
void
De4xxEphemeris::shutdown (
   void)
{
   if (ident != NULL) {
      JEOD_DELETE_ARRAY (ident);
   }
   file.shutdown ();

   return;
}


/**
 * Nominally, activate the object.
 * In the case of a De4xxEphemeris object,
 * an inactive object cannot be activated.
 */
void
De4xxEphemeris::activate (
   void)
{
   if (! active) {
      MessageHandler::error (
         __FILE__, __LINE__, EphemeridesMessages::internal_error,
         "A previously inactive De4xxEphemeris cannot be re-activated.");
   }

   return;
}


/**
 * Deactivate the De4xxEphemeris object.
 */
void
De4xxEphemeris::deactivate (
   void)
{
   active = false;

   return;
}


/**
 * Return time of last update.
 * @return Timestamp\n Units: day
 */
double
De4xxEphemeris::timestamp (
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
De4xxEphemeris::get_name (
   void)
const
{
   return ident;
}


/**
 * Initialize the De4xxEphemeris model.
 * This method is called before the planets have been registered
 * with the reference frame manager, so we don't know whether the
 * ephemeris items should be enabled or disabled.
 * \param[in] time_manager Time manager
 * \param[in,out] ephem_manager Ephemerides manager
 * \param[in] time_type optional "tt"||"tdb" "tt" default manager
 */
void
De4xxEphemeris::initialize_model (
   const TimeManager & time_manager,
   EphemeridesManager & ephem_manager,
   std::string time_type)
{

   // Nothing to do if the model is inactive.
   if (! active) {
      return;
   }

   // Get pointers to the ephemeris time and dynamic time time objects.
   initialize_time (time_manager, time_type);

   // Initialize the De4xxFile model.
   initialize_file ();

   // Tell the EphemeridesManager that this is an active ephemeris model.
   ephem_manager.add_ephemeris (*this);

   // Initialize the ephemeris items.
   initialize_items (ephem_manager);
}


/**
 * Initialize De4xxEphemeris timing.
 * \param[in] time_manager Time manager
 * \param[in] time_type time type
 */
void
De4xxEphemeris::initialize_time (
   const TimeManager & time_manager,
   std::string time_type)
{

   // Get pointers to the ephemeris time and dynamic time time objects.
   if (time_type == "TDB") {
      time_tt =
         dynamic_cast<const TimeStandard *> (time_manager.get_time_ptr ("TDB"));
   }
   else {
      if (time_type != "TT") {
         MessageHandler::warn (
            __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
            "Time type %s not allowed.  Only choices are TT and TDB",
            time_type.c_str());
      }
      time_tt  =
         dynamic_cast<const TimeStandard *> (time_manager.get_time_ptr ("TT"));
   }

   time_dyn = dynamic_cast<const TimeDyn *> (time_manager.get_time_ptr ("Dyn"));

   // Check for valid time types
   if ((time_tt == NULL) || (time_dyn == NULL)) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
         "Could not find the Terrestrial Time and Dynamic Time time objects.");

      // Not reached
      return;
   }
}


/**
 * Initialize the De4xxEphemeris file.
 */
void
De4xxEphemeris::initialize_file (
   void)
{
   double epoch_time;                  // day Julian date
   double time_offset;                 // s   Terrestrial Time offset
   double init_time;                   // day Days from epoch


   // Determine the time offsets.
   // FIXME: Switch to J2000
   // FIXME: Use accessors.
   epoch_time  = time_tt->tjt_jd_offset;
   init_time   = time_tt->trunc_julian_time;
   time_offset = 0.0;

   // Initialize the De4xxFile model.
   file.initialize (epoch_time, 0.0, time_offset, init_time * 86400.0);

   // Construct the identifier for this model.
   {
      std::stringstream denum;
      std::stringstream lenum;
      denum << static_cast<int>(file.io.metaData->de_constants[De4xxBase::De4xx_Const_DENUM]);
      lenum << static_cast<int>(file.io.metaData->de_constants[De4xxBase::De4xx_Const_LENUM]);
      
      ident = NamedItem::construct_name("DE", denum.str().c_str(), "LE", lenum.str().c_str());
   }
}


/**
 * Initialize the De4xxEphemeris item data.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
De4xxEphemeris::initialize_items (
   EphemeridesManager & ephem_manager)
{

   // Process the user-inputable selected_items array.
   for (uint32_t ii = 0; ii < De4xxBase::number_jeod_items(file.file_spec.get_model_number()); ++ii) {
      // Mark deselected items (and items that are not present) as deselected.
      if ((! selected_items[ii]) || (item_data[ii].item == NULL)) {
         if (item_data[ii].item != NULL) {
            item_data[ii].item->disable ();
         }
         item_data[ii].status = De4xxEphemItem::Deselected;
      }

      // Initially mark selected items as inactive.
      // This status will be updated later.
      else {
         item_data[ii].status = De4xxEphemItem::Inactive;
      }
   }

   // Sanity check: The Earth and Moon must be set by the same model.
   if (item_data[De4xxBase::De4xx_Ephem_Earth].status !=
         item_data[De4xxBase::De4xx_Ephem_Moon].status) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
         "The Earth and Moon must be set by the same model.");
   }

   // Tell the EphemeridesManager about each selected ephemeris item.
   // While the constructor enables each modeled ephemeris item,
   // these can be turned off using the selected_items data member.

   // Register the solar system and Earth-Moon barycenter frames with the
   // EphemeridesManager.
   if ((item_data[De4xxBase::De4xx_Ephem_SSbary].status !=
         De4xxEphemItem::Deselected) &&
         (ephem_manager.find_ref_frame (
             solar_system_barycenter_frame.get_name()) == NULL)) {
      ephem_manager.add_integ_frame (solar_system_barycenter_frame);
   }

   if ((item_data[De4xxBase::De4xx_Ephem_EMbary].status !=
         De4xxEphemItem::Deselected) &&
         (ephem_manager.find_ref_frame (
             earth_moon_barycenter_frame.get_name()) == NULL)) {
      ephem_manager.add_integ_frame (earth_moon_barycenter_frame);
   }

   // Tell the EphemeridesManager about the ephemeris points.
   for ( uint32_t ii = 0;
         ii < De4xxBase::number_trans_points(file.file_spec.get_model_number());
         ++ii)
   {
      if (item_data[ii].status != De4xxEphemItem::Deselected) {
         ephem_manager.add_ephem_item (points[ii]);
      }
   }

   // Finally register the lunar orientation ("lunar RNP").
   if (item_data[De4xxBase::De4xx_Ephem_LLibration].status !=
         De4xxEphemItem::Deselected) {
      ephem_manager.add_ephem_item (lunar_orientation);
   }
}


/**
 * Complete the initialization process.
 * This method should be called after all other ephemeris models have
 * completed their basic initialization and after all planets have
 * registered themselves with the ephemeris manager.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
De4xxEphemeris::ephem_initialize (
   EphemeridesManager & ephem_manager JEOD_UNUSED)
{

   // Nothing to do if the model is inactive.
   if (! active) {
      return;
   }

   // Get item target frames and deselect items that do not have a target frame.
   for (uint32_t ii = 0; ii < De4xxBase::number_jeod_items(file.file_spec.get_model_number()); ++ii) {

      // Process all items that currently are handled by the model.
      if (item_data[ii].item != NULL) {

         // If the item is in the simulation it will (must) have a target
         // frame by this point.
         // (If not there is a problem with initialization job sequencing.)
         item_data[ii].frame = item_data[ii].item->get_target_frame();

         // Deselect items that don't have a target frame.
         if (item_data[ii].frame == NULL) {
            item_data[ii].status = De4xxEphemItem::Deselected;
         }
      }

      // Item is not part of the model: Deselect
      // Note: This should already by the case, but for completeness ...
      else {
         item_data[ii].status = De4xxEphemItem::Deselected;
      }
   }
}


/**
 * Mark appropriate items in the model as active.
 * @return Void
 */
unsigned int
De4xxEphemeris::activate_nodes (
   void)
{
   unsigned int tot_active;

   // Get the basic status for each item.
   tot_active = 0;
   nactive_items = 0;
   for (uint32_t ii = 0; ii < De4xxBase::number_jeod_items(file.file_spec.get_model_number()); ++ii) {
      De4xxEphemItem & item = item_data[ii];
      if (item.status != De4xxEphemItem::Deselected) {
         item.enabled_item = item.item->get_enabled_item ();
         if ((item.enabled_item == NULL) ||
               (! item.enabled_item->is_active ())) {
            item.status = De4xxEphemItem::Inactive;
         }
         else if (item.enabled_item != item.item) {
            item.status = De4xxEphemItem::InTree;
         }
         else {
            item.status = De4xxEphemItem::Active;
            nactive_items++;
         }
      }
      else {
         item.enabled_item = NULL;
      }

      if ((ii < De4xxBase::number_trans_points(file.file_spec.get_model_number())) &&
            (item.status > De4xxEphemItem::Inactive)) {
         tot_active++;
      }
   }

   return tot_active;
}


/**
 * Adjust Earth, Moon, and Earth-Moon barycenter activity.
 * @return Void
 * \param[in] tot_active Number active translation nodes
 */
unsigned int
De4xxEphemeris::activate_em_nodes (
   unsigned int tot_active)
{
   static const De4xxBase::De4xxEphemBodies embodies[3] = {
      De4xxBase::De4xx_Ephem_Earth,
      De4xxBase::De4xx_Ephem_Moon,
      De4xxBase::De4xx_Ephem_EMbary
   };
   unsigned int embcount;


   // Count how many of the Earth, Moon, and Earth-Moon barycenter are active.
   embcount = 0;
   for (unsigned int ii = 0; ii < 3; ++ii) {
      De4xxEphemItem & item = item_data[embodies[ii]];
      if (item.status > De4xxEphemItem::Inactive) {
         embcount++;
      }
   }

   // Make all three of the Earth, Moon, and Earth-Moon barycenter active
   // if any two of them are active OR
   // if only the Earth or Moon is active and
   //  some other solar system body is active.
   if ((embcount == 2) ||
         ((embcount == 1) &&
          (item_data[De4xxBase::De4xx_Ephem_EMbary].status <=
           De4xxEphemItem::Inactive) &&
          (tot_active > 1))) {

      for (unsigned int ii = 0; ii < 3; ++ii) {
         De4xxEphemItem & item = item_data[embodies[ii]];
         if (item.status == De4xxEphemItem::Deselected) {
            MessageHandler::fail (
               __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
               "Item '%s' needs to be activated but is deselected",
               item.name);
         }
         else if (item.enabled_item == NULL) {
            item.enabled_item = item.item;
            item.item->enable();
            item.item->activate();
            item.status = De4xxEphemItem::Active;
            nactive_items++;
            tot_active++;
         }
         else if (item.status == De4xxEphemItem::Inactive) {
            item.enabled_item->activate();
            if (item.enabled_item != item.item) {
               item.status = De4xxEphemItem::InTree;
            }
            else {
               item.status = De4xxEphemItem::Active;
               nactive_items++;
            }
            tot_active++;
         }
      }
   }

   return tot_active;
}


/**
 * Determine which item should be the root of the ref frame tree.
 */
void
De4xxEphemeris::determine_root_node (
   void)
{
   int root_index;
   unsigned int active_nodes[De4xxBase::number_trans_points(file.file_spec.get_model_number())];
   unsigned int tot_active;


   // Build a list of active planetary nodes.
   tot_active = 0;
   for (uint32_t ii = 0; ii < De4xxBase::number_trans_points(file.file_spec.get_model_number()); ++ii) {
      if (item_data[ii].status > De4xxEphemItem::Inactive) {
         active_nodes[tot_active++] = ii;
      }
   }

   // Determine the root point of the reference frame tree.
   // Nothing is active: there is no root.
   if (tot_active == 0) {
      root_index = -1;
   }

   // One item active: That active item is the root.
   else if (tot_active == 1) {
      root_index = active_nodes[0];
   }

   // Activity is within the Earth-Moon barycenter: EMbary is the root.
   else if ((tot_active == 3) &&
            (item_data[De4xxBase::De4xx_Ephem_EMbary].status >
             De4xxEphemItem::Inactive) &&
            (item_data[De4xxBase::De4xx_Ephem_Earth].status >
             De4xxEphemItem::Inactive) &&
            (item_data[De4xxBase::De4xx_Ephem_Moon].status >
             De4xxEphemItem::Inactive)) {
      root_index = De4xxBase::De4xx_Ephem_EMbary;
   }

   // All other cases: The root is the solar system barycenter.
   else {
      root_index = De4xxBase::De4xx_Ephem_SSbary;
   }


   // The root item does not need ephemeris.
   // Its state is tautologically zero.
   if (root_index >= 0) {
      root_item = &item_data[root_index];
      if (root_item->item == root_item->enabled_item) {
         (dynamic_cast<EphemerisPoint*>(root_item->item))->initialize_state ();
         nactive_items--;
      }
      root_item->status = De4xxEphemItem::IsRoot;
   }

   else {
      root_item = NULL;
   }
}


/**
 * Mark appropriate items in the model as active.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
De4xxEphemeris::ephem_activate (
   EphemeridesManager & ephem_manager JEOD_UNUSED)
{
   unsigned int tot_active;

   // Nothing to do if the model is inactive.
   if (! active) {
      return;
   }

   // Activate nodes based on the activity of the ephemeris points and
   // based on the DE4xx reference frame tree.
   tot_active = activate_nodes ();
   tot_active = activate_em_nodes (tot_active);

   // Determine the root point of the reference frame tree.
   determine_root_node ();


   // Count the number of active frames that belong to this model.
   nactive_items = 0;
   for (uint32_t ii = 0; ii < De4xxBase::number_jeod_items(file.file_spec.get_model_number()); ++ii) {
      if (item_data[ii].status == De4xxEphemItem::Active) {
         nactive_items++;
      }
   }


   // Synchronize the file activity with this model's activity.
   for (int ii = De4xxBase::De4xx_Ephem_Sun;
         ii <= De4xxBase::De4xx_Ephem_EMbary;
         ++ii) {
      De4xxBase::De4xxFileEntries file_id = (De4xxBase::De4xxFileEntries)body_to_file_idx[ii];
      if (file_id < file.io.metaData->number_file_items) {
         file.item[file_id].active =
            (item_data[ii].status == De4xxEphemItem::Active);
      }
   }

   // Librations = lunar orientation.
   file.item[De4xxBase::De4xx_File_LLibration].active =
      (item_data[De4xxBase::De4xx_Ephem_LLibration].status ==
       De4xxEphemItem::Active);

   // This model doesn't use the nutations.
   file.item[De4xxBase::De4xx_File_ENutation].active = false;

   // Force an update to reflect the change in status.
   force_update = true;

   return;
}


/**
 * Construct the ephemeris model portions of the reference frame tree.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void
De4xxEphemeris::ephem_build_tree (
   EphemeridesManager & ephem_manager)
{

   // Nothing to do if the model is inactive.
   if (! active) {
      return;
   }

   // Remove all owned frames from the tree.
   for ( uint32_t ii = 0;
         ii < De4xxBase::number_trans_points(file.file_spec.get_model_number());
         ++ii)
   {
      if ((item_data[ii].status != De4xxEphemItem::Deselected) &&
            (item_data[ii].item == item_data[ii].enabled_item)) {
         item_data[ii].frame->remove_from_parent ();
      }
   }

   // Set the root of the reference frame tree.
   if ((root_item != NULL) &&
         (root_item->item == root_item->enabled_item)) {
      ephem_manager.add_frame_to_tree (*(root_item->frame), NULL);
   }

   // Add non-root items to the tree.
   for ( uint32_t ii = 0;
         ii < De4xxBase::number_trans_points(file.file_spec.get_model_number());
         ++ii)
   {
      De4xxEphemItem & item = item_data[ii];
      EphemerisRefFrame * parent_frame;
      if (item.status == De4xxEphemItem::Active) {
         if ((ii == De4xxBase::De4xx_Ephem_Earth) ||
               (ii == De4xxBase::De4xx_Ephem_Moon)) {
            parent_frame = item_data[De4xxBase::De4xx_Ephem_EMbary].frame;
         }
         else {
            parent_frame = item_data[De4xxBase::De4xx_Ephem_SSbary].frame;
         }
         if (parent_frame == NULL) {
            MessageHandler::fail (
               __FILE__, __LINE__, EphemeridesMessages::inconsistent_setup,
               "Unable to find parent frame for '%s'",
               item.name);

            // Not reached
            return;
         }
         ephem_manager.add_frame_to_tree (*(item.frame), parent_frame);
      }
   }
}


/**
 * Update ephemerides for subscribed items.
 */
void
De4xxEphemeris::ephem_update (
   void)
{
   static const double null_vec[3] = {0.0, 0.0, 0.0};

   // Nothing to do if
   // - the model isn't active or
   // - neither time nor the tree has changed.
   if ((! active) ||
         ((! force_update) && (Numerical::compare_exact(update_time,time_dyn->seconds)))) {
      return;
   }

   update_time = time_dyn->seconds;
   force_update = false;

   // Update the root if it is owned by this model.
   if ((root_item != NULL) &&
         (root_item->item == root_item->enabled_item)) {
      EphemerisPoint * point = dynamic_cast<EphemerisPoint*>(root_item->item);
      point->update (null_vec, null_vec, update_time);
   }

   if (nactive_items > 0) {

      // Update the ephemeris file to reflect the current ephemeris time.
      // FIXME: Switch to J2000
      file.update (time_tt->trunc_julian_time * 86400.0);

      // Extract the requisite data directly represented in the ephemeris file
      // from the ephemeris file object.
      // Note: The Earth and Moon need special treatment (see below).
      for (unsigned int ii = De4xxBase::De4xx_Ephem_Sun;
            ii <= De4xxBase::De4xx_Ephem_EMbary;
            ++ii) {

         // Point is directly represented and is needed:
         // Update state based on  the ephemeris file data for this object.
         if ((ii != De4xxBase::De4xx_Ephem_Earth) &&
               (ii != De4xxBase::De4xx_Ephem_Moon) &&
               (item_data[ii].status == De4xxEphemItem::Active)) {
            EphemerisPoint * point =
               dynamic_cast<EphemerisPoint*>(item_data[ii].item);
            De4xxFileItem & file_item = file.item[body_to_file_idx[ii]];
            point->update (file_item.state[0], file_item.state[1], update_time);
         }
      }

      // Compute Earth and Moon states to reflect the ref frame tree structure.
      // The Moon state is relative to the Earth in the ephemeris file.
      if ((item_data[De4xxBase::De4xx_Ephem_Earth].status ==
            De4xxEphemItem::Active) &&
            (item_data[De4xxBase::De4xx_Ephem_Moon].status ==
             De4xxEphemItem::Active)) {

         De4xxFileItem & moon_ephem =
            file.item[body_to_file_idx[De4xxBase::De4xx_Ephem_Moon]];

         points[De4xxBase::De4xx_Ephem_Earth].update_scaled (
            moon_ephem.state[0], moon_ephem.state[1],
            -file.header.be_em_dist_ratio, update_time);

         points[De4xxBase::De4xx_Ephem_Moon].update_scaled (
            moon_ephem.state[0], moon_ephem.state[1],
            file.header.bm_em_dist_ratio, update_time);
      }

      // Lunar orientation needed:
      // Extract the lunar orientation Euler angles from the ephemeris file
      // and update the lunar orientation.
      if (item_data[De4xxBase::De4xx_Ephem_LLibration].status ==
            De4xxEphemItem::Active) {
         De4xxFileItem & file_angles =
            file.item[De4xxBase::De4xx_File_LLibration];
         lunar_orientation.update (file_angles.state[0], file_angles.state[1],
                                   update_time);
      }
   }
}


/**
 * Check whether the specified time is represented in the JPL ephemeris file.
 *
 * \par Assumptions and Limitations
 *  - Ephemeris file is open for input
 *  - Ephemeris file is blocked per value set in the ephem_file structure
 * @return True if time is in file
 */
bool
De4xxEphemeris::time_is_in_range (
   void)
const
{
   return file.time_is_in_range (time_tt->trunc_julian_time * 86400.0);
}


/**
 * Propagate the lunar orientation to the current time.
 */
void
De4xxEphemeris::propagate_lunar_rnp (
   void)
{

   if ((! active) ||
         (item_data[De4xxBase::De4xx_Ephem_LLibration].status !=
          De4xxEphemItem::Active)) {
      return;
   }

   lunar_orientation.propagate (time_dyn->seconds);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
