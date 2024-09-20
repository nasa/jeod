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
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemItem
 * @{
 *
 * @file models/environment/ephemerides/ephem_item/include/ephem_item.hh
 * Define classes for items represented in some ephemeris model.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/ephem_item.cc))



*******************************************************************************/

#ifndef JEOD_EPHEM_ITEM_HH
#define JEOD_EPHEM_ITEM_HH

// System includes

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_interface.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The EphemerisItem class is the base class for representing an item that is
 * modeled in an ephemeris model. Ephemeris items form the bridge between the
 * reference frame model and the ephemeris models.
 * An EphemerisItem
 *  - Has a name, which is not necessarily unique. Ephemeris items with the same
 *    name are linked to one another to form a linked list.
 *  - Has a target reference frame. This is the ephemeris reference frame which
 *    has the same name as the ephemeris item.
 *  - Can be enabled or disabled. At most one item of a set of commonly-named
 *    items can be enabled, and only if a correspondingly-named ephemeris
 *    reference frame exists. Ownership of the correspondingly-named ephemeris
 *    reference frame transfers to the enabled ephemeris item. When an ephemeris
 *    item is disabled, the ephemeris model that owns the ephemeris item should
 *    not operate on the correspondingly-named reference frame.
 *  - Can be active or inactive. Disabled items must always be inactive. The
 *    activity level of the enabled item for a set of commonly-named items is
 *    automatically maintained to be the same as that of the target frame.
 *
 * The ephemeris model that owns an active ephemeris item is responsible
 * for ensuring that the correspondingly-named ephemeris reference frame is
 * a part of the active reference frame tree and for updating that reference
 * frame's state.
 */
class EphemerisItem : public RefFrameOwner
{
    JEOD_MAKE_SIM_INTERFACES(jeod, EphemerisItem)

public:
    // Types

    /**
     * Defines the aspect of the target frame that will be modified by the
     * EphemerisItem object.
     */
    enum TargetAspect
    {
        Translation = 0, /*
           The object updates the translational state of the target frame. */

        Rotation = 1 /*
       The object updates the rotational state of the target frame. */
    };

    // Member functions
    EphemerisItem() = default;
    ~EphemerisItem() override = default;
    EphemerisItem(const EphemerisItem &) = delete;
    EphemerisItem & operator=(const EphemerisItem &) = delete;

    // Advertised interfaces

    // Accessors for the item's name.
    virtual void set_name(const std::string & new_name);
    virtual void set_name(const std::string & pname, const std::string & fname);
    std::string get_name() const;

    // Accessors for the timestamp.
    virtual void set_timestamp(double time);
    double timestamp() const;

    // Accessors for the owner.
    // Restrictions: set_owner should only be called by the item's owner.
    virtual void set_owner(EphemerisInterface * new_owner);
    EphemerisInterface * get_owner() const;

    // Accessors for the manager.
    // Restrictions: set_manager should only be called by the EphemeridesManager.
    virtual void set_manager(BaseEphemeridesManager * new_manager);
    BaseEphemeridesManager * get_manager() const;

    // Accessors for the next and head pointers.
    // Restrictions: These should only be used by the EphemeridesManager.
    virtual void set_next(EphemerisItem * next_item);
    EphemerisItem * get_next() const;

    virtual void set_head(EphemerisItem * head_item);
    EphemerisItem * get_head() const;

    // Accessors for the target_frame pointer.
    // Restrictions: The setter should only be called by the ephemeris model
    // that owns this item or by the EphemeridesManager.
    virtual void set_target_frame(EphemerisRefFrame & frame);
    EphemerisRefFrame * get_target_frame() const;

    // Accessors for the enabled flag.
    virtual void enable();
    virtual void disable();
    bool is_enabled() const;

    // Get the enabled item amongst all like-named items, if any.
    EphemerisItem * get_enabled_item() const;

    // Accessors for the active flag.
    // Restrictions: activate should only be called if the item is activatable.
    virtual void activate();
    virtual void deactivate();
    bool is_active() const;
    bool is_activatable() const;

    // Validate a name.
    void validate_name(const char * file,
                       unsigned int line,
                       const std::string & new_value,
                       const std::string & old_value,
                       const std::string & variable_name);

    /**
     * Identifies which part of the target frame does the object updates.
     */
    virtual TargetAspect updates_what() const = 0;

    /**
     * The default suffix for the item.
     */
    virtual std::string default_suffix() const = 0;

    /**
     * Disconnect the item from the reference frame tree.
     */
    virtual void disconnect_from_tree() = 0;

protected:
    // Member functions

    // Set the name of the item.
    virtual void set_name_internal(const std::string & new_name);

    // Member data

    /**
     * The name of the item.
     */
    std::string name; //!< trick_units(--)

    /**
     * The ephemeris model that owns this object.
     */
    EphemerisInterface * owner{}; //!< trick_units(--)

    /**
     * The ephemeris manager that manages this object.
     */
    BaseEphemeridesManager * manager{}; //!< trick_units(--)

    /**
     * The reference frame whose non-constant state is set by this object.
     */
    EphemerisRefFrame * target_frame{}; //!< trick_units(--)

    /**
     * The first ephemeris item with the same name as this item.
     */
    EphemerisItem * head{}; //!< trick_units(--)

    /**
     * The next ephemeris item with the same name as this item.
     */
    EphemerisItem * next{}; //!< trick_units(--)

    /**
     * Time of last update, dynamic time seconds.
     */
    double update_time{}; //!< trick_units(s)

    /**
     * Is the item enabled?
     * - An item can be enabled only if the data associated with the item such as
     * the translational state of a planet exist somewhere in the simulation.
     * - Only one of a set of ephemeris items that share the same name can be
     * enabled. The enable method ensures that this is the case.
     * - Exactly one of a set of ephemeris items that share same name should be
     * enabled if some other simulation agent depends on the data associated
     * with an ephemeris item. Ensuring that this is the case is the
     * responsibility of the ephemeris models and the users of those models.
     */
    bool enabled{}; //!< trick_units(--)

    /**
     * Is the item active?
     * - An item can be activated only if it is enabled. The enable and activate
     * methods assure that this is the case.
     * - Activity is determined by the activity of the target frame, which is
     * in turn determined by the reference frame subscription model.
     */
    bool active{}; //!< trick_units(--)
};

} // namespace jeod

#include "ephem_item_inline.hh"

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
