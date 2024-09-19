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
 * @file models/dynamics/body_action/include/body_detach.hh
 * Define the class MassBodyDetach, the base class used for detaching one
 * MassBody object from one another.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/body_detach.cc))



*******************************************************************************/

#ifndef JEOD_MASS_BODY_DETACH_HH
#define JEOD_MASS_BODY_DETACH_HH

// System includes

// JEOD includes
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "body_action.hh"
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Provides the basic ability to detach one MassBody from another.
 * This is inherently an asynchronous BodyAction.
 * The is_ready() method simply returns the action's active flag.
 * The action will be performed when the sim user or some simulation job enables
 * the active flag.
 *
 * The basic detachment action is to cause a body to detach from its
 * immediate parent body. Subclasses can cause bodies to detach elsewhere.
 */
class BodyDetach : public BodyAction
{
    JEOD_MAKE_SIM_INTERFACES(jeod, BodyDetach)

    // Member data

    // All requisite data for a direct detachment are in the parent classes.

    // Member functions

public:
    BodyDetach();
    ~BodyDetach() override = default;
    BodyDetach(const BodyDetach &) = delete;
    BodyDetach & operator=(const BodyDetach &) = delete;

    // apply: Detach the body from its parent body.
    void apply(DynManager & dyn_manager) override;

    // is_ready: Is the action ready? (In this case, is the active flag set?)
    bool is_ready() override;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
