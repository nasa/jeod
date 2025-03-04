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
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/include/planetary_derived_state.hh
 * Define the class PlanetaryDerivedState, the class used for deriving the
 * planet-fixed representations of a subject DynBody's position.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/planetary_derived_state.cc))



*******************************************************************************/

#ifndef JEOD_PLANETARY_DERIVED_STATE_HH
#define JEOD_PLANETARY_DERIVED_STATE_HH

// System includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "environment/planet/include/class_declarations.hh"
#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_posn.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "derived_state.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The class used for deriving the planet-fixed representations
 * of a subject DynBody's position.
 */
class PlanetaryDerivedState : public DerivedState
{
    JEOD_MAKE_SIM_INTERFACES(jeod, PlanetaryDerivedState)

    // Member data

public:
    /**
     * The planet-fixed state of the subject body's composite CoM.
     */
    PlanetFixedPosition state; //!< trick_units(--)

    /**
     * The planet, the name of which is specified by the inherited
     * reference_name data member.
     */
    Planet * planet{}; //!< trick_units(--)

protected:
    /**
     * Use pfix or alt_pfix flag
     */
    bool use_alt_pfix{};

    /**
     * Pointer to planet fixed frame to be used, either
     * pfix or alt_pfix
     */
    EphemerisRefFrame * pfix_ptr{};

    // Methods

public:
    // Default constructor
    PlanetaryDerivedState() = default;
    ~PlanetaryDerivedState() override;
    PlanetaryDerivedState(const PlanetaryDerivedState &) = delete;
    PlanetaryDerivedState & operator=(const PlanetaryDerivedState &) = delete;

    // Setter for use_alt_pfix
    void set_use_alt_pfix(const bool use_alt_pfix_in);

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

#ifdef TRICK_VER
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
