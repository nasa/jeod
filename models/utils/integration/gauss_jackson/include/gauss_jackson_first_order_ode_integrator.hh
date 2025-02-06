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
 * @addtogroup GaussJackson
 * @{
 *
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_first_order_ode_integrator.hh
 * Defines the class GaussJacksonFirstOrderODEIntegrator,
 * which integrates a first order ODE using the summed Adams technique.
 *  *
 * Note: This is a header-only implementation. There is no source file
 * that corresponds to this header.
 */

/*
Purpose: ()
*/

#ifndef JEOD_GAUSS_JACKSON_FIRST_ORDER_ODE_INTEGRATOR_HH
#define JEOD_GAUSS_JACKSON_FIRST_ORDER_ODE_INTEGRATOR_HH

// Local includes
#include "gauss_jackson_integrator_base_first.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/first_order_ode_integrator.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Integrates a first order ODE using the summed Adams technique
 * that is embedded within the Gauss-Jackson technique.
 */
class GaussJacksonFirstOrderODEIntegrator : public er7_utils::FirstOrderODEIntegrator,
                                            public GaussJacksonIntegratorBaseFirst
{ // changed from private to public to fixed "cannot access private member error when move into namespace"

    JEOD_MAKE_SIM_INTERFACES(jeod, GaussJacksonFirstOrderODEIntegrator)

public:
    GaussJacksonFirstOrderODEIntegrator() = default;
    ~GaussJacksonFirstOrderODEIntegrator() override = default;

    /**
     * Non-default constructor.
     * @param  priming_constructor  Integrator constructor for the technique
     *                              used during priming.
     * @param  controls             The Gauss-Jackson integration controls that
     *                              drives this state integrator.
     * @param  size_in              State size.
     * @param  priming_controls     Integration controls used during priming.
     */
    GaussJacksonFirstOrderODEIntegrator(const er7_utils::IntegratorConstructor & priming_constructor,
                                        GaussJacksonIntegrationControls & controls,
                                        unsigned int size_in,
                                        er7_utils::IntegrationControls & priming_controls)
        : er7_utils::FirstOrderODEIntegrator(size_in, controls),
          GaussJacksonIntegratorBaseFirst(priming_constructor, controls, size_in, priming_controls)
    {
    }

    /**
     * Copy constructor.
     */
    GaussJacksonFirstOrderODEIntegrator(const GaussJacksonFirstOrderODEIntegrator & src)
        : er7_utils::FirstOrderODEIntegrator(src),
          GaussJacksonIntegratorBaseFirst(src)
    {
    }

    /**
     * Assignment operator.
     */
    GaussJacksonFirstOrderODEIntegrator & operator=(GaussJacksonFirstOrderODEIntegrator src)
    {
        swap(src);
        return *this;
    }

    /**
     * Non-throwing swap.
     */
    void swap(GaussJacksonFirstOrderODEIntegrator & other)
    {
        FirstOrderODEIntegrator::swap(other);
        GaussJacksonIntegratorBaseFirst::swap(other);
    }

    /**
     * Replicate this.
     */
    er7_utils::FirstOrderODEIntegrator * create_copy() const override
    {
        return er7_utils::alloc::replicate_object(*this);
    }

    /**
     * Reset the integrator.
     */
    void reset_integrator() override
    {
        base_reset();
    }

    /**
     * Integrate.
     */
    er7_utils::IntegratorResult integrate(double dyn_dt,
                                          unsigned int target_stage,
                                          const double * ER7_UTILS_RESTRICT deriv,
                                          double * ER7_UTILS_RESTRICT state) override
    {
        GaussJacksonOneState gjState(state);
        return base_integrate(dyn_dt, target_stage, deriv, gjState);
    }

private:
    using FirstOrderODEIntegrator::swap;
    using GaussJacksonIntegratorBaseFirst::swap;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
