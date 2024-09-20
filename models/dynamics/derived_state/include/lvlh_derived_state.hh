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
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/include/lvlh_derived_state.hh
 * Define the class LvlhDerivedState, the class used for deriving the
 * rectilinear LVLH representations of a subject DynBody's state.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/lvlh_derived_state.cc))



*******************************************************************************/

#ifndef JEOD_LVLH_DERIVED_STATE_HH
#define JEOD_LVLH_DERIVED_STATE_HH

// System includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "utils/lvlh_frame/include/lvlh_frame.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "derived_state.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The class used for deriving the rectilinear LVLH representations
 * of a subject DynBody's state.
 */
class LvlhDerivedState : public DerivedState
{
    JEOD_MAKE_SIM_INTERFACES(jeod, LvlhDerivedState)

    // Member data

public:
    /**
     * If set (default), the LVLH frame will be registered with the dynamics
     * manager at initialization time. This will make the frame accessible
     * through the dynamic manager via its find_ref_frame method.
     */
    bool register_frame{true}; //!< trick_units(--)

    /**
     * The LVLH frame of the subject body with respect to the planet
     * specified by the reference name.
     */
    RefFrame lvlh_frame; //!< trick_units(--)

    /**
     * The LvlhFrame object responsible for maintaining the lvlh_frame.
     */
    LvlhFrame lvlh_state; //!< trick_units(--)

protected:
    /**
     * The inertial frame with origin at the center of the specified planet.
     */
    RefFrame * planet_centered_inertial{}; //!< trick_units(--)

    DynManager * local_dm{};

    // Methods

public:
    // Default constructor and destructor
    LvlhDerivedState() = default;
    ~LvlhDerivedState() override;
    LvlhDerivedState(const LvlhDerivedState &) = delete;
    LvlhDerivedState & operator=(const LvlhDerivedState &) = delete;

    // initialize(): Initialize the DerivedState (but not necessarily the
    // state itself.)
    // Rules for derived classes:
    // All derived classes must forward the initialize() call to the immediate
    // parent class and then perform class-dependent object initializations.
    void initialize(DynBody & subject_body, DynManager & dyn_manager) override;

    // update(): Update the DerivedState representation of the subject DynBody.
    // Rules for derived classes:
    // All derived classes must perform class-dependent actions and then
    // must forward the update() call to the immediate parent class.
    void update() override;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
