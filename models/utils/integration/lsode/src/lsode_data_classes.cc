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
 * @file models/utils/integration/lsode/src/lsode_data_classes.cc
 * Define member functions for the data-grouping classes specified in lsode_data_classes.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  (TBS)

Library dependencies:
  ((lsode_data_classes.o))

 

*******************************************************************************/


// System includes

// Integration includes
#include "er7_utils/interface/include/message_handler.hh"
#include "er7_utils/integration/core/include/integration_messages.hh"

// Interface includes
#include "er7_utils/interface/include/alloc.hh"

// Integration includes

// Model includes
#include "../include/lsode_data_classes.hh"

using namespace jeod;

/**
 * constructor
 */

LsodeDataJacobianPrep::LsodeDataJacobianPrep()
:
   fac(0.0),
   hl0(0.0),
   index(0),
   index_max(0),
   r0(0.0),
   yj(0.0)
{}


/**
 * constructor
 */

LsodeDataStode::LsodeDataStode()
:
   step_ratio(0.0),
   step_ratio_order_inc(0.0),
   told(0.0),
   dsm(0.0),
   iredo(0),
   iret(0),
   ncf(0),
   new_method_order(0)
{}


/**
 * constructor
 */

LsodeDataArrays::LsodeDataArrays()
:
   pivots(NULL),
   history(NULL),
   lin_alg_1(0.0),
   lin_alg_2(0.0),
   lin_alg(NULL),
   error_weight(NULL),
   save(NULL),
   accum_correction(NULL),
   lin_alg_index1(0),
   num_odes(3),
   allocated(false)
{}


/**
 * Allocates memory for the variable size arrays
 */
void
LsodeDataArrays::allocate_arrays(
      unsigned int num_odes_in,
      LsodeControlDataInterface::CorrectorMethod corrector_method)
{
// This is a code chunk adapted from lines  1321-1325 in original fortran.

   num_odes = num_odes_in;

   // num_odes appears to be at least as large as num_equations, which
   // may be variable (in original Lsode).
   // In this version, they are identical.
   // So allocate pivots and history, which should
   // be sized by num_equations, by num_odes instead.
   pivots = er7_utils::alloc::allocate_array<int>(num_odes);


   // history data must have size of at least 1 + the integrator order.
   // The interator order is limited by :
   //    the specified value of maximum order,
   //    5 if integrator-method = 2
   //    12 if integrator-method = 1.
   //    To avoid checking on the size of this array, going straight
   //    to 1+12, the largest it could possibly be.
   history = er7_utils::alloc::allocate_array<double *>(num_odes);
   for (unsigned int ii = 0; ii < num_odes; ++ii) {
      history[ii] = er7_utils::alloc::allocate_array<double> (13);
   }

   // lin_alg was of length LWM.
   // Now it has two scalar components (lin_alg_1, lin_alg_2) and
   // a 2-d array.
   unsigned int index1, index2;
   //if (miter.eq.0) lenwm=0
   if (corrector_method == LsodeControlDataInterface::FunctionalIteration) {
      index1 = index2 = 1;
   }
   // if (miter.eq.1 || 2) lenwm=n*n+2
   else if (corrector_method == LsodeControlDataInterface::NewtonIterUserJac ||
            corrector_method == LsodeControlDataInterface::NewtonIterInternalJac ) {
      index1 = index2 = num_odes;
   }
   // if (miter.eq.3) lenwm=n+2,
   // so 1xn + the two outstanding elements.
   else if (corrector_method == LsodeControlDataInterface::JacobiNewtonInternalJac) {
      index1 = 1;
      index2 = num_odes;
   }
   else {
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::internal_error,
         "This should never happen!");

      // Unreached
      index1 = 0;
      index2 = 0;

   }
//   //if (miter.ge.4) lenwm=(2*ml+mu+1)*n+2
// MITER = 4 or 5 not supported.
//   else {
//      index1 =
//            2*jacobian_lower_half_bandwidth + jacobian_upper_half_bandwidth +1;
//      if (index1 < 2) {
//         index1 = 1;
//      }
//      index2 = num_odes;
//   }

   // lin_alg[index1][index2]
   // lewt = lwm + lenwm means lin_alg takes up lenwm spaces:
   // 2 of those are reserved for lin_alg_1 and lin_alg_2,
   // so the lin_alg array takes up lenwm-2 spaces.
   lin_alg_index1 = index1;

   lin_alg = er7_utils::alloc::allocate_array <double*> (index1);
   for (unsigned int ii = 0; ii < index1; ++ii) {
      lin_alg[ii] = er7_utils::alloc::allocate_array <double> (index2);
   }

   //lsavf=lewt+n means ewt takes up n spaces.
   error_weight = er7_utils::alloc::allocate_array(num_odes);//new double[num_odes];
   //lacor=lsavf+n means savf takes up n spaces.
   save = er7_utils::alloc::allocate_array(num_odes);//new double[num_odes];
   //lenrw=lacor+n-1 means accum_correction takes up n-1 spaces.
   accum_correction = er7_utils::alloc::allocate_array(num_odes);//nnew double[num_odes];

   allocated = true;

   return;
}

/**
 * Allows for refactoring and reallocation of newly sized arrays.
 */
void
LsodeDataArrays::destroy_allocated_arrays()
{
   if (allocated) {
      er7_utils::alloc::deallocate_array<int>(pivots);
      er7_utils::alloc::deallocate_array<double>(error_weight);
      er7_utils::alloc::deallocate_array<double>(save);
      er7_utils::alloc::deallocate_array<double>(accum_correction);


      for (unsigned int ii = 0; ii < num_odes; ++ii) {
         er7_utils::alloc::deallocate_array <double> (history[ii]);
      }
      er7_utils::alloc::deallocate_array <double*> (history);
      for (unsigned int ii = 0; ii < lin_alg_index1; ++ii) {
         er7_utils::alloc::deallocate_array <double> (lin_alg[ii]);
      }
      er7_utils::alloc::deallocate_array <double*> (lin_alg);
   }
   allocated = false;

   return;
}

/**
 * @}
 * @}
 * @}
 * @}
 */
