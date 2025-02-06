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
 * @file models/utils/integration/lsode/src/lsode_generalized_second_order_ode_integrator.cc
 * Define member functions for the class LsodeGeneralizedDerivSecondOrderODEIntegrator.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  (TBS)

Library dependencies:
  ((lsode_generalized_second_order_ode_integrator.o))

 

*******************************************************************************/


// System includes

// Interface includes
#include "er7_utils/interface/include/alloc.hh"

// Integration includes
#include "er7_utils/integration/core/include/integ_utils.hh"
#include "er7_utils/interface/include/message_handler.hh"
#include "er7_utils/integration/core/include/integration_messages.hh"
#include "er7_utils/integration/core/include/integration_controls.hh"

// Model includes
#include "../include/lsode_control_data_interface.hh"
#include "../include/lsode_generalized_second_order_ode_integrator.hh"

using namespace jeod;

/**
 * Default Constructor
 */
LsodeGeneralizedDerivSecondOrderODEIntegrator::LsodeGeneralizedDerivSecondOrderODEIntegrator(void)
:
   LsodeSecondOrderODEIntegrator()
{}

/**
 * non-default constructor
 */
LsodeGeneralizedDerivSecondOrderODEIntegrator::LsodeGeneralizedDerivSecondOrderODEIntegrator (
        const LsodeControlDataInterface & data_in,
        er7_utils::IntegrationControls & controls,
        const er7_utils::GeneralizedPositionDerivativeFunctions & deriv_funs,
        unsigned int position_size,
        unsigned int velocity_size)
:
    LsodeSecondOrderODEIntegrator( data_in,
                                   controls,
                                   deriv_funs,
                                   position_size,
                                   velocity_size)
{
   posdot = er7_utils::alloc::allocate_array<double> (position_size);
}

/**
 * Copy Constructor
 */
LsodeGeneralizedDerivSecondOrderODEIntegrator::LsodeGeneralizedDerivSecondOrderODEIntegrator(
   const LsodeGeneralizedDerivSecondOrderODEIntegrator & src JEOD_UNUSED):
   jeod::LsodeSecondOrderODEIntegrator()
{
   er7_utils::MessageHandler::fail (__FILE__, __LINE__,
      er7_utils::IntegrationMessages::internal_error,
      "Copy constructor for LsodeGeneralizedDerivSecondOrderODEIntegrator "
      "not implemented.\n");
}



/**
 * Clone a LsodeGeneralizedDerivSecondOrderODEIntegrator.
 */
LsodeGeneralizedDerivSecondOrderODEIntegrator *
LsodeGeneralizedDerivSecondOrderODEIntegrator::create_copy ()
const
{
   er7_utils::MessageHandler::fail (__FILE__, __LINE__,
      er7_utils::IntegrationMessages::internal_error,
      "Copy constructor for LsodeGeneralizedDerivSecondOrderODEIntegrator "
      "not implemented.\n");
   //return er7_utils::alloc::replicate_object (*this);
   return NULL;
}


/**
 * Destructor
 */
LsodeGeneralizedDerivSecondOrderODEIntegrator::~LsodeGeneralizedDerivSecondOrderODEIntegrator (void)
{
      er7_utils::alloc::deallocate_array<double> (posdot);
}


// Propagate state via LSODE.
er7_utils::IntegratorResult
LsodeGeneralizedDerivSecondOrderODEIntegrator::integrate (
   double dyn_dt,
   unsigned int,
   double const * ER7_UTILS_RESTRICT accel,
   double * ER7_UTILS_RESTRICT velocity,
   double * ER7_UTILS_RESTRICT position)
{
   er7_utils::IntegratorResult return_val;  // Don't move this line.  First entry for optimization

   compute_posdot (position, velocity, posdot);
   for (unsigned int ii = 0; ii < zeroth_derivative_size; ii++) {
      y[ii] = position[ii];
      y_dot[ii] = posdot[ii];
   }
   for (unsigned int ii = 0; ii < first_derivative_size; ii++) {
      y[ii+zeroth_derivative_size] = velocity[ii];
      y_dot[ii+zeroth_derivative_size] = accel[ii];
   }
   return_val =  first_order_integrator.integrate( dyn_dt,
                                                    1,
                                                    y,
                                                    y_dot);
   for (unsigned int ii = 0; ii < zeroth_derivative_size; ii++) {
      position[ii] = y[ii];
   }
   for (unsigned int ii = 0; ii < first_derivative_size; ii++) {
      velocity[ii] = y[ii+zeroth_derivative_size];
   }

   return return_val;
}

/**
 * @}
 * @}
 * @}
 * @}
 */
