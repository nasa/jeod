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
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 * @addtogroup Lsode
 * @{
 *
 * @file models/utils/integration/lsode/include/lsode_integrator_constructor.hh
 * Define the class LsodeIntegratorConstructor, the class that constructs the
 * integration controls and the integrators for the LSODE method.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  (())

Library dependencies:
  ((../src/lsode_integrator_constructor.cc)
   (../src/lsode_integration_controls.cc))



*******************************************************************************/

#ifndef JEOD_LSODE_INTEGRATOR_CONSTRUCTOR_HH
#define JEOD_LSODE_INTEGRATOR_CONSTRUCTOR_HH

// System includes
#include <vector>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Integration includes
#include "er7_utils/integration/core/include/integrator_constructor.hh"

// Model includes
#include "lsode_control_data_interface.hh"
#include "lsode_first_order_ode_integrator.hh"
#include "lsode_generalized_second_order_ode_integrator.hh"
#include "lsode_second_order_ode_integrator.hh"
#include "lsode_simple_second_order_ode_integrator.hh"

namespace jeod
{

/**
 * Create state and time integrators that propagate using standard Lsode.
 */
class LsodeIntegratorConstructor : public er7_utils::IntegratorConstructor
{
    JEOD_MAKE_SIM_INTERFACES(jeod, LsodeIntegratorConstructor)

    // Note: The default constructor, copy constructor, assignment operator,
    // and destructor for this class are not declared.
    // The implicit C++ defaults suffice for this class.

public:
    LsodeIntegratorConstructor() = default;
    LsodeIntegratorConstructor(const LsodeIntegratorConstructor & src);
    LsodeIntegratorConstructor & operator=(const LsodeIntegratorConstructor &) = delete;

    // Static member functions.

    /**
     * Named constructor; create an LsodeIntegratorConstructor instance.
     * The caller is responsible for deleting the returned object.
     * @return Newly created LsodeIntegratorConstructor instance.
     */
    static er7_utils::IntegratorConstructor * create_constructor();

    // Member functions.

    /**
     * Return the class name.
     */
    const char * get_class_name() const override
    {
        return "LsodeIntegratorConstructor";
    }

    /**
     * Lsode currently does not implement a second order generalized step
     * integrator.
     */
    bool implements(er7_utils::Integration::ODEProblemType problem_type) const override
    {
        return (problem_type != er7_utils::Integration::GeneralizedDerivSecondOrderODE) &&
               (problem_type != er7_utils::Integration::GeneralizedStepSecondOrderODE);
    }

    /**
     * Lsode currently does not provide a second order generalized step
     * integrator.
     */
    bool provides(er7_utils::Integration::ODEProblemType problem_type) const override
    {
        return (problem_type != er7_utils::Integration::GeneralizedDerivSecondOrderODE) &&
               (problem_type != er7_utils::Integration::GeneralizedStepSecondOrderODE);
    }

    /**
     * Create a duplicate of the constructor.
     * The caller is responsible for deleting the returned object.
     * @return Duplicated constructor.
     */
    er7_utils::IntegratorConstructor * create_copy() const override;

    /**
     * Create an integration controls that guides the Lsode integration process.
     * The caller is responsible for deleting the created object.
     * @return Integration controls object
     */
    er7_utils::IntegrationControls * create_integration_controls() const override;

    /**
     * Create an Lsode state integrator for a first order ODE.
     * The caller is responsible for deleting the created object.
     * @return State integrator
     * @param[in]     size      State size
     * @param[in,out] controls  Integration controls
     */
    er7_utils::FirstOrderODEIntegrator * create_first_order_ode_integrator(
        unsigned int size, er7_utils::IntegrationControls & controls) const override;

    /**
     * Create an Lsode state integrator for a simple second order ODE.
     * The caller is responsible for deleting the created object.
     * @return State integrator
     * @param[in]     size      State size
     * @param[in,out] controls  Integration controls
     */
    er7_utils::SecondOrderODEIntegrator * create_second_order_ode_integrator(
        unsigned int size, er7_utils::IntegrationControls & controls) const override;

    /**
     * Create an Lsode state integrator for a generalized second order ODE
     * where generalized position is advanced with the use of the
     * position derivative function.
     * The caller is responsible for deleting the created object.
     * @return State integrator
     * @param[in]     position_size  Size of the generalized position
     * @param[in]     velocity_size  Size of the generalized velocity
     * @param[in]     deriv_funs     Position derivative functions container
     * @param[in,out] controls       Integration controls
     */
    er7_utils::SecondOrderODEIntegrator * create_generalized_deriv_second_order_ode_integrator(
        unsigned int position_size,
        unsigned int velocity_size,
        const er7_utils::GeneralizedPositionDerivativeFunctions & deriv_funs,
        er7_utils::IntegrationControls & controls) const override;

    /**
     * Lsode dioes not use a linear transition table
     * @return Always returns 0.
     */
    unsigned int get_transition_table_size() const override
    {
        return 0;
    }

    LsodeControlDataInterface data_interface;
};

} // namespace jeod

#ifdef TRICK_ICG
#include "er7_utils/integration/core/include/single_cycle_integration_controls.hh"
// #include "lsode_first_order_ode_integrator.hh"
// #include "lsode_second_order_ode_integrator.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
