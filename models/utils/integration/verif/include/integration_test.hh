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
  (Define the class IntegrationTest, the base class for testing both
   translational and rotational state integration.)

Reference:
  (((Hammen, David)
    (JSC Engineering Orbital Dynamics: Integration)
    (NASA Johnson Space Center, Houston TX)
    (2010)))

Assumptions and limitations:
  ((This model is intended for IV&V purposes only.))

Library dependencies:
  ((../src/integration_test.cc))

 

*******************************************************************************/


#ifndef JEOD_INTEGRATION_TEST_HH
#define JEOD_INTEGRATION_TEST_HH

// System includes
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <typeinfo>

// Trick includes
#include "sim_services/Integrator/include/Integrator.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"
#include "er7_utils/integration/core/include/integrable_object.hh"

// JEOD includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "random_orientation.hh"


//! Namespace jeod
namespace jeod {

class IntegrationTestManager;


/*
Purpose:
  (Base class for testing state propagation.)
*/
class IntegrationTest : public er7_utils::IntegrableObject {
JEOD_MAKE_SIM_INTERFACES(IntegrationTest)

public:

   // Member functions

   /**
     Default constructor.
   */
   IntegrationTest (void);

   /**
     Copy constructor.
     Creates a partial copy, with the source providing data reasonably assigned
     from the input file but all other members initialized to pre-initialization
     values.
   */
   IntegrationTest (const IntegrationTest & source,
                    unsigned int test_incr = 0);

   /**
     Destructor.
   */
   ~IntegrationTest (void) override;

   // Final methods

   // Getter for the time scale.
   double get_time_scale (void) const;

   // Getters for state integrator properties.
   const std::type_info & integrator_id (void) const;

   /**
    * Is the test active?
    */
   bool get_active (void) const
   {
      return active;
   }

   /**
    * Tell the item to use the deprecated propagation mode.
    */
   void set_deprecated_rotation_integration (bool val = true)
   {
      deprecated_rotation_integration = val;
   }

   // Configure the test.
   void configure (bool randomize, unsigned int caseno);

   // Initialize the test.
   void initialize (
      int option,
      er7_utils::IntegratorConstructor * integ_constructor,
      double sim_dt);


   /**
     Reset the integrators.
   */
   void reset_integrators (void) override
   {
      if (state_integrator != nullptr) {
         state_integrator->reset_integrator ();
      }
   }


   // Implemented virtual methods

   // Initialize the test.
   // The pre_initialize method performs early initializations.
   // The post_initialize method performs later initializations.
   // Derived classes should override both the pre_ and post_initialize
   // methods and should forward the call to their parent class.
   // The forwarding should be performed *first* in the pre_initialize
   // method, *last* in the post_initialize method.
   virtual void pre_initialize (void);
   virtual void post_initialize (void);

   // Generate end-of-simulation report.
   // Derived classes should forward the shutdown call to their parent
   // class on entry to shutdown.
   virtual void shutdown (double sim_time, double dyn_time, FILE * report);


   // Pure virtual methods

   /**
     Make sure that the integration can be performed.
    */
   virtual void validate_integrability (
      const er7_utils::IntegratorConstructor & generator) = 0;

   /**
     Create a copy of the object.
     \return Constructed IntegrationTest object
   */
   virtual IntegrationTest * replicate (unsigned int test_incr) = 0;

   /**
     Propagate the true state to the specified dynamic time and
     evaluate the errors in the integrated state.
     \param[in] dyn_time Simulation dynamic time \\
     Units: s
   */
   virtual void propagate (double dyn_time, bool update_errors) = 0;

   /**
     Propagate the integrated state via state integration over the
     specified dynamic time interval.
     \return Integration status
     \param[in] dyn_dt Dynamic time step \\
     Units: s
     \param[in] target_state Integration stage to be achieved
     Units: s
   */
   er7_utils::IntegratorResult integrate (
      double dyn_dt,
      unsigned int target_stage) override = 0;

   /**
     Compute state derivatives.
   */
   virtual void compute_derivatives (void) = 0;


   // Member data

   double omega_dt; /* trick_units(rad) @n
      User input characteristic angle.
      Product of characteristic frequency and dynamic time.
      This value determines the dynamics characteristics of the system. */


protected:

   // Member data

   bool random_orientations; /* trick_units(--) @n
      When set, the orientation between canonical and integration frames will
      be set randomly per the test_case member datum.
      Making this non-volatile leads to some very funky behavior when
      compiled optimized. */

   bool initialized; /* trick_units(--) @n
      Several options cannot be changed once the object has been initialized. */

   bool deprecated_rotation_integration; /* trick_units(--) @n
      Should the antiquated rotational integration scheme be used?
      This data member has effect only when set prior to the creation of the
      body's integrators. The body's rotational integrator will use the JEOD 2.0
      rotation integration scheme if this member is set, but will use a scheme
      that better models rotational motion if this member is clear.

      As indicated by the member's name, this old-style integration scheme is
      deprecated. */

   bool active; /* trick_units(--) @n
      Is the test active? */

   unsigned int test_case; /* trick_units(--) @n
      User input test case number.
      Value initializes the random number generators. */

   int integ_opt; /* trick_units(--) @n
      Trick integrator structure integration option. */

   double delta_t; /* trick_units(s) @n
      Dynamic time step. */

   double omega; /* trick_units(rad/s) @n
      Characteristic frequency; omega_dt/delta_t. */

   double time_scale; /* trick_units(--) @n
      Time scale factor; dyn_time = sim_time * time_scale. */

   er7_utils::SecondOrderODEIntegrator * state_integrator;  /* trick_io(**) @n
      State integrator created by the integrator constructor. */

   RandomSeedGenerator seed_gen; /* trick_units(--) @n
      Random seed generator. */

   RandomOrientation iinteg_icanon; /* trick_units(--) @n
      Integration inertial frame to canonical inertial frame orientation. */

   Quaternion Q_iinteg_icanon; /* trick_units(--) @n
      Inertial integration to canonical left transformation quaternion. */

   double T_iinteg_icanon[3][3]; /* trick_units(--) @n
      Inertial integration to canonical transformation matrix. */

   RandomOrientation bcanon_binteg; /* trick_units(--) @n
      Canonical body frame to integration body frame orientation. */

   Quaternion Q_bcanon_binteg; /* trick_units(--) @n
      Body canonical to integration left transformation quaternion. */

   double T_bcanon_binteg[3][3]; /* trick_units(--) @n
      Body canonical to integration transformation matrix. */


private:

   /**
    * Assignment operator is private but is implemented.
    * Nobody calls (tested w/ debugger), but an implementation is needed.
    */
   IntegrationTest & operator = (const IntegrationTest & source __attribute__ ((unused)) )
   { return *this; }
};

} // End JEOD namespace


#endif
