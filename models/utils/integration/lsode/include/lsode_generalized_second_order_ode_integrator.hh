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
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 * @addtogroup Lsode
 * @{
 *
 * @file models/utils/integration/lsode/include/lsode_generalized_second_order_ode_integrator.hh
 * Define the class LsodeGeneralizedDerivSecondOrderODEIntegrator.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Library dependencies:
  ((../src/lsode_generalized_second_order_ode_integrator.cc)
   (../src/lsode_second_order_ode_integrator.cc))



*******************************************************************************/

#ifndef JEOD_LSODE_GENERALIZED_DERIV_SECOND_ORDER_ODE_INTEGRATOR_HH
#define JEOD_LSODE_GENERALIZED_DERIV_SECOND_ORDER_ODE_INTEGRATOR_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Integration includes
#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"

// Model includes
#include "lsode_second_order_ode_integrator.hh"

namespace jeod
{

class LsodeControlDataInterface;

/**
 * JEOD-compatible version of the Livermore ODE solver, LSODE, capable of
 * integrating second-order ODEs.
 */
class LsodeGeneralizedDerivSecondOrderODEIntegrator : public LsodeSecondOrderODEIntegrator
{
    JEOD_MAKE_SIM_INTERFACES(jeod, LsodeGeneralizedDerivSecondOrderODEIntegrator)

    // Methods:
public:
    // Constructors

    /**
     * LsodeGeneralizedDerivSecondOrderODEIntegrator default constructor.
     */
    LsodeGeneralizedDerivSecondOrderODEIntegrator() = default;

    /**
     * LsodeGeneralizedDerivSecondOrderODEIntegrator copy constructor.
     * @param[in] src  Item to be copied.
     */
    LsodeGeneralizedDerivSecondOrderODEIntegrator(const LsodeGeneralizedDerivSecondOrderODEIntegrator & src);

    LsodeGeneralizedDerivSecondOrderODEIntegrator(const LsodeControlDataInterface & data_in,
                                                  er7_utils::IntegrationControls & controls,
                                                  const er7_utils::GeneralizedPositionDerivativeFunctions & deriv_funs,
                                                  unsigned int position_size,
                                                  unsigned int velocity_size);

    /**
     * LsodeGeneralizedDerivSecondOrderODEIntegrator destructor.
     */
    ~LsodeGeneralizedDerivSecondOrderODEIntegrator() override;

    LsodeGeneralizedDerivSecondOrderODEIntegrator * create_copy() const override;

    /**
     * Propagate state via Lsode's method.
     * @param[in] dyn_dt        Integration interval step, dynamic time seconds.
     * @param[in] target_stage  The stage of the integration process
     *                          that the integrator should try to attain.
     * @param[in] accel         Generalized acceleration vector.
     * @param[in,out] velocity  Generalized velocity vector.
     * @param[in,out] position  Generalized position vector.
     *
     * @return The status (time advance, pass/fail status) of the integration.
     */
    er7_utils::IntegratorResult integrate(double dyn_dt,
                                          unsigned int target_stage,
                                          const double * ER7_UTILS_RESTRICT accel,
                                          double * ER7_UTILS_RESTRICT velocity,
                                          double * ER7_UTILS_RESTRICT position) override;

    LsodeGeneralizedDerivSecondOrderODEIntegrator & operator=(const LsodeGeneralizedDerivSecondOrderODEIntegrator &) =
        delete;

    // Variables

public:
    /**
     * Stash space for the result of the computation of the derivative of the
     * zeroth-derivative.  Used with the Generalized derivative form, in which
     * the deriviative of the zeroth derivative is not equal to the
     * first-derivative.
     */
    double * posdot{}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
