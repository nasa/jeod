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
 * @file models/dynamics/derived_state/include/lvlh_relative_derived_state.hh
 * Define the class LvlhRelativeDerivedState, the class used for calculating
 * the LVLH state of a subject DynBody relative to some LVLH reference frame.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/lvlh_relative_derived_state.cc))



*******************************************************************************/

#ifndef JEOD_LVLH_RELATIVE_DERIVED_STATE_HH
#define JEOD_LVLH_RELATIVE_DERIVED_STATE_HH

// System includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "environment/planet/include/base_planet.hh"
#include "utils/lvlh_frame/include/lvlh_type.hh"
#include "utils/ref_frames/include/class_declarations.hh"
#include "utils/ref_frames/include/ref_frame_state.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "relative_derived_state.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The class used for calculating the LVLH state of a subject DynBody
 * relative to some LVLH reference frame.
 */
class LvlhRelativeDerivedState : public RelativeDerivedState
{
    JEOD_MAKE_SIM_INTERFACES(jeod, LvlhRelativeDerivedState)

    // Member data

public:
    /**
     * Indicates type of LVLH coordinates desired. Default is rectilinear.
     */
    LvlhType::Type lvlh_type{LvlhType::Rectilinear}; //!< trick_units(--)

    /**
     * Indicates whether or not to correct for changing phase angle in
     * curvilinear coordinates. Default is false.
     */
    bool use_theta_dot_correction{}; //!< trick_units(--)

    // Methods

public:
    // Default constructor and destructor
    LvlhRelativeDerivedState();
    ~LvlhRelativeDerivedState() override = default;
    LvlhRelativeDerivedState(const LvlhRelativeDerivedState &) = delete;
    LvlhRelativeDerivedState & operator=(const LvlhRelativeDerivedState &) = delete;

    // initialize(): Initialize the LvlhRelativeDerivedState instance
    void initialize(DynBody & subject_body, DynManager & dyn_manager) override;

    // update(): Compute the LVLH relative state
    void update() override;

    // Convert between types of LVLH coordinates
    void convert_rect_to_circ(const RefFrameState & rect_rel_state);
    void convert_circ_to_rect(const RefFrameState & circ_rel_state);

private:
    // Method to correct omega for variable phase angle
    void do_theta_dot_correction(double omega[3], const RefFrameState & state, const double r, bool c2r);
};

} // namespace jeod

#ifdef TRICK_VER
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
