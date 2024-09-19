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
 * @file models/utils/integration/lsode/include/lsode_second_order_ode_integrator.hh
 * Define the class LsodeSecondOrderODEIntegrator.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Library dependencies:
  ((../src/lsode_second_order_ode_integrator.cc))



*******************************************************************************/

#ifndef JEOD_LSODE_SECOND_ORDER_ODE_INTEGRATOR_HH
#define JEOD_LSODE_SECOND_ORDER_ODE_INTEGRATOR_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Integration includes
#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"

// Model includes
#include "lsode_first_order_ode_integrator.hh"
#include "lsode_integration_controls.hh"

namespace jeod
{

class LsodeControlDataInterface;

/**
 * JEOD-compatible version of the Livermore ODE solver, LSODE, capable of
 * integrating second-order ODEs.
 */
class LsodeSecondOrderODEIntegrator : public er7_utils::SecondOrderODEIntegrator
{
    JEOD_MAKE_SIM_INTERFACES(jeod, LsodeSecondOrderODEIntegrator)

    // Methods:
public:
    /**
     * LsodeSecondOrderODEIntegrator destructor.
     */
    ~LsodeSecondOrderODEIntegrator() override;

    LsodeSecondOrderODEIntegrator & operator=(const LsodeSecondOrderODEIntegrator &) = delete;
    LsodeSecondOrderODEIntegrator(const LsodeSecondOrderODEIntegrator &) = delete;

    // Member functions.

    /**
     * Get the integrator's reentry point.
     */
    int get_re_entry_point()
    {
        return first_order_integrator.get_re_entry_point();
    }

protected:
    LsodeSecondOrderODEIntegrator() = default;

    /**
     * LsodeSecondOrderODEIntegrator non-default constructor.
     * @param[in]     data_in   LSODE-specific control data.
     * @param[in,out] controls  Integration controls.
     * @param[in]     size      State size.
     */
    LsodeSecondOrderODEIntegrator(const LsodeControlDataInterface & data_in,
                                  er7_utils::IntegrationControls & controls,
                                  unsigned int size);

    LsodeSecondOrderODEIntegrator(const LsodeControlDataInterface & data_in,
                                  er7_utils::IntegrationControls & controls,
                                  const er7_utils::GeneralizedPositionDerivativeFunctions & deriv_funs,
                                  unsigned int position_size,
                                  unsigned int velocity_size);

    /**
     * Reset the integrator.
     */
    void reset_integrator() override
    {
        first_order_integrator.reset_integrator();
    }

    // Variables:

public:
    // State
    /**
     * State vector (zeroth derivative).
     */
    double * y{}; //!< trick_units(--)

    /**
     * State vector (first derivative).
     */
    double * y_dot{}; //!< trick_units(--)

    unsigned int zeroth_derivative_size{};
    unsigned int first_derivative_size{};

    LsodeFirstOrderODEIntegrator first_order_integrator;

    bool arrays_allocated{};
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
