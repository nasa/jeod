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
 * @file models/environment/ephemerides/ephem_manager/src/ephem_manager.cc
 * Define EphemeridesManager methods.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((ephem_manager.cc)
   (find_planet.cc)
   (environment/ephemerides/ephem_interface/src/ephem_ref_frame.cc)
   (environment/ephemerides/ephem_interface/src/ephem_messages.cc)
   (environment/ephemerides/ephem_item/src/ephem_item.cc)
   (environment/ephemerides/ephem_item/src/ephem_orient.cc)
   (environment/ephemerides/ephem_item/src/ephem_point.cc)
   (environment/planet/src/base_planet.cc)
   (utils/message/src/message_handler.cc))


******************************************************************************/

// System includes
#include <algorithm>
#include <cstddef>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/ephemerides/ephem_item/include/ephem_item.hh"
#include "environment/ephemerides/ephem_item/include/ephem_orient.hh"
#include "environment/ephemerides/ephem_item/include/ephem_point.hh"
#include "environment/planet/include/base_planet.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame.hh"

// Model includes
#include "../include/ephem_manager.hh"

//! Namespace jeod
namespace jeod
{

/**
 * EphemeridesManager default constructor.
 */
EphemeridesManager::EphemeridesManager()
    : BaseEphemeridesManager(),
      RefFrameManager()
{
    JEOD_REGISTER_CLASS(EphemeridesManager);
    JEOD_REGISTER_CLASS(BasePlanet);
    JEOD_REGISTER_CLASS(EphemerisInterface);
    JEOD_REGISTER_CLASS(EphemerisItem);
    JEOD_REGISTER_CLASS(EphemerisRefFrame);
    JEOD_REGISTER_CHECKPOINTABLE(this, planets);
    JEOD_REGISTER_CHECKPOINTABLE(this, ephemerides);
    JEOD_REGISTER_CHECKPOINTABLE(this, ephem_items);
    JEOD_REGISTER_CHECKPOINTABLE(this, integ_frames);
}

/**
 * EphemeridesManager destructor.
 */
EphemeridesManager::~EphemeridesManager()
{
    JEOD_DEREGISTER_CHECKPOINTABLE(this, planets);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, ephemerides);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, ephem_items);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, integ_frames);
}

/*******************************************************************************
Callback methods
*******************************************************************************/

/**
 * Denote that the reference frame tree needs to be rebuilt.
 */
void EphemeridesManager::ephem_note_tree_status_change()
{
    regenerate_ref_frame_tree = true;
}

/*******************************************************************************
Planet list methods
*******************************************************************************/

/**
 * Add a planet to the planets registry.
 * @param planet  Planet to be added to the registry.
 */
void EphemeridesManager::add_planet(BasePlanet & planet)
{
    // Handle errors.
    // 1. The planet must have a minimally valid name.
    if(!validate_name(__FILE__, __LINE__, planet.name, "Argument", "planet.name"))
    {
        return;
    }

    // 2. The planet must not have been previously registered.
    if(std::find(planets.begin(), planets.end(), &planet) != planets.end())
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              EphemeridesMessages::duplicate_entry,
                              "Planet '%s' is already registered.",
                              planet.name.c_str());
        return;
    }

    // 3. The planet must have a unique name.
    if(find_base_planet(planet.name) != nullptr)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              EphemeridesMessages::duplicate_entry,
                              "Planet with name '%s' was previously registered.",
                              planet.name.c_str());
        return;
    }

    // Add the planet to the planet list.
    planets.push_back(&planet);
}

/**
 * Find the planet with the given name.
 * @param name  Planet name.
 * @return Found planet; NULL if not found.
 */
BasePlanet * EphemeridesManager::find_base_planet(const std::string & name) const
{
    BasePlanet * found_planet = nullptr;

    // Search the planet list until a match is found.
    auto name_compare = [&name](const BasePlanet * planet)
    {
        return planet->name == name;
    };

    const auto iter = std::find_if(planets.begin(), planets.end(), name_compare);

    if(iter != planets.end())
    {
        found_planet = *iter;
    }

    // Return the found planet (if any).
    return found_planet;
}

/**
 * Return number of registered planets.
 * @returns: Number of registered planets.
 */
unsigned int EphemeridesManager::get_num_planets() const
{
    return planets.size();
}

/*******************************************************************************
Ephemerides model list methods
*******************************************************************************/

/**
 * Make subsequent calls to add_ephemeris deactivate the specified ephemeris
 * model instead of adding it to the list of managed models.
 * @note This is an irrevocable act.
 *
 * ## Assumptions and limitations:
 *  - This method is typically used after clearing the ephemerides model
 *    list and then adding the one allowed ephermides model:@code
 *      EphemeridesManager::clear_added_ephemerides();
 *      EphemeridesManager::add_ephemeris (model);
 *      EphemeridesManager::add_ephem_item (item);
 *      EphemeridesManager::disable_add_ephemeris ();
 *    @endcode
 */
void EphemeridesManager::disable_add_ephemeris()
{
    single_ephem_mode = true;
}

/**
 * Add an ephemeris model to the list of managed models.
 *
 * ## Assumptions and limitations:
 *  - Ephemeris models must be registered with the ephemerides manager in
 *    dependency order: Models with no dependencies are registered first,
 *    followed by models that depend on these base models, and so on.
 *
 * @param ephem_if  Ephemeris model to be added to the registry.
 */
void EphemeridesManager::add_ephemeris(EphemerisInterface & ephem_if)
{
    // Add the provided ephemeris model to the list of such.
    ephemerides.push_back(&ephem_if);
}

/**
 * Deactivate any registered ephemeris items and remove them from the list.
 */
void EphemeridesManager::clear_added_ephemerides()
{
    if((!ephemerides.empty()) || (!ephem_items.empty()))
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             EphemeridesMessages::single_ephem_mode,
                             "Ephemeris models have been prematurely registered with the manager.\n"
                             "Please check the priorities in the simulation definition file.");

        // Disable each ephemeris item.
        for(auto ephem_item : ephem_items)
        {
            ephem_item->disable();
        }

        // Empty the list of ephemeris items.
        ephem_items.clear();

        // Empty the ephemerides
        ephemerides.clear();
    }
}

/*******************************************************************************
Ephemeris item list methods
*******************************************************************************/

/**
 * Add an ephemeris item to the set of ephemeris items known to the ephemerides
 * manager. Each ephemeris model calls this method for each ephemeris item
 * represented by that ephemeris model. The provided item is added to
 * - The ephem_items list if the provided item's name is unique or
 * - The tail of the list of items with the same name if an item with the
 *   provided item's name is already in the ephem_items list.
 *
 * Multiple ephemerides models might represent the same conceptual item. The
 * ephem_items list contains only items with unique names. These head items link
 * to other items with the same name via the items' next data members, which
 * form a singly-linked list of commonly-named items.
 *
 * ## Assumptions and limitations:
 *  - The item must have a valid name.
 *  - Only one item with a given name can be enabled.
 *
 * @param ephem_item  Ephemeris item to be added to the registry.
 */
void EphemeridesManager::add_ephem_item(EphemerisItem & ephem_item)
{
    // The ephemeris item must have a minimally valid name.
    if(!validate_name(__FILE__, __LINE__, ephem_item.get_name(), "Argument", "ephem_item.name"))
    {
        return;
    }

    // Reject items that represents points when in single ephemeris mode.
    // (Items that represent rotations are allowed in single ephemeris mode.)
    if(single_ephem_mode && (ephem_item.updates_what() == EphemerisItem::Translation))
    {
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               EphemeridesMessages::single_ephem_mode,
                               "Ephemeris item '%s' is being disabled and ignored.",
                               ephem_item.get_name().c_str());
        ephem_item.disable();
        return;
    }

    // Set the item's ephemeris manager to this object.
    ephem_item.set_manager(this);

    // Find the head of the list of items with the same name.
    EphemerisItem * head_item = find_ephem_item(ephem_item.get_name());

    // No item yet:
    // Add the provided item to the ephem_items list.
    if(head_item == nullptr)
    {
        ephem_item.set_head(&ephem_item);
        ephem_item.set_next(nullptr);
        ephem_items.push_back(&ephem_item);
    }

    // Existing item: validate and add to head item's list.
    else
    {
        EphemerisItem * last_item = head_item;

        // Find the last item and verify that the item has not been registered.
        for(EphemerisItem * item = head_item; item != nullptr; item = item->get_next())
        {
            last_item = item;
            if(item == &ephem_item)
            {
                MessageHandler::error(__FILE__,
                                      __LINE__,
                                      EphemeridesMessages::duplicate_entry,
                                      "Ephemeris item '%s' is already registered.",
                                      ephem_item.get_name().c_str());
                return;
            }
        }

        // At most one item can be enabled.
        if((head_item->get_enabled_item() != nullptr) && ephem_item.is_enabled())
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 EphemeridesMessages::inconsistent_setup,
                                 "Multiple ephemeris items named '%s' are enabled.\n"
                                 "Disabling item from %s model.",
                                 ephem_item.get_name().c_str(),
                                 ephem_item.get_owner()->get_name().c_str());
            ephem_item.disable();
        }

        // Link the item into the list of items with the same name.
        ephem_item.set_head(head_item);
        last_item->set_next(&ephem_item);
        ephem_item.set_next(nullptr);
    }

    RefFrame * frame = find_ref_frame(ephem_item.get_name());
    auto * ephem_frame = dynamic_cast<EphemerisRefFrame *>(frame);

    if(ephem_frame != nullptr)
    {
        ephem_item.set_target_frame(*ephem_frame);
    }

    else if(frame != nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EphemeridesMessages::internal_error,
                             "Frame '%s' is not an ephemeris reference frame.",
                             frame->get_name().c_str());
    }
}

/**
 * Find the first registered EphemerisItem with the given name.
 * @param name  Ephemeris item name
 * @return Found ephemeris item
 */
EphemerisItem * EphemeridesManager::find_ephem_item(const std::string & name) const
{
    EphemerisItem * found_item = nullptr;

    for(auto ephem_item : ephem_items)
    {
        if(name == ephem_item->get_name())
        {
            found_item = ephem_item;
            break;
        }
    }
    return found_item;
}

/**
 * Find the EphemerisOrientation with the given name.
 * @param name  Ephemeris angle name
 * @return Found ephemeris angle
 */
EphemerisOrientation * EphemeridesManager::find_ephem_angle(const std::string & name) const
{
    EphemerisItem * ephem_item = find_ephem_item(name);
    EphemerisOrientation * ephem_angle = nullptr;

    if(ephem_item != nullptr)
    {
        ephem_angle = dynamic_cast<EphemerisOrientation *>(ephem_item);
        if(ephem_angle == nullptr)
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  EphemeridesMessages::invalid_item,
                                  "Ephemeris item '%s' is not an angle",
                                  name.c_str());
        }
    }
    return ephem_angle;
}

/**
 * Find the EphemerisPoint with the given name.
 * @param name  Ephemeris point name
 * @return Found ephemeris point
 */
EphemerisPoint * EphemeridesManager::find_ephem_point(const std::string & name) const
{
    EphemerisItem * ephem_item = find_ephem_item(name);
    EphemerisPoint * ephem_point = nullptr;

    if(ephem_item != nullptr)
    {
        ephem_point = dynamic_cast<EphemerisPoint *>(ephem_item);
        if(ephem_point == nullptr)
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  EphemeridesMessages::invalid_item,
                                  "Ephemeris item '%s' is not a point",
                                  name.c_str());
        }
    }
    return ephem_point;
}

/*******************************************************************************
Integration frames methods
*******************************************************************************/

/**
 * Add a frame to the reference frame and integration frame lists.
 * @param ref_frame  Integration frame to be added to the registries
 */
void EphemeridesManager::add_integ_frame(EphemerisRefFrame & ref_frame)
{
    // Add the frame to the list of integration frames.
    integ_frames.push_back(&ref_frame);

    // All integration frames are reference frames.
    // Note that this protects against duplicate names.
    add_ref_frame(ref_frame);
}

/**
 * Find the integration frame with the given name.
 * @param name  Integration frame name
 * @return Found integration frame
 */
EphemerisRefFrame * EphemeridesManager::find_integ_frame(const std::string & name) const
{
    EphemerisRefFrame * found_frame = nullptr;

    for(auto ref_frame : integ_frames)
    {
        if(name == ref_frame->get_name())
        {
            found_frame = ref_frame;
            break;
        }
    }
    return found_frame;
}

/**
 * Get a copy of the vector of integration frames.
 * @return Copy of integration frames vector
 */
const std::vector<EphemerisRefFrame *> & EphemeridesManager::get_integ_frames() const
{
    return integ_frames;
}

/**
 * Determine if supplied frame is an integration frame.
 * @param ref_frame  Reference frame to test
 * @return True if the frame is a registered integration frame, false otherwise
 */
bool EphemeridesManager::is_integ_frame(const RefFrame & ref_frame) const
{
    bool result = false;

    auto frame_matcher = [&ref_frame](const RefFrame * integ_frame)
    {
        return integ_frame == &ref_frame;
    };

    if(std::any_of(integ_frames.begin(), integ_frames.end(), frame_matcher))
    {
        result = true;
    }

    return result;
}

/**
 * Find the index of provided frame in the integration frames vector.
 * @param ref_frame  Reference to be found
 * @return Index of found frame
 */
unsigned int EphemeridesManager::find_integ_frame_index(const EphemerisRefFrame & ref_frame) const
{
    unsigned int nframes = integ_frames.size();
    unsigned int integ_frame_index = nframes;
    unsigned int ii = 0;

    for(auto it = integ_frames.begin(); it != integ_frames.end(); ++ii, ++it)
    {
        const EphemerisRefFrame * integ_frame = *it;
        if(integ_frame == &ref_frame)
        {
            integ_frame_index = ii;
            break;
        }
    }

    if(integ_frame_index == nframes)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EphemeridesMessages::invalid_item,
                             "Reference frame '%s' is not an integration frame",
                             ref_frame.get_name().c_str());

        // Not reached
        return nframes;
    }

    return integ_frame_index;
}

/*******************************************************************************
Reference frame methods
*******************************************************************************/

/**
 * Add a reference frame to the reference frame registry.
 * @param ref_frame  Reference frame to be added to the registry
 */
void EphemeridesManager::add_ref_frame(RefFrame & ref_frame)
{
    // Add the frame to the registry.
    RefFrameManager::add_ref_frame(ref_frame);
    set_target_frame(ref_frame);
}

/**
 * Set the target-frame reference for the ref-frame and all known EphemItems
 * with similar target-frame names.
 * @param ref_frame  Reference frame to be used as the target-frame.
 */
void EphemeridesManager::set_target_frame(RefFrame & ref_frame)
{
    // An ephemeris reference frame needs special treatment.
    // There should be an ephemeris item with the same name that needs to
    // to be tied to the frame, and the frame needs to be tied to the
    // ephemerides manager (this object).
    auto * ephem_frame = dynamic_cast<EphemerisRefFrame *>(&ref_frame);
    EphemerisItem * head_item = find_ephem_item(ref_frame.get_name());
    if(ephem_frame != nullptr)
    {
        // A change in the active status of an ephemeris reference frames means
        // that the reference frame tree needs to be rebuilt.
        // To accomodate this, ephemeris reference frames contain a link to the
        // ephemerides manager (this object).
        ephem_frame->set_ephem_manager(this);

        // The target frame of an ephemeris item is the ephemeris reference frame
        // with the same name as the ephemeris item.
        // Note that this sets the target frame for the head item and all linked
        // items that follow it through the "next" link.
        if(head_item != nullptr)
        {
            head_item->set_target_frame(*ephem_frame);
        }

        // No else; the frame will be tied to the ephemeris item when the
        // appropriately-named item is added.
    }

    // Non-ephemeris reference frames that map to an ephemeris item are
    // highly problematic. The simulation was build incorrectly.
    else if(head_item != nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EphemeridesMessages::inconsistent_setup,
                             "Frame '%s' is not an ephemeris reference frame.",
                             ref_frame.get_name().c_str());
    }
}

/*******************************************************************************
Ephemeris model methods
*******************************************************************************/

/**
 * Initialize the ephemeris models.
 */
void EphemeridesManager::initialize_ephemerides()
{
    // Initialize each ephemeris model.
    for(auto ephem : ephemerides)
    {
        ephem->ephem_initialize(*this);
    }

    // Force the tree to be rebuilt.
    regenerate_ref_frame_tree = true;
}

/**
 * Update each ephemeris model.
 */
void EphemeridesManager::update_ephemerides()
{
    // Update which ephemeris items are needed if any gross activity
    // changes have occurred.
    if(regenerate_ref_frame_tree)
    {
        activate_ephemerides();
    }

    // Update each ephemeris model.
    for(std::vector<EphemerisInterface *>::const_iterator it = ephemerides.begin(); it != ephemerides.end(); ++it)
    {
        EphemerisInterface * ephem = *it;
        ephem->ephem_update();
    }
}

/**
 * Activate ephemeris items based on frame subscription status,
 * activate ephemeris models, and build the reference frame tree.
 */
void EphemeridesManager::activate_ephemerides()
{
    // There's nothing to do if the tree doesn't need to be built/rebuilt.
    if(!regenerate_ref_frame_tree)
    {
        return;
    }

    // Disconnect the registered ephemeris items from the tree.
    for(std::vector<EphemerisItem *>::const_iterator it = ephem_items.begin(); it != ephem_items.end(); ++it)
    {
        EphemerisItem * ephem_item = *it;
        ephem_item->disconnect_from_tree();
    }

    // Forget that the reference frame tree has a root.
    reset_tree_root_node();

    // Activate the ephemeris models in reverse order.
    // Activating an ephemeris model that depend on some other ephemeris model
    // may activate additional ephemeris items in that higher-level model.
    // This dependency-based activation necessitates the reverse order.
    for(auto it = ephemerides.rbegin(); it != ephemerides.rend(); ++it)
    {
        EphemerisInterface * ephem = *it;
        ephem->ephem_activate(*this);
    }

    // Build the tree starting from the first ephemeris model.
    // Here we start from the base (dependency-free) ephemeris models and
    // proceed downward to ensure that the base of the tree is built before
    // adding to it.
    for(std::vector<EphemerisInterface *>::const_iterator it = ephemerides.begin(); it != ephemerides.end(); ++it)
    {
        EphemerisInterface * ephem = *it;
        ephem->ephem_build_tree(*this);
    }

    // Finally, mark the tree as no longer needed to be regenerated.
    // Note that this assumes a single pass is all that is needed.
    // This will be the case if the base (dependency-free) ephemeris models
    // are registered first and if the dependencies form an acyclic graph.
    regenerate_ref_frame_tree = false;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
