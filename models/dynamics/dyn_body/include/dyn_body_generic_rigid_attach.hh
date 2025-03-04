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
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file
 * Define the class Wrench.
 */

/*
Purpose: ()
*/

#ifndef JEOD_DYN_BODY_GENERIC_RIGID_ATTACH_HH
#define JEOD_DYN_BODY_GENERIC_RIGID_ATTACH_HH

#include "../../mass/include/mass_point_state.hh"
#include "body_ref_frame.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * A wrench comprises a torque and a force applied at a point on a DynBody.
 * The torque should not include the torque due to the application of the
 * force.
 *
 * A Trick simulation issues vcollect statements such as
 *
 *      vcollect vehicle.dyn_body.collect_wrench.collection
 *      {
 *          wrench_model1.wrench,
 *          wrench_model2.wrench
 *      };
 */
class DynBodyGenericFrameAttachment
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DynBodyGenericFrameAttachment)

public:
    /**
     * Default constructor.
     */
    DynBodyGenericFrameAttachment() = default;

    // Member functions
    // Initialize the attachment variables with the parent_frame and the offset from the parent_frame
    // to the structure frame
    void initialize_attachment(RefFrame & parent_frame, const RefFrameState & attach_state)
    {
        active = true;
        rigid_attach_parent = &parent_frame;
        Vector3::copy(attach_state.trans.position, rigid_attach_state.trans.position);
        Matrix3x3::copy(attach_state.rot.T_parent_this, rigid_attach_state.rot.T_parent_this);
        rigid_attach_state.rot.Q_parent_this = attach_state.rot.Q_parent_this;
    }

    void clear_attachment()
    {
        active = false;
    }

    bool isAttached() const
    {
        return active;
    }

    RefFrame * get_parent_frame() const
    {
        return rigid_attach_parent;
    }

    const RefFrameState & get_attach_offset() const
    {
        return rigid_attach_state;
    }

private:
    // Member data

    bool active{}; //!< trick_units(--)

    RefFrame * rigid_attach_parent{}; //!< trick_units(--)

    RefFrameState rigid_attach_state; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
