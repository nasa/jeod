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
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/include/body_attach.hh
 * Define the class MassBodyAttach, the base class used for attaching a pair
 * of MassBody objects to one another.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/body_attach.cc))



*******************************************************************************/

#ifndef JEOD_MASS_BODY_ATTACH_HH
#define JEOD_MASS_BODY_ATTACH_HH

// System includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "dynamics/mass/include/class_declarations.hh"
#include "utils/ref_frames/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "body_action.hh"
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Provides the basic ability to attach one MassBody to another.
 * This can be either an initialization or asynchronous BodyAction.
 * The action will be performed when the sim user or some simulation job
 * enables the active flag.
 *
 * MassBodyAttach actions that are ready at simulation initialization time
 * are run as a part of the initialization process, sandwiched between
 * initializing mass properties and initializing state. Attach actions
 * that are not ready at initialization time remain in the pending actions
 * queue until the active flag is set.
 */
class BodyAttach : public BodyAction
{
    JEOD_MAKE_SIM_INTERFACES(jeod, BodyAttach)

    // Member data

public:
    /**
     * Set the parent mass body of this action. Resets dyn_parent, frame_parent to null
     */
    void set_parent_body(MassBody & mass_body_in);

    /**
     * Set the parent dyn body of this action. Resets mass_parent, frame_parent to null
     */
    void set_parent_body(DynBody & dyn_body_in);

    /**
     * Set the parent ref frame of this action. Resets mass_parent, dyn_parent to null
     */
    void set_parent_frame(RefFrame & ref_parent_in);

    /**
     * Did the attachment succeed?
     */
    bool succeeded{}; //!< trick_units(--)

protected:
    /**
     * The MassBody corresponding to which the subject body is to be attached,
     * directly if the subject body is a root body, and indirectly by attaching
     * the subject body's root body to the parent body otherwise.
     * This pointer is one of ithe 3 possible pointers that must be supplied.
     */
    MassBody * mass_parent{}; //!< trick_units(--)

    /**
     * The DynBody corresponding to which the subject body is to be attached,
     * directly if the subject body is a root body, and indirectly by attaching
     * the subject body's root body to the parent body otherwise.
     * This pointer is one of ithe 3 possible pointers that must be supplied.
     */
    DynBody * dyn_parent{}; //!< trick_units(--)

    /**
     * The RefFrame corresponding to which the subject body is to be attached,
     * directly if the subject body is a root body, and indirectly by attaching
     * the subject body's root body to the parent RefFrame otherwise.
     * This pointer is one of ithe 3 possible pointers that must be supplied.
     */
    RefFrame * ref_parent{}; //!< trick_units(--)

public:
    BodyAttach() = default;
    ~BodyAttach() override = default;
    BodyAttach(const BodyAttach &) = delete;
    BodyAttach & operator=(const BodyAttach &) = delete;

    // initialize: Initialize the initializer.
    void initialize(DynManager & dyn_manager) override;

    // apply: Forward the apply call up the class heirarchy.
    void apply(DynManager & dyn_manager) override;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
