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
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_simple_second_order_ode_integrator.hh
 * Defines the class GaussJacksonSimpleSecondOrderODEIntegrator,
 * which integrates a simple second order ODE using Gauss-Jackson.
 */

/*
Purpose: ()
*/

#ifndef JEOD_GAUSS_JACKSON_SIMPLE_SECOND_ORDER_STATE_INTEGRATOR_HH
#define JEOD_GAUSS_JACKSON_SIMPLE_SECOND_ORDER_STATE_INTEGRATOR_HH

// Local includes
#include "gauss_jackson_integrator_base_second.hh"
#include "gauss_jackson_two_state.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Integrates a simple second order ODE using the Gauss-Jackson technique.
 *
 * The class inherits from er7_utils::SecondOrderODEIntegrator as an is-a
 * relationship (public inheritance) and from GaussJacksonIntegratorBaseSecond
 * as an is-implemented-by relationship (private inheritance).
 * Using composition instead of private inheritance would make Trick 13
 * checkpoint/restart a lot trickier to implement. With private inheritance,
 * the Trick 13 io_src file contains all the necessary information.
 */
class GaussJacksonSimpleSecondOrderODEIntegrator : public er7_utils::SecondOrderODEIntegrator,
                                                   public GaussJacksonIntegratorBaseSecond
{ // changed from private to public to fixed "cannot access private member error when move into namespace"
    JEOD_MAKE_SIM_INTERFACES(jeod, GaussJacksonSimpleSecondOrderODEIntegrator)

public:
    GaussJacksonSimpleSecondOrderODEIntegrator() = default;
    ~GaussJacksonSimpleSecondOrderODEIntegrator() override = default;

    /**
     * Non-default constructor. This is the constructor invoked by the
     * GaussJacksonIntegratorConstructor.
     * @param  priming_constructor  Integrator constructor for the technique
     *                              used during priming.
     * @param  controls             The Gauss-Jackson integration controls that
     *                              drives this state integrator.
     * @param  size_in              State size.
     * @param  priming_controls     Integration controls used during priming.
     */
    GaussJacksonSimpleSecondOrderODEIntegrator(const er7_utils::IntegratorConstructor & priming_constructor,
                                               GaussJacksonIntegrationControls & controls,
                                               unsigned int size_in,
                                               er7_utils::IntegrationControls & priming_controls)
        : er7_utils::SecondOrderODEIntegrator(size_in, controls),
          GaussJacksonIntegratorBaseSecond(priming_constructor, controls, size_in, priming_controls)
    {
    }

    /**
     * Copy constructor.
     * @param  src  Item to be copied.
     */
    GaussJacksonSimpleSecondOrderODEIntegrator(const GaussJacksonSimpleSecondOrderODEIntegrator & src)
        : er7_utils::SecondOrderODEIntegrator(src),
          GaussJacksonIntegratorBaseSecond(src)
    {
    }

    /**
     * Copy and swap assignment operator.
     * @param  src  Item to be copied.
     */
    GaussJacksonSimpleSecondOrderODEIntegrator & operator=(GaussJacksonSimpleSecondOrderODEIntegrator src)
    {
        swap(src);
        return *this;
    }

    /**
     * Non-throwing swap.
     * @param  other  Item whose contents are to be swapped with this.
     */
    void swap(GaussJacksonSimpleSecondOrderODEIntegrator & other)
    {
        SecondOrderODEIntegrator::swap(other);
        GaussJacksonIntegratorBaseSecond::swap(other);
    }

    /**
     * Replicate this.
     * @return Replicate of this.
     */
    er7_utils::SecondOrderODEIntegrator * create_copy() const override
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
     * Propagate state using Gauss-Jackson.
     * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
     * @param[in]     target_stage  The stage of the integration process
     *                              that the integrator should try to attain.
     * @param[in]     acc           Acceleration vector.
     * @param[in,out] vel           Velocity vector.
     * @param[in,out] pos           Position vector.
     *
     * @return The status (time advance, pass/fail status) of the integration.
     */
    er7_utils::IntegratorResult integrate(double dyn_dt,
                                          unsigned int target_stage,
                                          const double * ER7_UTILS_RESTRICT acc,
                                          double * ER7_UTILS_RESTRICT vel,
                                          double * ER7_UTILS_RESTRICT pos) override
    {
        return base_integrate(dyn_dt, target_stage, acc, GaussJacksonTwoState(vel, pos));
    }

private:
    using GaussJacksonIntegratorBaseSecond::swap;
    using SecondOrderODEIntegrator::swap;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
