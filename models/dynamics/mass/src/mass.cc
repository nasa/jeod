/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass.cc
 * Define methods for the MassBody class.
 */

/*******************************************************************************
  Purpose:
    ()

  Reference:
    (((TBS)))

  Assumptions and limitations:
    ((N/A))

  Class:
    (N/A)

  LIBRARY DEPENDENCY:
    ((mass.cc)
     (mass_attach.cc)
     (mass_calc_composite_cm.cc)
     (mass_calc_composite_inertia.cc)
     (mass_point_mass_inertia.cc)
     (mass_detach.cc)
     (mass_print_body.cc)
     (mass_print_tree.cc)
     (mass_reattach.cc)
     (mass_update.cc)
     (mass_properties_init.cc)
     (mass_point_init.cc)
     (mass_point.cc)
     (mass_messages.cc)
     (utils/named_item/src/named_item.cc)
     (dynamics/dyn_body/src/dyn_body_vehicle_point.cc)
     (dynamics/dyn_manager/src/dyn_manager.cc))


*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/ref_frames/include/tree_links_iterator.hh"

// Model includes
#include "../include/mass.hh"
#include "../include/mass_messages.hh"
#include "../include/mass_properties_init.hh"

//! Namespace jeod
namespace jeod
{

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES(MassPoint)

/**
 * Default constructor; constructs a MassBody object.
 */
MassBody::MassBody()
    : links(*this)
{
    JEOD_REGISTER_CLASS(MassBody);
    JEOD_REGISTER_CLASS(MassPoint);

    // Initialize the connectivities of the contained MassPoint objects. The
    // three MassProperties objects form a tree with the structure_point as
    // the root of this tree and the core_ and composite_properties as children
    // of the structure_point.
    core_properties.attach(structure_point);
    composite_properties.attach(structure_point);

    // The MassBody object contains additional representations of the core and
    // composite frames as mass points to aid in the computation of composite
    // properties. These are the core properties with respect to the composite
    // properties and the composite properties with respect to the parent mass
    // body's structural and composite body frames.
    // These latter two start unattached. The former needs to be attached.
    core_wrt_composite.attach(composite_wrt_pbdy);

    // The core and composite body frames share the same axes.
    // Note: This statement could be omitted as the MassPointState constructor
    // initializes the transformation matrix to identity.
    // The initialization is intentionally replicated here intentionally.
    Matrix3x3::identity(core_wrt_composite.T_parent_this);
}

/**
 * Default constructor; constructs a MassBody object.
 */
MassBody::MassBody(DynBody & owner)
    : name(),
      dyn_owner(&owner),
      links(*this)
{
    JEOD_REGISTER_CLASS(MassBody);
    JEOD_REGISTER_CLASS(MassPoint);

    // Initialize the connectivities of the contained MassPoint objects. The
    // three MassProperties objects form a tree with the structure_point as
    // the root of this tree and the core_ and composite_properties as children
    // of the structure_point.
    core_properties.attach(structure_point);
    composite_properties.attach(structure_point);

    // The MassBody object contains additional representations of the core and
    // composite frames as mass points to aid in the computation of composite
    // properties. These are the core properties with respect to the composite
    // properties and the composite properties with respect to the parent mass
    // body's structural and composite body frames.
    // These latter two start unattached. The former needs to be attached.
    core_wrt_composite.attach(composite_wrt_pbdy);

    // The core and composite body frames share the same axes.
    // Note: This statement could be omitted as the MassPointState constructor
    // initializes the transformation matrix to identity.
    // The initialization is intentionally replicated here intentionally.
    Matrix3x3::identity(core_wrt_composite.T_parent_this);
}

/**
 * Destroy a MassBody object.
 */
MassBody::~MassBody()
{
    // Denote that the parent's (if any) composite properties need an update.
    set_update_flag();

    // If frame has children, all those children and their progeny contain
    // pointers to this frame. Sever those links by making each child a root.
    while(links.has_children())
    {
        links.child_tail()->container().detach();
    }

    // Sever the links from the parent and sibling nodes as well.
    if(!links.is_root())
    {
        bool success = detach();
        // Assume failure because this or attachment belongs to DynBody
        if(!success)
        {
            /*
             * FATAL!
             * Can't find a DynManager, can't attempt DynBody disconnect.
             * Try to print a message.
             * NOTE:
             * During shutdown destruction, the MessageHandler may not be
             * available!
             */
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 MassBodyMessages::invalid_detach,
                                 "MassBody '%s' is attached to something "
                                 "at shutdown, but failed to detach!",
                                 name.c_str());
        }
    }

    // Detach and free the allocated MassPoints.
    while(!mass_points.empty())
    {
        MassPoint * point = mass_points.back();
        point->detach();
        JEOD_DELETE_OBJECT(point);
        mass_points.pop_back();
    }
}

/**
 * Initialize a MassBody object.
 * \param[in] properties Core mass ppty specs
 * \param[in] points Mass point specs
 */
void MassBody::initialize_mass(const MassPropertiesInit & properties, const std::vector<MassPointInit *> & points)
{
    // Denote that the composite properties need an update.
    set_update_flag();

    // Initialize the core mass properties.
    properties.initialize_mass_properties(core_properties);

    mass_properties_initialized = true;

    // The core and composite frames have the same alignment wrt structure.
    composite_properties.Q_parent_this = core_properties.Q_parent_this;
    Matrix3x3::copy(core_properties.T_parent_this, composite_properties.T_parent_this);

    // Add the mass points.
    for(size_t ii = 0; ii < points.size(); ii++)
    {
        MassPointInit * point = points.at(ii);
        if(dyn_owner == nullptr)
        {
            add_mass_point(*point);
        }
        else
        {
            dyn_owner->add_mass_point(*point);
        }
    }
}

/**
 * Returns the MassBody's parent body, as a const pointer.
 * @return Pointer to parent body
 */
const MassBody * MassBody::get_parent_body() const
{
    return (links.parent());
}

/**
 * Returns the MassBody's parent body, as a non-const pointer.
 * @return Pointer to parent body
 */
MassBody * MassBody::get_parent_body_internal()
{
    return (links.parent());
}

/**
 * Finds & returns root of current MassBody's tree.
 * @return Pointer to root body
 */
const MassBody * MassBody::get_root_body() const
{
    return links.root();
}

/**
 * Finds & returns root of current MassBody's tree.
 * @return Pointer to root body
 */
MassBody * MassBody::get_root_body_internal()
{
    return links.root();
}

/**
 * Return true if this MassBody is an offspring of provided one,
 * false if not.
 * @return Is this offspring of test_body?
 * \param[in] test_body Other MassBody
 */
bool MassBody::is_progeny_of(const MassBody & test_body) const
{
    bool is_progeny;

    // Traverse up the mass tree from this to try to find test_body.
    is_progeny = false;
    for(auto * link : TreeLinksAscendRange<const MassBodyLinks>(links))
    {
        if(&link->container() == &test_body)
        {
            is_progeny = true;
            break;
        }
    }

    return is_progeny;
}

/**
 * Flag mass bodies from the current body on up the mass tree
 * as in need of mass property updates.
 */
void MassBody::set_update_flag()
{
    // Mark all bodies from this body on up the mass tree as needing an update.
    for(auto * link : TreeLinksAscendRange<MassBodyLinks>(links))
    {
        link->container().needs_update = true;
    }
}

/**
 * Return the number of mass points for this body.
 * @return Mass point
 */
size_t MassBody::mass_points_size() const
{
    return mass_points.size();
}

/**
 * Find the mass point with the given name.
 * @return Mass point
 * \param[in] pt_name mass point name
 */
const MassPoint * MassBody::find_mass_point(const std::string & pt_name) const
{
    std::size_t search_offset = name.size() + 1;
    const MassPoint * found_point = nullptr;
    std::string pt_suffix;

    // Only search if the name is valid. (No else; found point is already NULL)
    if(!pt_name.empty())
    {
        pt_suffix = name.suffix(pt_name);

        // Search for the point.
        auto name_compare = [&search_offset, &pt_suffix](const MassPoint * point)
        {
            return point->name.ends_with(search_offset, pt_suffix);
        };

        const auto iter = std::find_if(mass_points.begin(), mass_points.end(), name_compare);

        if(iter != mass_points.end())
        {
            found_point = *iter;
        }
    }

    return found_point;
}

/**
 * Add a mass point to the list of such.
 * \param[in] mass_point_init Mass point spec
 */
void MassBody::add_mass_point(const MassPointInit & mass_point_init)
{
    const std::string & pt_suffix = name.suffix(mass_point_init.name);
    MassPoint * mass_point;

    // Sanity checks:
    // 1. The MassPoint must have a name.
    if(pt_suffix.empty())
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             MassBodyMessages::invalid_name,
                             "Attempt to add a MassPoint with an empty name "
                             "to the MassPoint list for MassBody '%s'.",
                             name.c_str());

        // Not reached
        return;
    }

    // 2. The MassPoint's name must be unique.
    else if(find_mass_point(mass_point_init.name) != nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             MassBodyMessages::invalid_name,
                             "Attempt to add a MassPoint with duplicate name '%s' "
                             "to the MassPoint list for MassBody '%s'.",
                             mass_point_init.name.c_str(),
                             name.c_str());

        // Not reached
        return;
    }
    // No else: All checks passed.

    // Allocate the point.
    mass_point = JEOD_ALLOC_CLASS_OBJECT(MassPoint, ());

    // Construct the mass point's name as body_name.suffix.
    mass_point->name.set_name(name.get_name(), pt_suffix);
    mass_point->name.freeze_name();

    // Link the mass point to the structural frame.
    mass_point->attach(structure_point);

    // Complete initialization of the mass point.
    mass_point_init.initialize_mass_point(*mass_point);

    // Add the mass point to the list of such.
    mass_points.push_back(mass_point);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
