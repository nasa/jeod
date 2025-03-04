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
 * @addtogroup Environment
 * @{
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/MET/include/MET_atmosphere_state_vars.hh
 * Implement the MET atmosphere state variables using the atmosphere framework
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
   (Provides the set of variables that comprise a METAtmosphereState; this
    architecture supports the implementation of a local set of variables in
    METAtmosphere, while giving the actual METAtmosphereState access to the
    METAtmosphere.)
LIBRARY DEPENDENCY:
   (../src/MET_atmosphere_state_vars.cc)

*******************************************************************************/

#ifndef JEOD_MET_ATMOSPHERE_STATE_VARS_HH
#define JEOD_MET_ATMOSPHERE_STATE_VARS_HH

// System includes

// JEOD includes
#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_posn.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "environment/atmosphere/base_atmos/include/atmosphere.hh"
#include "environment/atmosphere/base_atmos/include/atmosphere_state.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The data variables component of the MET specific implementation
 * of AtmosphereState.
 */

class METAtmosphereStateVars : public AtmosphereState
{
    JEOD_MAKE_SIM_INTERFACES(jeod, METAtmosphereStateVars)

public:
    double exo_temp{};   /*!< trick_units(K) Exospheric temperature */
    double log10_dens{}; /*!< trick_units(--) Log10( total density ) */
    double mol_weight{}; /*!< trick_units(--) Average molecular weight */
    double N2{};         /*!< trick_units(--) N2 number density */
    double Ox2{};        /*!< trick_units(--) O2 number density */
    double Ox{};         /*!< trick_units(--) O number density */
    double A{};          /*!< trick_units(--) A number density */
    double He{};         /*!< trick_units(--) He number density */
    double Hyd{};        /*!< trick_units(--) H number density */

    // constructor, destructor, copy-constructor, operator=
    METAtmosphereStateVars() = default;
    METAtmosphereStateVars(Atmosphere & atmos_model, const PlanetFixedPosition & pfix_pos);
    ~METAtmosphereStateVars() override = default;
    METAtmosphereStateVars(const METAtmosphereStateVars & rhs);
    METAtmosphereStateVars & operator=(const METAtmosphereStateVars & rhs);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
