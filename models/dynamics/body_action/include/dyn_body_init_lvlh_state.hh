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
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/include/dyn_body_init_lvlh_state.hh
 * Define the class DynBodyInitLvlhState, the base class for initializing
 * selected aspects of a vehicle's state with respect to some vehicle's
 * LVLH frame.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/dyn_body_init_lvlh_state.cc))



*******************************************************************************/

#ifndef JEOD_DYN_BODY_INIT_LVLH_STATE_HH
#define JEOD_DYN_BODY_INIT_LVLH_STATE_HH

// System includes

// JEOD includes
#include "utils/lvlh_frame/include/lvlh_frame.hh"
#include "utils/lvlh_frame/include/lvlh_type.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "dyn_body_init_planet_derived.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Initialize selected aspects of a vehicle's state with respect to some
 * vehicle's LVLH frame.
 */
class DynBodyInitLvlhState : public DynBodyInitPlanetDerived
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DynBodyInitLvlhState)

    // Member data

public:
    /**
     * Indicates type of LVLH coordinates desired. Default is rectilinear.
     */
    LvlhType::Type lvlh_type{LvlhType::Rectilinear}; //!< trick_units(--)

private:
    /**
     * A pointer to an LvlhFrame which can be supplied by the user.
     */
    LvlhFrame * lvlh_object_ptr{}; //!< trick_units(--)

    // Member functions

public:
    DynBodyInitLvlhState();
    ~DynBodyInitLvlhState() override = default;
    DynBodyInitLvlhState(const DynBodyInitLvlhState &) = delete;
    DynBodyInitLvlhState & operator=(const DynBodyInitLvlhState &) = delete;

    // set_lvlh_frame_object: Sets a pointer to user-supplied LvlhFrame
    void set_lvlh_frame_object(LvlhFrame & lvh_frame_object);

    // initialize: Initialize the initializer.
    void initialize(DynManager & dyn_manager) override;

    // apply: Apply the state to the subject body.
    void apply(DynManager & dyn_manager) override;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
