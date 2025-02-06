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
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_integration_controls.hh
 * Defines the class GaussJacksonIntegrationControls, which controls
 * Gauss-Jackson integration process.
 */

/*
Purpose: ()
Library dependencies:
  ((../src/gauss_jackson_integration_controls.cc))
*/

#ifndef JEOD_GAUSS_JACKSON_INTEGRATION_CONTROLS_HH
#define JEOD_GAUSS_JACKSON_INTEGRATION_CONTROLS_HH

// Local includes
#include "gauss_jackson_coeffs.hh"
#include "gauss_jackson_config.hh"
#include "gauss_jackson_state_machine.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integration_controls.hh"

/**
 * Namespace er7_utils contains the state integration models used by JEOD.
 */
namespace er7_utils
{
class IntegratorConstructor;
} // namespace er7_utils

//! Namespace jeod
namespace jeod
{

/**
 * IntegrationControls specialized for Gauss-Jackson integration.
 */
class GaussJacksonIntegrationControls : public er7_utils::IntegrationControls
{
    JEOD_MAKE_SIM_INTERFACES(jeod, GaussJacksonIntegrationControls)

public:
    /**
     * Default constructor.
     */
    GaussJacksonIntegrationControls();

    /**
     * Non-default constructor. This is the constructor invoked by the
     * GaussJacksonIntegratorConstructor.
     * @param  priming_constructor  Integrator constructor for the technique
     *                              used during priming.
     * @param config_in             Gauss-Jackson configuration data.
     */
    GaussJacksonIntegrationControls(const er7_utils::IntegratorConstructor & priming_constructor,
                                    const GaussJacksonConfig & config_in);

    /**
     * Copy constructor.
     * @param  src  Object to be copied.
     */
    GaussJacksonIntegrationControls(const GaussJacksonIntegrationControls & src);

    /**
     * Destructor.
     */
    ~GaussJacksonIntegrationControls() override;

    /**
     * Copy and swap assignment operator.
     * @param  src  Object to be copied.
     */
    GaussJacksonIntegrationControls & operator=(GaussJacksonIntegrationControls src)
    {
        swap(src);
        return *this;
    }

    /**
     * Create a duplicate of this object.
     * @return Replicated GaussJacksonIntegrationControls.
     */
    er7_utils::IntegrationControls * create_copy() const override;

    /**
     * Getter for the priming_controls data member.
     * @return Reference to the integration controls used during priming.
     */
    er7_utils::IntegrationControls & get_priming_controls() const
    {
        return *priming_controls;
    }

    /**
     * Getter for the coeff data member.
     * @return Reference to the Gauss-Jackson coefficients object.
     */
    const GaussJacksonCoeffs & get_coeff() const
    {
        return coeff;
    }

    /**
     * Getter for the config data member.
     * @return Reference to the Gauss-Jackson configuration object.
     */
    const GaussJacksonConfig & get_config() const
    {
        return config;
    }

    /**
     * Getter for the state_machine data member.
     * @return Reference to the Gauss-Jackson state_machine object.
     */
    const GaussJacksonStateMachine & get_state_machine() const
    {
        return state_machine;
    }

    /**
     * Reset the integration controls object.
     */
    void reset_integrator() override;

    /**
     * Make one step in the process that eventually integrates state from
     * the start_time to start_time+sim_dt.
     * @return Step number; zero when finished.
     * @param[in] start_time
     *    The simulation engine time at the start of the integration tour.
     * @param[in] sim_dt
     *    The difference between the simulation time at the end and start of the
     *    integration tour.
     * @param[in,out] time_interface
     *    Object external to the ER7 utilities suite that represents time.
     * @param[in,out] integ_interface
     *    Interface with the simulation engine for this integration controls.
     * @param[in,out] integ_group
     *    The integration group that contains this integration controls.
     */
    unsigned int integrate(double start_time,
                           double sim_dt,
                           er7_utils::TimeInterface & time_interface,
                           er7_utils::IntegratorInterface & integ_interface,
                           er7_utils::BaseIntegrationGroup & integ_group) override;

protected:
    /**
     * Non-throwing swap function.
     * Swap contents of 'this' with that of the other.
     * @param[in] other  Item with which contents are to be swapped.
     */
    virtual void swap(GaussJacksonIntegrationControls & other);

    using er7_utils::IntegrationControls::swap;

private:
    /**
     * The integration controls object used to prime the Gauss-Jackson
     * integration process.
     */
    er7_utils::IntegrationControls * priming_controls{}; //!< trick_units(--)

    /**
     * The simulation time of the start of the current integration cycle.
     * An integration cycle starts when cycle_stage is zero and ends when
     * it reaches zero once again.
     */
    double cycle_starttime{}; //!< trick_units(--)

    /**
     * The simulation time span of the current integration cycle.
     */
    double cycle_simdt{}; //!< trick_units(--)

    /**
     * The dynamic time span corresponding to cycle_simdt.
     */
    double cycle_dyndt{}; //!< trick_units(s)

    /**
     * The simulation time of the most recent reset.
     */
    double reset_time{}; //!< trick_units(--)

    /**
     * The Gauss-Jackson corrector and predictor coefficients.
     */
    GaussJacksonCoeffs coeff; //!< trick_units(--)

    /**
     * The Gauss-Jackson configuration data.
     */
    GaussJacksonConfig config; //!< trick_units(--)

    /**
     * The Gauss-Jackson state machine.
     */
    GaussJacksonStateMachine state_machine; //!< trick_units(--)

    /**
     * The state machine's finite state.
     */
    GaussJacksonStateMachine::FsmState fsm_state{GaussJacksonStateMachine::Reset}; //!< trick_units(--)

    /**
     * Maximum number of correction iterations allowed.
     */
    unsigned int max_correction_iterations{}; //!< trick_units(--)

    /**
     * The order of the Gauss Jackson integrator immediately after priming.
     */
    unsigned int initial_order{}; //!< trick_units(--)

    /**
     * The current order of the Gauss Jackson integrator.
     */
    unsigned int order{}; //!< trick_units(--)

    /**
     * Number of times that the current set of history have been edited.
     */
    unsigned int edit_count{}; //!< trick_units(--)

    /**
     * Flag indicating that the current integration cycle is the last one in
     * an integration tour (i.e., that a major time step will be completed).
     */
    bool at_end_of_tour{}; //!< trick_units(--)

    // Member functions

    /**
     * Perform start of integration cycle actions.
     */
    void start_cycle(double sim_dt);

    /**
     * Guide integration while in BootstrapEdit mode.
     */
    void integrate_edit(er7_utils::TimeInterface & time_interface, er7_utils::BaseIntegrationGroup & integ_group);

    /**
     * Guide integration while in BootstrapStep or Operational mode.
     */
    void integrate_gj(er7_utils::TimeInterface & time_interface, er7_utils::BaseIntegrationGroup & integ_group);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
