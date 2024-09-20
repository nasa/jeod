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
 * @addtogroup Environment
 * @{
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/MET/include/MET_atmosphere_state.hh
 * Implement the MET atmosphere state using the atmosphere framework
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
   (Provides the specific implementation of the atmosphere state for a
   MET atmosphere.)
ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.)
    (Only an METAtmosphere pointer should be given to an METAtmosphereState
     object for the update_state call. Otherwise there can be unexpected
     behavior))
Library dependencies:
   (../src/MET_atmosphere_state.cc)

*******************************************************************************/

#ifndef JEOD_MET_ATMOSPHERE_STATE_HH
#define JEOD_MET_ATMOSPHERE_STATE_HH

// System includes

// JEOD includes
#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_posn.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "MET_atmosphere.hh"
#include "MET_atmosphere_state_vars.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The MET specific implementation of AtmosphereState.
 */

class METAtmosphereState : public METAtmosphereStateVars
{
    JEOD_MAKE_SIM_INTERFACES(jeod, METAtmosphereState)

private:
    METAtmosphere * met_atmos{}; /* !<trick_units(--) Pointer to the METAtmosphere relative to which this class will
                                    provide a state.*/

public: // public member functions
    // non-default constructor, in preparation for shift to references over
    // pointers:
    METAtmosphereState(METAtmosphere & atmos_model, const PlanetFixedPosition & pfix_pos);

    METAtmosphereState() = default;
    ~METAtmosphereState() override = default;
    METAtmosphereState & operator=(const METAtmosphereState &) = delete;
    METAtmosphereState(const METAtmosphereState &) = delete;

    // Updates this particular atmosphere state from an METAtmosphere
    // atmosphere model.
    //  - version 1 (deprecated) brings in pointers to the external models,
    //  - version 2 uses internal pointers set in the constructor; this is
    //    provided for an eventual path toward using references instead of
    //    pointers.
    void update_state(METAtmosphere * atmos_model, const PlanetFixedPosition * pfix_pos);
    void update_state() override;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
