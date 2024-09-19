/*******************************************************************************

Purpose:
  (Define methods for the class IntegrationTest.)

Reference:
  (((TBS)))

Library dependencies:
  ((integration_test.cc)
   (integration_test_messages.cc)
   (new_orientation.cc)
   (random.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item_demangle.cc)
   (utils/quaternion/src/quat_to_mat.cc))



*******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <sys/select.h>
#include <typeinfo>

// Trick includes
#include "sim_services/include/exec_proto.h"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrator_constructor.hh"

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/integration_test.hh"
#include "../include/integration_test_messages.hh"

#define PIx2 6.28318530717958647692528676655900577

//! Namespace jeod
namespace jeod
{

/*
Purpose: (Default constructor.)
*/
IntegrationTest::IntegrationTest()
    : seed_gen(),
      iinteg_icanon(seed_gen, true),
      bcanon_binteg(seed_gen, true)
{
}

/*
Purpose: (Copy constructor.)
*/
IntegrationTest::IntegrationTest(const IntegrationTest & source, unsigned int test_incr)
    : er7_utils::IntegrableObject(),
      omega_dt(source.omega_dt),
      deprecated_rotation_integration(source.deprecated_rotation_integration),
      integ_opt(source.integ_opt),
      delta_t(source.delta_t),
      omega(source.omega),
      time_scale(source.time_scale),
      seed_gen(),
      iinteg_icanon(seed_gen, true),
      bcanon_binteg(seed_gen, true)
{
    configure(source.random_orientations, source.test_case + test_incr);
}

/*
Purpose: (Destructor.)
*/
IntegrationTest::~IntegrationTest()
{
    er7_utils::Er7UtilsDeletable::delete_instance(state_integrator);
}

/*
Purpose: (Get the test's time scale.)
*/
double IntegrationTest::get_time_scale() const
{
    return time_scale;
}

/*
Purpose: (Get the state integrator typeinfo.)
*/
const std::type_info & IntegrationTest::integrator_id() const
{
    return typeid(*state_integrator);
}

/*
Purpose:
  (Configure the test.)
*/
void IntegrationTest::configure(bool randomize,      // In: -- Use a random orientation?
                                unsigned int caseno) // In: -- If so, which test case?
{
    if(initialized)
    {
        MessageHandler::fail(__FILE__, __LINE__, IntegrationTestMessages::invalid_data, "Too late!");
    }

    // Save the input values in the instance.
    random_orientations = randomize;
    test_case = caseno;

    // Prepare the RandomOrientation objects that represent the relation
    // between the canonical and integration frames for initialization.
    // In the case of a Monte-Carlo test, make the objects produce pseudorandom
    // orientations, the test_case-th element from their respective sequences.
    if(randomize)
    {
        bcanon_binteg.set_generate_random_vector(true);
        iinteg_icanon.set_generate_random_vector(true);
        bcanon_binteg.set_skip_count(test_case);
        iinteg_icanon.set_skip_count(test_case);
    }
    // In the case of a regression test, make the objects produce deterministic
    // (but not overly simplistic) orientations.
    else
    {
        bcanon_binteg.set_generate_random_vector(false);
        iinteg_icanon.set_generate_random_vector(false);
        bcanon_binteg.set_vector(0.1, 0.2, 0.3);
        iinteg_icanon.set_vector(0.4, 0.5, 0.6);
    }
}

/*
Purpose:
  (Initialize the test.)
*/
void IntegrationTest::initialize(                         // Return: -- Void
    int option,                                           // In:     -- Integration option number
    er7_utils::IntegratorConstructor * integ_constructor, // In: -- Integrator constructor
    double sim_dt)                                        // In:     -- Sim. dynamic step size
{
    if(initialized)
    {
        MessageHandler::fail(__FILE__, __LINE__, IntegrationTestMessages::invalid_data, "Already initialized!");
    }

    // Save the integration option; printed in the shutdown report.
    integ_opt = option;

    // Pre-initialize; performed in derived -> base order
    pre_initialize();

    // Set omega_dt, omega, and delta_t.
    // At least one of omega_dt and omega must be set by the time this method
    // is invoked. These settings might be from the input file or from a
    // derived class' pre_initialize method.

    // Case 1: Both are set to a positive value:
    // Compute delta_t and set the object's time scale factor accordingly.
    if((omega > 0.0) && (omega_dt > 0.0))
    {
        delta_t = omega_dt / omega;
        time_scale = delta_t / sim_dt;
    }

    // Case 2: omega is set:
    // Use sim_dt as delta_t and compute omega_dt = omega * delta_t.
    else if((omega > 0.0) && (std::fpclassify(omega_dt) == FP_ZERO))
    {
        delta_t = sim_dt;
        omega_dt = omega * delta_t;
        time_scale = 1.0;
    }

    // Case 3: omega_dt is set:
    // Use sim_dt as delta_t and compute omega = omega_dt / delta_t.
    else if((omega_dt > 0.0) && (std::fpclassify(omega) == FP_ZERO))
    {
        delta_t = sim_dt;
        omega = omega_dt / delta_t;
        time_scale = 1.0;
    }

    // Everything else is a user error.
    else
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             IntegrationTestMessages::invalid_data,
                             "Invalid omega=%g, omega_dt=%g",
                             omega,
                             omega_dt);
    }

    // Make sure we can do what is asked.
    validate_integrability(*integ_constructor);

    // Post-initialize; performed in base -> derived order
    post_initialize();

    initialized = true;
}

/*
Purpose:
  (Pre-initialize an IntegrationTest instance.)
*/
void IntegrationTest::pre_initialize()
{
    // Construct the canonical inertial to integration inertial and
    // integration body to canonical body transformations.
    bcanon_binteg.initialize();
    bcanon_binteg.get_quaternion(Q_bcanon_binteg);
    Q_bcanon_binteg.left_quat_to_transformation(T_bcanon_binteg);

    iinteg_icanon.initialize();
    iinteg_icanon.get_quaternion(Q_iinteg_icanon);
    Q_iinteg_icanon.left_quat_to_transformation(T_iinteg_icanon);
}

/*
Purpose:
  (Post-initialize an IntegrationTest instance.)
*/
void IntegrationTest::post_initialize()
{
    // Empty for now.
}

/*
Purpose:
  (Generate the end of sim run report.)
*/
void IntegrationTest::shutdown( // Return: -- Void
    double sim_time,            // In:     s  Simulation time
    double dyn_time,            // In:     s  Dynamic time
    FILE * report)              // Inout:  -- Output stream
{
    // Report defaults to stdout if not set.
    if(report == nullptr)
    {
        report = stdout;
    }

    // Print the common header.
    fprintf(report,
            "%s\n"
            "  integ option        = %d\n"
            "  omega_dt        (d) = %g\n"
            "  omega         (d/s) = %g\n"
            "  test number         = %d\n"
            "  sim end time    (s) = %g\n"
            "  dyn end time    (s) = %g\n"
            "  # cycles            = %g\n",
            NamedItem::demangle(typeid(*this)).c_str(),
            integ_opt,
            omega_dt * 180.0 / M_PI,
            omega * 180.0 / M_PI,
            test_case,
            sim_time,
            dyn_time,
            omega * dyn_time / PIx2);
}
} // namespace jeod
