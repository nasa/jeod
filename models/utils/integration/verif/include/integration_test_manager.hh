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
/*******************************************************************************

Purpose:
  (Define the class IntegrationTestManager, the class that drives testing
   of multiple IntegrationTest objects.)

Reference:
  (((Hammen, David)
    (JSC Engineering Orbital Dynamics: Integration)
    (NASA Johnson Space Center, Houston TX)
    (2010)))

Assumptions and limitations:
  ((This model is intended for IV&V purposes only.))

Library dependencies:
  ((../src/integration_test_manager.cc))



*******************************************************************************/

#ifndef JEOD_INTEGRATION_TEST_MANAGER_HH
#define JEOD_INTEGRATION_TEST_MANAGER_HH

// System includes
#include <cstdio>
#include <string>

// Trick includes
#include "sim_services/Integrator/include/Integrator.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrator_constructor.hh"

// JEOD includes
#include "utils/integration/include/jeod_integration_group.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

// Forward declarations
class IntegrationTest;
class TimeManager;

/*
Purpose:
  (Perform multiple tests.)
*/
class IntegrationTestManager : public JeodIntegrationGroupOwner
{
    JEOD_MAKE_SIM_INTERFACES(jeod, IntegrationTestManager)

public:
    // Member functions

    // Default constructor.
    IntegrationTestManager();

    // Destructor
    ~IntegrationTestManager() override;

    // Initialize tests.
    void initialize(TimeManager & time_manager, double sim_dt);

    // Reset integrators that store past history.
    void reset_integrators(const TimeManager & time_manager);

    // Compute second derivatives.
    void compute_derivatives();

    // Integrate time and state.
    int integrate(double sim_begtime, TimeManager & time_manager);

    // Propagate true states and compute errors.
    void propagate(double dyn_time);

    // Generate reports.
    void shutdown(double sim_time, double dyn_time);

    // Setters for the template_items and integrator constructor

    /**
     * Set the nth template item.
     * @param index Template item number
     * @param test  Reference to the test object.
     */
    void set_template_item(unsigned int index, IntegrationTest & test)
    {
        template_items[index] = &test;
    }

    /**
     * Set the integrator constructor.
     * @param integ_cotr Reference to the integrator constructor to be used.
     */
    void set_integ_constructor(er7_utils::IntegratorConstructor & integ_cotr)
    {
        integ_constructor = integ_cotr.create_copy();
    }

    /**
     * Tell the test to use the deprecated propagation mode.
     */
    void set_deprecated_rotation_integration(bool val = true)
    {
        deprecated_rotation_integration = val;
    }

    /**
     * Update an integration group; does nothing.
     */
    void update_integration_group(JeodIntegrationGroup &) override {}

    // Member data

    bool generate_report{true}; /* trick_units(--) @n
       Generate a report at the end.
       This exists only to help find the nasty memory problem. */

    bool debug{}; /* trick_units(--) @n
       Print integration status. */

    bool regression_test{}; /* trick_units(--) @n
       When set, orientations are not random and num_tests is forced to be 1. */

    unsigned int base_case{}; /* trick_units(--) @n
       User input base case number. */

    unsigned int num_tests{1}; /* trick_units(--) @n
       Number of test cases per template item. */

    unsigned int total_tests{1}; /* trick_units(--) @n
       Total number of tests. */

    unsigned int cycle_count{}; /* trick_units(--) @n
       Number of integration cycles. */

    unsigned int nskip{}; /* trick_units(--) @n
       Number of integration cycles to skip before taking statistics. */

    int report_fid{1}; /* trick_units(--) @n
       Report file ID. */

    double end_time{-1}; /* trick_units(s) @n
       Expected simulation end time. */

    JEOD_SIM_INTEGRATOR_POINTER_TYPE sim_integrator{}; /* trick_units(--) @n
       Pointer to the integration object used by the simulation engine itself. */

    er7_utils::Integration::Technique integ_option{er7_utils::Integration::Unspecified}; /* trick_units(--) @n
       The integration technique. */
    int integ_option_int{};                                                              /* trick_units(--) @n
                                                                    Used for JEOD-based integration options.*/

protected:
    // Member data

    IntegrationTest ** template_items{}; /* trick_units(--) @n
       Template items. */

    IntegrationTest ** test_items{}; /* trick_units(--) @n
       Test items. */

    double time_scale{1}; /* trick_units(--) @n
       Time scale factor; dyn_time = sim_time * time_scale. */

    bool deprecated_rotation_integration{}; /* trick_units(--) @n
       Should the antiquated rotational integration scheme be used?
       This data member has effect only when set prior to the creation of the
       body's integrators. The body's rotational integrator will use the JEOD 2.0
       rotation integration scheme if this member is set, but will use a scheme
       that better models rotational motion if this member is clear.

       As indicated by the member's name, this old-style integration scheme is
       deprecated. */

    bool initialized{}; /* trick_units(--) @n
       Set when initialization is complete. */

    er7_utils::IntegratorConstructor * integ_constructor{}; /* trick_units(--) @n
        Integrator generator; typically created based on the integration
        option in the Trick integrator structure. */

    JeodIntegratorInterface * integ_interface{}; /* trick_units(--) @n
       Interface with the simulation integration structure. */

    JeodIntegrationGroup * integ_group{}; /* trick_units(--) @n
       The integration group that integrates state and time. */

    std::string * state_integrator_id{}; /* trick_units(--) @n
       State integrator identifier. */

public:
private:
    // Static member data

    static const unsigned int template_items_size; /* trick_io(*o) trick_units(--) @n
       Size of the template_items. */
};

} // namespace jeod

#ifdef TRICK_VER
#include "environment/time/include/time_manager.hh"
#include "er7_utils/integration/core/include/integrator_constructor.hh"
#include "er7_utils/integration/core/include/integrator_constructor_factory.hh"
#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"
#include "integration_test.hh"
#include "utils/integration/gauss_jackson/include/gauss_jackson_integrator_constructor.hh"
#include "utils/integration/lsode/include/lsode_integrator_constructor.hh"
#include "utils/sim_interface/include/jeod_trick_integrator.hh"
#endif

#endif
