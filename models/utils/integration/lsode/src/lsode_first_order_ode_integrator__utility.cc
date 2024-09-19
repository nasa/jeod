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
 * @file models/utils/integration/lsode/src/lsode_first_order_ode_integrator__utility.cc
 * Define member functions for the class LsodeFirstOrderODEIntegrator.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((dlsode.f)))

Assumptions and limitations:
  (TBS)

Library dependencies:
  ((lsode_first_order_ode_integrator__manager.cc))



*******************************************************************************/

// System includes
#include <cmath> //std
#include <cmath> // sqrt

// Interface includes
#include "er7_utils/interface/include/alloc.hh"

// Integration includes
#include "er7_utils/integration/core/include/integ_utils.hh"
#include "er7_utils/integration/core/include/integration_controls.hh"
#include "er7_utils/integration/core/include/integration_messages.hh"
#include "er7_utils/interface/include/message_handler.hh"

// Model includes
#include "../include/lsode_first_order_ode_integrator.hh"

using namespace jeod;

// Default constructor for a LsodeFirstOrderODEIntegrator.
LsodeFirstOrderODEIntegrator::LsodeFirstOrderODEIntegrator()
{
    // set epsilon
    calculate_epsilon();
}

// Non-default constructor for a LsodeFirstOrderODEIntegrator.
LsodeFirstOrderODEIntegrator::LsodeFirstOrderODEIntegrator(const LsodeControlDataInterface & data_in,
                                                           er7_utils::IntegrationControls & controls,
                                                           unsigned int size)
    : FirstOrderODEIntegrator(size, controls),
      control_data(data_in)
{
    // set epsilon
    calculate_epsilon();
    control_data.num_odes = size;
    update_control_data();
}

/**
 *
 */
LsodeFirstOrderODEIntegrator::~LsodeFirstOrderODEIntegrator()
{
    if(!first_pass)
    {
        arrays.destroy_allocated_arrays();
        control_data.destroy_allocated_arrays();
    }
}

/**
 * Gets the control data from where it can be populated in the
 * constructor and verifies that the input control parameters
 * are not out of sensible range.
 */
void LsodeFirstOrderODEIntegrator::update_control_data()
{
    //   DLSODE lines 25-70, along with other checks, have been moved to
    //   the data-interface class method, check_interface_data.
    control_data.check_interface_data();

    if(control_data.integration_method == LsodeControlDataInterface::ImplicitAdamsNonStiff)
    {
        max_order_internal = 12;
    }
    else // control_data.integration_method == ImplicitBackDiffStiff
    {
        max_order_internal = 5;
    }
    if(control_data.max_order < max_order_internal)
    {
        max_order_internal = control_data.max_order;
    }

    // 50
    if(control_data.max_step_size > 0.0)
    {
        max_step_size_inv = 1.0 / control_data.max_step_size;
    }
    else
    {
        max_step_size_inv = 0.0;
    }
}

// Clone a LsodeFirstOrderODEIntegrator. (Not really. Abort instead.)
LsodeFirstOrderODEIntegrator * LsodeFirstOrderODEIntegrator::create_copy() const
{
    er7_utils::MessageHandler::fail(__FILE__,
                                    __LINE__,
                                    er7_utils::IntegrationMessages::internal_error,
                                    "Copy constructor for LsodeFirstOrderODEIntegrator not implemented.\n");
    return nullptr;
}

/**
 * returns the RMS value of {V dot W}, where V and W are N-vectors.
 *
 * Modified version of DVNORM
 *
 * The only places DVNORM is used, it is multiplying some array by the
 * error_weight array (arrays.error_weight) across control_data.num_odes terms.
 * These values are fixed for our application, and do not need to be passed in.
 *
 * We provide two implementations, one for a one-dimensional array, and one for
 * a two-dimensional array in which the first index is the variable.
 * \param v array
 */
double LsodeFirstOrderODEIntegrator::magnitude_of_weighted_array(const double * v)
{
    double sum = 0.0;
    for(unsigned int ii = 0; ii < control_data.num_odes; ii++)
    {
        double mult = v[ii] * arrays.error_weight[ii];
        sum += (mult * mult);
    }
    return sqrt(sum / control_data.num_odes);
}

/**
 * returns RMS value of v[*][index]
 *
 * Modified version of DVNORM, second implementation.
 * \param index use this index
 * \param v array
 */
double LsodeFirstOrderODEIntegrator::magnitude_of_weighted_array(unsigned int index, double ** v)
{
    double sum = 0.0;
    for(unsigned int ii = 0; ii < control_data.num_odes; ii++)
    {
        double mult = v[ii][index] * arrays.error_weight[ii];
        sum += (mult * mult);
    }
    return sqrt(sum / control_data.num_odes);
}

/**
 * Factors a double array (arrays.lin_alg) by Gaussian elimination.
 *
 * Modified version of DGEFA.
 */
int LsodeFirstOrderODEIntegrator::gauss_elim_factor()
{
    // DGEFA was only called once.  While it had two indices passed in, they were
    // both the same (N = control_data.num_odes).  The array passed in was
    // WM(3) = arrays.lin_alg.
    // Also, IWM(21)=pivots was passed in, and IER passed out.
    // num_odes, arrays.lin_alg, and pivots are all class variables so do not
    // need to be passed.  The return value (IER) is not reproduced, but is
    // passed back by the return statement instead.

    int info = 0;
    if(control_data.num_odes >= 2) // inverse go to 70;
    {
        for(unsigned int k = 0; k < control_data.num_odes - 1; k++) // do 60
        {
            //  find l = pivot index;
            //  Original had IDAMAX + K -1.
            //  If nth tested element is max, IDAMAX returns n, while
            //  index_of_max_magnitude returns index = (n-1)+k
            unsigned int l = index_of_max_magnitude(control_data.num_odes - k, arrays.lin_alg, k);
            arrays.pivots[k] = l;
            // zero pivot implies this column already triangularized
            if(std::fpclassify(arrays.lin_alg[l][k]) != FP_ZERO) // inverse go to 40;
            {
                // interchange if necessary
                if(l != k) // inverse go to 10;
                {
                    double temp_val = arrays.lin_alg[l][k];
                    arrays.lin_alg[l][k] = arrays.lin_alg[k][k];
                    arrays.lin_alg[k][k] = temp_val;
                    // 10
                }
                // compute multipliers
                double t = -1.0 / arrays.lin_alg[k][k];
                for(unsigned int ii = 0; ii < control_data.num_odes - k - 1; ii++)
                {
                    arrays.lin_alg[k + 1 + ii][k] *= t;
                }
                // row elimination with column indexing
                for(unsigned int j = k + 1; j < control_data.num_odes; j++)
                {
                    // do 30
                    t = arrays.lin_alg[l][j];
                    if(l != k) // inverse go to 20;
                    {
                        arrays.lin_alg[l][j] = arrays.lin_alg[k][j];
                        arrays.lin_alg[k][j] = t;
                        // 20
                    }
                    for(unsigned int ii = 0; ii < control_data.num_odes - k - 1; ii++)
                    {
                        arrays.lin_alg[k + 1 + ii][j] += (t * arrays.lin_alg[k + 1 + ii][k]);
                    }
                    // 30
                }
                // go to 50 using if-else structure.
                // 40
            }
            else
            {
                info = k;
                // 50
            }
            // 60
        }
        // 70
    }

    arrays.pivots[control_data.num_odes - 1] = control_data.num_odes - 1;
    if(std::fpclassify(arrays.lin_alg[control_data.num_odes - 1][control_data.num_odes - 1]) == FP_ZERO)
    {
        info = control_data.num_odes - 1;
    }
    return info;
}

/**
 * Solves the equation Y' = A Y, with A = arrays.lin_alg.
 */
void LsodeFirstOrderODEIntegrator::linear_solver()
{
    double t;
    // job = 0 , solve  a * x = b;
    // first solve  l*y = b;
    if(control_data.num_odes > 1) // inverse go to 30;
    {
        for(unsigned int k = 0; k < control_data.num_odes - 1; k++) // do 20
        {
            unsigned int l = arrays.pivots[k];
            t = y[l];
            if(l != k) // inverse go to 10;
            {
                y[l] = y[k];
                y[k] = t;
                // 10
            }
            for(unsigned int ii = 0; ii < control_data.num_odes - k - 1; ii++)
            {
                y[k + 1 + ii] += (t * arrays.lin_alg[k + 1 + ii][k]);
            }
            // 20
        }
        // 30
    }
    // now solve  u*x = y;
    for(int k = control_data.num_odes - 1; k >= 0; k--) // do 40
    {
        y[k] /= arrays.lin_alg[k][k];
        t = -y[k];
        for(int ii = 0; ii < k; ii++)
        {
            y[ii] += (t * arrays.lin_alg[ii][k]);
        }
        // 40
    }
    // go to 100 using if-else structure
    // 50
}

// DGBFA and DGBSL to be implemented at a later time, subject to
// confirmation of their desirability.  There are enough loose ends
// already.
// See lsode_first_order_ode_integrator_non_implemented.cc for content of these functions

/**
 * Modified version of IDAMAX.  IDAMAX has 2 operations, one for
 * situations in which the index increments by 1, and another for the
 * converse.  Since all instances in LSODE use the unit-increment method,
 * that is the only one represented here.
 * Search through matrix "array", starting at
 * array[start_ix_1][start_ix_2] for the next "num_points" elements.
 * The boolean search_ix_1 controls whether to increment index#1 (true),
 * or index#2 (false).
 * Method returns the searched index that corresponds to the largest
 * magnitude.
 *
 * @note
 * The only call to this method passed "k" in for both indices, so I stripped the
 * second argument.  If DGBFA gets implemented, it will have to be added back in;
 * the call from DGBFA is for array starting at (M,K)
 */
// confirmation of their desirability.  There are enough loose ends
// already.
// See lsode_first_order_ode_integrator_non_implemented.cc for content of these functions

/*****************************************************************************
index_of_max_magnitude
Purpose:(Modified version of IDAMAX.  IDAMAX has 2 operations, one for
         situations in which the index increments by 1, and another for the
         converse.  Since all instances in LSODE use the unit-increment method,
         that is the only one represented here.
         Search through matrix "array", starting at
         array[start_ix_1][start_ix_2] for the next "num_points" elements.
         The boolean search_ix_1 controls whether to increment index#1 (true),
         or index#2 (false).
         Method returns the searched index that corresponds to the largest
         magnitude.

@note
The only call to this method passed "k" in for both indices, so I stripped the
second argument.  If DGBFA gets implemented, it will have to be added back in;
the call from DGBFA is for array starting at (M,K))
******************************************************************************/
unsigned int LsodeFirstOrderODEIntegrator::index_of_max_magnitude(unsigned int num_points,
                                                                  double ** array,
                                                                  int start_ix)
{
    unsigned int idamax = start_ix;
    double max_value = std::abs(array[start_ix][start_ix]);

    for(unsigned int ii = 1; ii < num_points; ii++)
    {
        double test_value = std::abs(array[start_ix + ii][start_ix]);
        if(test_value > max_value)
        {
            max_value = test_value;
            idamax = ii;
        }
    }
    return idamax;
}

/**
 * Load the externally generated derivative values (incoming as y_dot)i
 * into the array derivs.
 */
void LsodeFirstOrderODEIntegrator::load_derivatives(double * derivs)
{
    for(unsigned int ii = 0; ii < control_data.num_odes; ii++)
    {
        derivs[ii] = y_dot[ii];
    }
}

/**
 * @}
 * @}
 * @}
 * @}
 */
