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
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/include/spring_pair_interaction.hh
 * A class for pair interactions based on a simple spring
 */

/*****************************************************************************

 Purpose:
     ()

 Reference:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
     ((N/A))

Library dependencies:
    ((../src/spring_pair_interaction.cc))



*****************************************************************************/

#ifndef SPRING_PAIR_INTERACTION_HH
#define SPRING_PAIR_INTERACTION_HH

/* JEOD includes */
#include "utils/sim_interface/include/jeod_class.hh"

/* Model includes */
#include "pair_interaction.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Simple spring contact parameters
 */
class SpringPairInteraction : public PairInteraction
{
    JEOD_MAKE_SIM_INTERFACES(jeod, SpringPairInteraction)

public:
    /**
     * Spring stiffness constant.
     */
    double spring_k{}; //!< trick_units(N/m)

    /**
     * Spring damping constant.
     */
    double damping_b{}; //!< trick_units(N*s/m)

    /**
     * Coefficent of friction.
     */
    double mu{}; //!< trick_units(--)

    SpringPairInteraction() = default;
    ~SpringPairInteraction() override = default;
    SpringPairInteraction & operator=(const SpringPairInteraction &) = delete;
    SpringPairInteraction(const SpringPairInteraction &) = delete;

    /* force calculation function */
    void calculate_forces(ContactFacet * subject,
                          ContactFacet * target,
                          RelativeDerivedState * rel_state,
                          double * penetration_vector,
                          double * rel_velocity) override;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
