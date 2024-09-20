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
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/include/mass_point.hh
 * Define the class MassPoint, which defines the base features of
 * a point related to a MassBody. A MassPoint is simply a MassPointState
 * with a name plus a pointer to a "parent" MassPoint.
 */

/********************************* TRICK HEADER *******************************

Purpose:
  ()

Library Dependencies:
  ((../src/mass_point.cc))



*******************************************************************************/

#ifndef JEOD_MASS_POINT_HH
#define JEOD_MASS_POINT_HH

// Model includes
#include "class_declarations.hh"
#include "mass_point_links.hh"
#include "mass_point_state.hh"

// JEOD includes
#include "utils/named_item/include/named_item.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <string>
#include <utility>

//! Namespace jeod
namespace jeod
{

/**
 * Adds tree linkages and a name to a MassPointState.
 */
class MassPoint : public MassPointState
{
    JEOD_MAKE_SIM_INTERFACES(jeod, MassPoint)

    friend class MassPointLinks;
    friend class MassBody;

    // Member data
protected:
    /**
     * The name of the mass point.
     */
    NamedItem name; //!< trick_units(--)

    /**
     * Linkage to rest of mass tree.
     * Programmatic interfaces:
     * - MassPointLinks provides accessors to the parent and root and
     * provides methods to attach, detach links (and hence bodies).
     * - This class provides accessors to the same.
     * - Various iterators provide the ability to iterate over child bodies
     * and up the parent chain.
     */
    MassPointLinks links; //!< trick_units(--)

    // Member functions
public:
    /**
     * Default constructor.
     */
    MassPoint()
        : links(*this)
    {
    }

    // Destructor.
    ~MassPoint() override;

    MassPoint(const MassPoint &) = delete;
    MassPoint & operator=(const MassPoint &) = delete;

    // Initialize a MassPoint.
    void initialize_mass_point() override;

    // Set the name.
    void set_name(std::string name_in)
    {
        name = std::move(name_in);
    }

    // Get the name.
    std::string get_name() const
    {
        return name.get_name();
    }

    // compute_relative_state: Compute the relative state between points
    // This and the ref_point can be any points in the same tree
    // The relative state is in terms of the ref_point's frame.
    virtual void compute_relative_state(const MassPoint & ref_point, MassPointState & rel_state) const;

    // compute_state_wrt_pred: Compute the relative state of this point
    // with respect to some reference point, which must be at or above
    // this point via the parent links.
    // The relative state is in terms of the ref_point's frame.
    virtual void compute_state_wrt_pred(const MassPoint & ref_point, MassPointState & rel_state) const;

    // compute_state_wrt_pred: Compute the relative state of this point
    // with respect to some reference point, which must be at or above
    // this point via the parent links.
    // The relative state is in terms of the ref_point's frame.
    virtual void compute_state_wrt_pred(unsigned int ref_point_index, MassPointState & rel_state) const;

    // compute_pred_rel_state: Compute the relative state of some
    // reference point respect to this point. The reference point must
    // be at or above this point via the parent links.
    // The relative state is in terms of the this' frame.
    virtual void compute_pred_rel_state(const MassPoint & ref_point, MassPointState & rel_state) const;

    // compute_pred_rel_state: Compute the relative state of some
    // reference point respect to this point. The reference point must
    // be at or above this point via the parent links.
    // The relative state is in terms of the this' frame.
    virtual void compute_pred_rel_state(unsigned int ref_point_index, MassPointState & rel_state) const;

    // find_last_common_node: Find the point of departure between nodes
    const MassPoint * find_last_common_node(const MassPoint & ref_point) const;

protected:
    // find_last_common_index: Find the point of departure between nodes
    int find_last_common_index(const MassPoint & ref_point) const;

private:
    // attach: Attach to another point
    void attach(MassPoint & parent);

    // detach: Detach point from its parent
    void detach();
};

/**
 * Each mass point has a path from the root of the mass point tree
 * to the point in question. The paths for two mass points will have
 * some initial sequence of common nodes.
 * Find the index number of this last element in this sequence.
 * @return Last common node
 * \param[in] ref_point Other point
 */
inline int MassPoint::find_last_common_index(const MassPoint & ref_point) const
{
    return links.find_last_common_index(ref_point.links);
}

/**
 * Each mass point has a path from the root of the mass point tree
 * to the point in question. The paths for two mass points will have
 * some initial sequence of common nodes.
 * Find the last element in this sequence.
 * @return Last common node
 * \param[in] frame Other point
 */
inline const MassPoint * MassPoint::find_last_common_node(const MassPoint & frame) const
{
    const MassPointLinks * common_link = links.find_last_common_node(frame.links);
    if(common_link != nullptr)
    {
        return (&(common_link->container()));
    }
    else
    {
        return nullptr;
    }
}

/**
 * Attach a mass point to another.
 *
 * \par Assumptions and Limitations
 *  - This method only addresses the linkages.
 *                  Some external agent must address the physical relation.
 * \param[in,out] parent parent node
 */
inline void MassPoint::attach(MassPoint & parent)
{
    links.attach(parent.links);
}

/**
 * Detach a mass point from its parent.
 *
 * \par Assumptions and Limitations
 *  - This method only addresses the linkages.
 *                  Some external agent must address the physical relation.
 */
inline void MassPoint::detach()
{
    links.detach();
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
