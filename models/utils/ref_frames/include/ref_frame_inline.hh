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
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/include/ref_frame_inline.hh
 * Define inline methods for the RefFrame class.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))



*******************************************************************************/

#ifndef JEOD_REF_FRAME_INLINE_HH
#define JEOD_REF_FRAME_INLINE_HH

// System includes
#include <cstddef>

// JEOD includes
#ifndef JEOD_REF_FRAME_HH
#include "ref_frame.hh"
#endif

//! Namespace jeod
namespace jeod
{

/**
 * Return the name.
 * @return Void
 */
inline std::string RefFrame::get_name() const
{
    return name;
}

/**
 * Set the owner of this frame.
 * \param[in] new_owner New owner
 */
inline void RefFrame::set_owner(RefFrameOwner * new_owner)
{
    owner = new_owner;
}

/**
 * Return the owner of this frame.
 * @return Frame owner
 */
inline RefFrameOwner * RefFrame::get_owner() const
{
    return owner;
}

/**
 * Return the parent of this frame.
 * @return Frame parent
 */
inline const RefFrame * RefFrame::get_parent() const
{
    return links.parent();
}

/**
 * Return the root of this frame's tree.
 * @return Tree root
 */
inline const RefFrame * RefFrame::get_root() const
{
    return links.root();
}

/**
 * Set the update time of this frame.
 * \param[in] time Time\n Units: s
 */
inline void RefFrame::set_timestamp(double time)
{
    update_time = time;
}

/**
 * Return the update time of this frame.
 * @return Time of last update\n Units: s
 */
inline double RefFrame::timestamp() const
{
    return update_time;
}

/**
 * Make this frame a root frame.
 */
inline void RefFrame::make_root()
{
    links.make_root();
}

/**
 * Add a child frame to this frame.
 * \param[in,out] frame Frame to add as child
 */
inline void RefFrame::add_child(RefFrame & frame)
{
    frame.links.attach(links);
}

/**
 * Remove this node as a child of its parent node.
 */
inline void RefFrame::remove_from_parent()
{
    links.detach();
}

/**
 * Each reference frame has a path from the root of the reference
 * frame tree to the frame in question. The paths for two reference
 * frames will have some initial sequence of common nodes.
 * Find the index number of this last element in this sequence.
 * @return Last common node
 * \param[in] frame Other frame
 */
inline int RefFrame::find_last_common_index(const RefFrame & frame) const
{
    return links.find_last_common_index(frame.links);
}

/**
 * Each reference frame has a path from the root of the reference
 * frame tree to the frame in question. The paths for two reference
 * frames will have some initial sequence of common nodes.
 * Find the last element in this sequence.
 * @return Last common node
 * \param[in] frame Other frame
 */
inline const RefFrame * RefFrame::find_last_common_node(const RefFrame & frame) const
{
    const RefFrameLinks * common_link = links.find_last_common_node(frame.links);
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
 * Return true if this frame is a progeny of the provided frame,
 * false if not.
 * @return This is progeny of frame
 * \param[in] frame Other frame
 */
inline bool RefFrame::is_progeny_of(const RefFrame & frame) const
{
    return links.is_progeny_of(frame.links);
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
