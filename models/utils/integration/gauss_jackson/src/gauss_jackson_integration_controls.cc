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
 * @file models/utils/integration/gauss_jackson/src/gauss_jackson_integration_controls.cc
 * Defines member functions for the class GaussJacksonIntegrationControls.
 */

/*
Purpose: ()
*/

/* JEOD includes */
#include "utils/math/include/numerical.hh"

// Local includes
#include "../include/gauss_jackson_integration_controls.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/base_integration_group.hh"
#include "er7_utils/integration/core/include/integrator_constructor.hh"
#include "er7_utils/integration/core/include/integrator_result.hh"
#include "er7_utils/integration/core/include/time_interface.hh"
#include "er7_utils/interface/include/alloc.hh"

// System includes
#include <cassert>

//! Namespace jeod
namespace jeod
{

// Default constructor.
GaussJacksonIntegrationControls::GaussJacksonIntegrationControls()
{
    reset_needed = false;
}

// Non-default constructor.
GaussJacksonIntegrationControls::GaussJacksonIntegrationControls(
    const er7_utils::IntegratorConstructor & priming_constructor, const GaussJacksonConfig & config_in)
    : er7_utils::IntegrationControls(config_in.final_order),
      config(config_in),
      max_correction_iterations(config_in.max_correction_iterations),
      initial_order(config_in.initial_order),
      order(config_in.initial_order)
{
    priming_controls = priming_constructor.create_integration_controls();

    state_machine.configure(config);
    coeff.configure(config.final_order);
    coeff.compute_coeffs(initial_order);
}

// Copy constructor.
GaussJacksonIntegrationControls::GaussJacksonIntegrationControls(const GaussJacksonIntegrationControls & src)
    : er7_utils::IntegrationControls(src.config.final_order),
      cycle_starttime(src.cycle_starttime),
      cycle_simdt(src.cycle_simdt),
      cycle_dyndt(src.cycle_dyndt),
      reset_time(src.reset_time),
      coeff(src.coeff),
      config(src.config),
      state_machine(src.state_machine),
      fsm_state(src.fsm_state),
      max_correction_iterations(src.max_correction_iterations),
      initial_order(src.initial_order),
      order(src.order),
      edit_count(src.edit_count),
      at_end_of_tour(src.at_end_of_tour)
{
    if(src.priming_controls != nullptr)
    {
        priming_controls = src.priming_controls->create_copy();
    }
}

// Destructor.
GaussJacksonIntegrationControls::~GaussJacksonIntegrationControls()
{
    er7_utils::alloc::delete_object(priming_controls);
}

er7_utils::IntegrationControls * GaussJacksonIntegrationControls::create_copy() const
{
    return er7_utils::alloc::replicate_object(*this);
}

// Non-throwing swap.
void GaussJacksonIntegrationControls::swap(GaussJacksonIntegrationControls & other)
{
    IntegrationControls::swap(other);

    std::swap(priming_controls, other.priming_controls);
    std::swap(cycle_starttime, other.cycle_starttime);
    std::swap(cycle_simdt, other.cycle_simdt);
    std::swap(cycle_dyndt, other.cycle_dyndt);
    std::swap(reset_time, other.reset_time);
    std::swap(coeff, other.coeff);
    std::swap(config, other.config);
    std::swap(state_machine, other.state_machine);
    std::swap(fsm_state, other.fsm_state);
    std::swap(max_correction_iterations, other.max_correction_iterations);
    std::swap(initial_order, other.initial_order);
    std::swap(order, other.order);
    std::swap(edit_count, other.edit_count);
    std::swap(at_end_of_tour, other.at_end_of_tour);
}

void GaussJacksonIntegrationControls::reset_integrator()
{
    fsm_state = GaussJacksonStateMachine::Reset;
    edit_count = 0;
    cycle_stage = 0;
    order = initial_order;
    at_end_of_tour = false;
    state_machine.reset();
}

unsigned int GaussJacksonIntegrationControls::integrate(double start_time,
                                                        double sim_dt,
                                                        er7_utils::TimeInterface & time_interface,
                                                        er7_utils::IntegratorInterface & integ_interface,
                                                        er7_utils::BaseIntegrationGroup & integ_group)
{
    // Starting a new integration tour needs special processing.
    if(step_number == 0)
    {
        // Reset the integrators, time if the meaning of time has changed.
        if(reset_needed || (!Numerical::compare_exact(integ_simdt, sim_dt)))
        {
            // Reset integrators.
            integ_group.reset_body_integrators();
            reset_integrator();

            // Update timing information.
            reset_time = start_time;
            time_scale_factor = time_interface.get_time_scale_factor();
            integ_simdt = sim_dt;
            integ_dyndt = sim_dt * time_scale_factor;

            cycle_simdt = sim_dt * state_machine.get_cycle_scale();
            cycle_dyndt = cycle_simdt * time_scale_factor;

            // Mark the reset as handled.
            reset_needed = false;
            integ_interface.restore_first_step_derivs_flag();
        }

        // Cache the integration tour start time.
        integ_starttime = start_time;
    }

    // Short-cut for operational mode.
    if(fsm_state == GaussJacksonStateMachine::Operational)
    {
        // The state machine is no longer needed.
        // All that remains is to set the cycle start time.
        if(cycle_stage == 0)
        {
            cycle_starttime = start_time;
        }
        // Integrate using the Gauss-Jackson predictor/corrector.
        integrate_gj(time_interface, integ_group);
    }

    else
    {
        // Advance the state machine at start of cycle.
        if(cycle_stage == 0)
        {
            start_cycle(sim_dt);
        }

        // Integrate per the state machine state.
        switch(fsm_state)
        {
            // Priming: Let the priming controls take control.
            case GaussJacksonStateMachine::Priming:
                priming_controls->integrate(cycle_starttime, cycle_simdt, time_interface, integ_interface, integ_group);
                cycle_stage = priming_controls->get_step_number();
                break;

            // Edit mode: Edit one point in the collected history.
            case GaussJacksonStateMachine::BootstrapEdit:
                integrate_edit(time_interface, integ_group);
                break;

            // Step and operational modes: Integrate using the predictor/corrector.
            case GaussJacksonStateMachine::BootstrapStep:
            case GaussJacksonStateMachine::Operational:
                integrate_gj(time_interface, integ_group);
                break;
            case GaussJacksonStateMachine::Reset:
            default:
                assert("Internal error" == nullptr);
                break;
        }
    }

    // Set the step number, the value to be returned to the caller.
    // Clear if the integration completed a cycle and we're at the end of an
    // integration tour, otherwise increment.
    if((cycle_stage == 0) && at_end_of_tour)
    {
        step_number = 0;
    }
    else
    {
        ++step_number;
    }

    return step_number;
}

void GaussJacksonIntegrationControls::integrate_edit(er7_utils::TimeInterface & time_interface,
                                                     er7_utils::BaseIntegrationGroup & integ_group)
{
    // Integrate the bodies. Note: Edit mode is always at target stage = 1.
    const er7_utils::IntegratorResult & integ_status = integ_group.integrate_bodies(cycle_dyndt, 1);

    // The integrator should have edited the element corresponding to the
    // state machine's history length.
    unsigned int edit_target = state_machine.get_history_length();

    assert((integ_status.get_merge_count() == 0) ||
           (Numerical::compare_exact(integ_status.get_time_scale(), edit_target)));

    // Update timing info.
    integ_simtime = cycle_starttime;
    time_interface.update_time(integ_simtime);

    // Check whether another sequence of corrections is needed.
    if((edit_count < max_correction_iterations) && !integ_status.get_passed())
    {
        state_machine.set_bootstrap_edit_redo_needed();
    }

    // Bump the edit count if this the end of the edit sequence.
    if(edit_target == order)
    {
        ++edit_count;
    }

    // Edit mode is always at cycle stage 0 (target stage 1).
    cycle_stage = 0;
}

void GaussJacksonIntegrationControls::integrate_gj(er7_utils::TimeInterface & time_interface,
                                                   er7_utils::BaseIntegrationGroup & integ_group)
{
    unsigned int target_stage = cycle_stage + 1;
    const er7_utils::IntegratorResult & integ_status = integ_group.integrate_bodies(cycle_dyndt, target_stage);

    assert(Numerical::compare_exact(integ_status.get_time_scale(), 1.0));

    if(target_stage == 1)
    {
        assert(integ_status.get_passed());
        integ_simtime = cycle_starttime + cycle_simdt;
        time_interface.update_time(integ_simtime);
        if(max_correction_iterations == 0)
        {
            cycle_stage = 0;
        }
        else
        {
            cycle_stage = 1;
            edit_count = 0;
        }
    }
    else
    {
        ++edit_count;
        if(integ_status.get_passed() || (edit_count == max_correction_iterations))
        {
            cycle_stage = 0;
        }
    }
}

void GaussJacksonIntegrationControls::start_cycle(double sim_dt)
{
    // Apply the state machine and get the new state.
    state_machine.perform_step();
    fsm_state = state_machine.get_fsm_state();

    // Compute the start time of this integration cycle.
    cycle_starttime = reset_time + sim_dt * state_machine.get_cycle_start_time();
    at_end_of_tour = state_machine.get_at_end_of_tour();

    if(state_machine.get_at_downsample())
    {
        cycle_simdt = sim_dt * state_machine.get_cycle_scale();
        cycle_dyndt = cycle_simdt * time_scale_factor;
        edit_count = 0;
    }

    if(state_machine.get_at_order_change())
    {
        order = state_machine.get_current_order();
        coeff.compute_coeffs(order);
        edit_count = 0;
    }

    if(state_machine.get_at_reinitialize())
    {
        cycle_stage = 0;
        if(fsm_state == GaussJacksonStateMachine::BootstrapEdit)
        {
            ++edit_count;
        }
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
