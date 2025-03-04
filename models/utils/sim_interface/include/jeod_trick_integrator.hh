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
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/include/jeod_trick_integrator.hh
 * Define the interface for accessing / updating elements of a Trick simulation
 * integrator object.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_TRICK10_INTEGRATOR_HH
#define JEOD_TRICK10_INTEGRATOR_HH

#ifdef TRICK_VER

// Trick includes
#include "sim_services/Integrator/include/Integrator.hh"

// er7_utils / Trick includes
#include "er7_utils/trick/integration/include/translate_trick_integ_type.hh"

// Local includes
#include "jeod_class.hh"
#include "jeod_integrator_interface.hh"

//! Namespace jeod
namespace jeod
{

/**
 * A TrickJeodIntegrator specializes the Trick::Integrator to provide
 * the Trick side of the integration interface between Trick and JEOD.
 */
class TrickJeodIntegrator : public Trick::Integrator
{
public:
    // Methods

    // Note:
    // The default constructor, copy constructor, and assignment operator
    // for this class are not declared. The C++ defaults are in force.

    /**
     * Destructor
     */
    ~TrickJeodIntegrator() override = default;

    // Trick::Integrator methods
    // The pure virtual methods required by Trick::Integrator are supplied
    // as required but do nothing. They should not be called.

    /**
     * Does nothing.
     */
    int integrate() override
    {
        return -1;
    }

    /**
     * Does nothing.
     */
    void initialize(int, double) override {}
};

/**
 * A JeodTrickIntegrator specializes the JeodIntegratorInterface for use
 * with Trick as the simulation engine.
 */
class JeodTrickIntegrator : public JeodIntegratorInterface
{
    JEOD_MAKE_SIM_INTERFACES(jeod, JeodTrickIntegrator)

public:
    // Methods

    /**
     * Default constructor
     */
    JeodTrickIntegrator()
        : JeodIntegratorInterface(),
          trick_integrator()

    {
    }

    /**
     * Destructor.
     */
    ~JeodTrickIntegrator() override = default;

    /**
     * Interpret the integration technique
     */
    er7_utils::Integration::Technique interpret_integration_type(int integ_technique) const override
    {
        return er7_utils::translate_trick_integrator_type(static_cast<Integrator_type>(integ_technique));
    }

    /**
     * Get the simulation engine's integrator.
     * @return Pointer to the simulation engine's integrator.
     */
    Trick::Integrator * get_integrator() override
    {
        return &trick_integrator;
    }

    /**
     * Get the integration cycle time step.
     * @return                Simulation time delta t, in seconds
     */
    double get_dt() const override
    {
        return trick_integrator.dt;
    }

    /**
     * Get the flag that tells the simulation engine to compute derivatives
     * on the initial step of each integration cycle.
     * @return Value of the first step derivatives flag
     */
    bool get_first_step_derivs_flag() const override
    {
        return trick_integrator.first_step_deriv;
    }

    /**
     * Set the flag that tells the simulation engine to compute derivatives
     * on the initial step of each integration cycle.
     * @param[in] value       Value of the first step derivatives flag
     */
    void set_first_step_derivs_flag(bool value) override
    {
        trick_integrator.first_step_deriv = value;
    }

    /**
     * Reset the flag that tells the simulation engine to compute derivatives
     * on the initial step of each integration cycle. Derivatives are always
     * needed just after a reset. The behavior should revert to nominal after
     * the reset has been performed.
     */
    void reset_first_step_derivs_flag() override
    {
        default_first_step_deriv = trick_integrator.first_step_deriv;
        trick_integrator.first_step_deriv = true;
    }

    /**
     * Restore the flag that tells the simulation engine to compute derivatives
     * on the initial step of each integration cycle to it's value prior to
     * the most recent call to reset_first_step_derivs_flag.
     */
    void restore_first_step_derivs_flag() override
    {
        trick_integrator.first_step_deriv = default_first_step_deriv;
    }

    /**
     * Set the step number within an integration cycle.
     * @param[in] stepno      Step number
     */
    void set_step_number(unsigned int stepno) override
    {
        trick_integrator.intermediate_step = stepno;
    }

    /**
     * Update the time model given the simulation time.
     * @param[in] sim_time Simulation time
     */
    void set_time(double sim_time) override
    {
        trick_integrator.time = sim_time;
    }

    // Deleted methods: copy constructor and assignment operator
    JeodTrickIntegrator(const JeodTrickIntegrator &) = delete;
    JeodTrickIntegrator & operator=(const JeodTrickIntegrator &) = delete;

private:
    // Member data

    /**
     * Trick integration structure.
     */
    TrickJeodIntegrator trick_integrator; //!< trick_units(--)

    /**
     * Default value of trick_integrator.first_step_deriv.
     */
    bool default_first_step_deriv{}; //!< trick_units(--)
};

} // namespace jeod

#endif
#endif

/**
 * @}
 * @}
 * @}
 */
