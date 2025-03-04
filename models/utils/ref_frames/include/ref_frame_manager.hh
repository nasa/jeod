//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
 * @file models/utils/ref_frames/include/ref_frame_manager.hh
 * Define the RefFrameManager class, which manages the reference frames in a
 * JEOD-based simulation.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/ref_frame_manager.cc))



*******************************************************************************/

#ifndef JEOD_REF_FRAME_MANAGER_HH
#define JEOD_REF_FRAME_MANAGER_HH

// System includes

// JEOD includes
#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "base_ref_frame_manager.hh"

//! Namespace jeod
namespace jeod
{

class RefFrame;

/**
 * The RefFrameManager class manages the reference frames in a simulation.
 * This class is the base class for the EphemeridesManager and DynManager
 * classes. Those derived classes add functionality to this class.
 */
class RefFrameManager : virtual public BaseRefFrameManager
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RefFrameManager)

public:
    // Member functions

    // Constructor and destructor
    RefFrameManager();
    ~RefFrameManager() override;

    RefFrameManager(const RefFrameManager &) = delete;
    RefFrameManager & operator=(const RefFrameManager &) = delete;

    // Add a reference frame to the list of such.
    void add_ref_frame(RefFrame & ref_frame) override;

    // Remove a reference frame from the list of such.
    void remove_ref_frame(RefFrame & ref_frame) override;

    // Find a reference frame.
    RefFrame * find_ref_frame(const std::string & name) const override;
    RefFrame * find_ref_frame(const std::string & prefix, const std::string & suffix) const override;

    // Check whether each reference frame has an owner.
    void check_ref_frame_ownership() const override;

    // Reset the root node in anticipation of rebuilding the entire tree.
    void reset_tree_root_node() override;

    // Add a reference frame to the reference frame tree.
    void add_frame_to_tree(RefFrame & ref_frame, RefFrame * parent) override;

    // Add a subscription to a reference frame.
    void subscribe_to_frame(const std::string & frame_name) override;
    void subscribe_to_frame(RefFrame & frame) override;

    // Remove a subscription from a reference frame.
    void unsubscribe_to_frame(const std::string & frame_name) override;
    void unsubscribe_to_frame(RefFrame & frame) override;

    // Check whether a reference frame has subscriptions.
    bool frame_is_subscribed(const std::string & frame_name) override;
    bool frame_is_subscribed(RefFrame & frame) override;

protected:
    // Member functions

    // Validate a name (with error reporting)
    bool validate_name(const char * file,
                       unsigned int line,
                       const std::string & variable_value,
                       const std::string & variable_type,
                       const std::string & variable_name) const;

    // Member data
    // NOTE WELL: These are protected rather than private because of simulation
    // engine limitations. Inheriting classes should treat these as private
    // and use access methods.

    /**
     * The root node of the reference frame tree.
     * This reference frame is the true inertial frame of the simulation.
     */
    RefFrame * root_node{}; //!< trick_units(--)

    /**
     * List of reference frames.
     */
    JeodPointerVector<RefFrame>::type ref_frames; //!< trick_io(**)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
