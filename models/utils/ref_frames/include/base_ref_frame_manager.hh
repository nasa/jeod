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
 * @file models/utils/ref_frames/include/base_ref_frame_manager.hh
 * Define the BaseRefFrameManager class, which defines the interfaces but not
 * the implementations of the class RefFrameManager.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_BASE_REF_FRAME_MANAGER_HH
#define JEOD_BASE_REF_FRAME_MANAGER_HH

// System includes
#include <string>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class RefFrame;

/**
 * The RefFrameManager class manages the reference frames in a simulation.
 * This class defines the external interfaces to that class.
 */
class BaseRefFrameManager
{
    JEOD_MAKE_SIM_INTERFACES(jeod, BaseRefFrameManager)

public:
    // Member functions
    // Note: The default constructor, copy constructor, and assignment operator
    // for this class are not declared. The C++ defaults are in force.

    /**
     * Destructor.
     */
    virtual ~BaseRefFrameManager() = default;

    /**
     * Add a reference frame to the list of such.
     * @param ref_frame Frame to be added.
     */
    virtual void add_ref_frame(RefFrame & ref_frame) = 0;

    /**
     * Remove a reference frame from the list of such.
     * @param ref_frame Frame to be removed.
     */
    virtual void remove_ref_frame(RefFrame & ref_frame) = 0;

    /**
     * Find a reference frame.
     * @param name Frame to be found.
     * @return Found reference frame.
     */
    virtual RefFrame * find_ref_frame(const std::string & name) const = 0;

    /**
     * Find a reference frame.
     * @param prefix Prefix of frame to be found.
     * @param suffix Suffix of frame to be found.
     * @return Found reference frame.
     */
    virtual RefFrame * find_ref_frame(const std::string & prefix, const std::string & suffix) const = 0;

    /**
     * Check whether each reference frame has an owner.
     */
    virtual void check_ref_frame_ownership() const = 0;

    /**
     * Reset the root node in anticipation of rebuilding the entire tree.
     */
    virtual void reset_tree_root_node() = 0;

    /**
     * Add a reference frame to the reference frame tree.
     * @param ref_frame Frame to be added.
     * @param parent    Parent of the frame.
     */
    virtual void add_frame_to_tree(RefFrame & ref_frame, RefFrame * parent) = 0;

    /**
     * Add a subscription to a reference frame.
     * @param frame_name Frame to which subscription is to be issued.
     */
    virtual void subscribe_to_frame(const std::string & frame_name) = 0;

    /**
     * Add a subscription to a reference frame.
     * @param frame Frame to which subscription is to be issued.
     */
    virtual void subscribe_to_frame(RefFrame & frame) = 0;

    /**
     * Remove a subscription from a reference frame.
     * @param frame_name Frame from which subscription is to be removed.
     */
    virtual void unsubscribe_to_frame(const std::string & frame_name) = 0;

    /**
     * Remove a subscription from a reference frame.
     * @param frame Frame from which subscription is to be removed.
     */
    virtual void unsubscribe_to_frame(RefFrame & frame) = 0;

    /**
     * Check whether a reference frame has subscriptions.
     * @param frame_name Frame to be checked.
     * @return True if frame has subscriptions, false otherwise.
     */
    virtual bool frame_is_subscribed(const std::string & frame_name) = 0;

    /**
     * Check whether a reference frame has subscriptions.
     * @param frame Frame to be checked.
     * @return True if frame has subscriptions, false otherwise.
     */
    virtual bool frame_is_subscribed(RefFrame & frame) = 0;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
