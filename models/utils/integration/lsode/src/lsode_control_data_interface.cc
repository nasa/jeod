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
 * @file models/utils/integration/lsode/src/lsode_control_data_interface.cc
 * Define member functions for the class LsodeControlDataInterface.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  (TBS)

Library dependencies:
  ((lsode_control_data_interface.cc))


*******************************************************************************/

// System includes

// Interface includes

// Integration includes
#include "er7_utils/integration/core/include/integration_messages.hh"
#include "er7_utils/interface/include/alloc.hh"
#include "er7_utils/interface/include/message_handler.hh"

// Model includes
#include "../include/lsode_control_data_interface.hh"

using namespace jeod;

/**
 * constructor
 */

LsodeControlDataInterface::LsodeControlDataInterface()
{
    rel_tolerance_error_control_vec.push_back(-1.0);
    abs_tolerance_error_control_vec.push_back(-1.0);
}

/**
 * verifies that the input data has legal values.
 */
void LsodeControlDataInterface::check_interface_data()
{
    // Components of this method are taken from DLSODE lines 25-70
    // (Block B), and parts are generated specifically for this
    // implementation.

    // Make sure there is something to solve
    if(num_odes == 0)
    {
        er7_utils::MessageHandler::fail(__FILE__,
                                        __LINE__,
                                        er7_utils::IntegrationMessages::invalid_request,
                                        "Number of equations to solve invalid (num_odes=%u)",
                                        num_odes);
    }

    // Check for control flags that are out of range.
    if(error_control_indicator < 1 || error_control_indicator > 4)
    {
        er7_utils::MessageHandler::fail(__FILE__,
                                        __LINE__,
                                        er7_utils::IntegrationMessages::invalid_request,
                                        "error_control_indicator=%u which is illegal",
                                        error_control_indicator);
    }

    if(integration_method < 1 || integration_method > 2)
    {
        er7_utils::MessageHandler::fail(__FILE__,
                                        __LINE__,
                                        er7_utils::IntegrationMessages::invalid_request,
                                        "Illegal value for integration_method (%u).\n"
                                        "integration_method must be 1 or 2\n",
                                        integration_method);
    }
    if(corrector_method < 0 || corrector_method > 5)
    {
        er7_utils::MessageHandler::fail(__FILE__,
                                        __LINE__,
                                        er7_utils::IntegrationMessages::invalid_request,
                                        "Illegal value for corrector_method (%u).\n"
                                        "corrector_method must be between 1 and 4 (inclusive).\n",
                                        corrector_method);
    }
    // if (jacobian_lower_half_bandwidth < 0 ||
    //     jacobian_lower_half_bandwidth >= num_odes) {
    //    er7_utils::MessageHandler::fail (__FILE__, __LINE__,
    //       er7_utils::IntegrationMessages::invalid_request,
    //       "jacobian_lower_half_bandwidth (%u) illegal value.\n"
    //       "Must be >=0 and < %u (the number of equations to solve)",
    //       jacobian_lower_half_bandwidth,num_equations_to_solve);
    //    calculation_phase = -3;
    //    return;
    // }
    // if (jacobian_upper_half_bandwidth < 0 ||
    //     jacobian_upper_half_bandwidth >= num_odes) {
    //    er7_utils::MessageHandler::fail (__FILE__, __LINE__,
    //       er7_utils::IntegrationMessages::invalid_request,
    //       "jacobian_upper_half_bandwidth (%u) illegal value.\n"
    //       "Must be >=0 and < %u (the number of equations to solve)",
    //       jacobian_upper_half_bandwidth,num_equations_to_solve);
    //    calculation_phase = -3;
    //    return;
    // }

// DGH: Commented out. These are unsigned ints. They cannot be negative.
#if 0
   if (max_order < 0) {
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::invalid_request,
         "max_order (%u) set to less than 0",
         max_order);
   }

   if (max_num_steps < 0) {
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::invalid_request,
         "The maximum number of steps (%u) allowed is invalid.  "
         "It must be > 0.\n",
         max_num_steps);
   }

   if (max_num_small_step_warnings < 0) {
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::invalid_request,
         "The maximum number of small step warnings allowed is invalid."
         "It must be >=0.\n",max_num_small_step_warnings
         );
   }
#endif

    if(max_step_size < 0.0)
    {
        er7_utils::MessageHandler::fail(__FILE__,
                                        __LINE__,
                                        er7_utils::IntegrationMessages::internal_error,
                                        "maximum step size (%f) is invalid (less than zero).\n",
                                        max_step_size);
    }

    if(min_step_size < 0.0)
    {
        er7_utils::MessageHandler::fail(__FILE__,
                                        __LINE__,
                                        er7_utils::IntegrationMessages::internal_error,
                                        "minimum step size (%f) is invalid (less than zero).\n",
                                        min_step_size);
    }

    // ## Check rel_tolerance_error_control for legality. ---------------------
    //  Always check the first element, and all elements if they will be
    //  used.
    unsigned int upper_limit = 0;
    if(error_control_indicator == CommonAbsSpecificRel || error_control_indicator == SpecificAbsSpecificRel)
    {
        upper_limit = num_odes - 1;
    }
    if(!error_control_vector_copied_over && rel_tolerance_error_control_vec.size() == 0)
    {
        er7_utils::MessageHandler::fail(__FILE__,
                                        __LINE__,
                                        er7_utils::IntegrationMessages::internal_error,
                                        "The relative error tolerance value has not been specified.\n"
                                        "Must set at least one value in "
                                        "constructor.data_interface.rel_tolerance_error_control");
    }

    for(unsigned int ii = 0; ii <= upper_limit; ii++)
    {
        if(rel_tolerance_error_control_vec[ii] < 0.0)
        {
            er7_utils::MessageHandler::fail(__FILE__,
                                            __LINE__,
                                            er7_utils::IntegrationMessages::internal_error,
                                            "The relative error tolerance value is invalid for index %u (%f).\n"
                                            "It must be no less than 0.0.\n"
                                            "Default setting is -1.0, this must be changed.",
                                            ii,
                                            rel_tolerance_error_control_vec[ii]);
        }
    }

    // ## Check abs_tolerance_error_control for legality. -----------
    //  Likewise for previous.
    if(error_control_indicator == SpecificAbsCommonRel)
    {
        upper_limit = num_odes - 1;
    }
    else if(error_control_indicator == CommonAbsSpecificRel)
    {
        upper_limit = 0;
    }
    if(!error_control_vector_copied_over && abs_tolerance_error_control_vec.size() == 0)
    {
        er7_utils::MessageHandler::fail(__FILE__,
                                        __LINE__,
                                        er7_utils::IntegrationMessages::internal_error,
                                        "The absolute error tolerance value has not been specified.\n"
                                        "Must set at least one value in "
                                        "constructor.data_interface.abs_tolerance_error_control");
    }
    for(unsigned int ii = 0; ii <= upper_limit; ii++)
    {
        if(abs_tolerance_error_control_vec[ii] < 0.0)
        {
            er7_utils::MessageHandler::fail(__FILE__,
                                            __LINE__,
                                            er7_utils::IntegrationMessages::internal_error,
                                            "The absolute error tolerance value is invalid for index %u (%f).\n"
                                            "It must be no less than 0.0.\n"
                                            "Default setting is -1.0, this must be changed.",
                                            ii,
                                            abs_tolerance_error_control_vec[ii]);
        }
    }
    // Perform additional checks against values that are not supported
    // in this implementation:
    if(corrector_method == 1)
    {
        er7_utils::MessageHandler::fail(__FILE__,
                                        __LINE__,
                                        er7_utils::IntegrationMessages::invalid_request,
                                        "Corrector_method =1 (i.e. newtonian iteration with user-supplied "
                                        "jacobian)\nnot supported.  There is no means to specify a "
                                        "Jacobian-generation function.\n");
    }

    if(corrector_method > 3) // 4 or 5.  Uses banded Jacobian.
    {
        //          Check bandwidth.
        er7_utils::MessageHandler::fail(__FILE__,
                                        __LINE__,
                                        er7_utils::IntegrationMessages::invalid_request,
                                        "Corrector_methods 4 and 5 (using banded Jacobians) not currently"
                                        "supported.\n");
    }
}

/**
 * allocates space for vector-populated data to allow for restart
 */
void LsodeControlDataInterface::allocate_arrays()
{
    unsigned int abs_vec_size = abs_tolerance_error_control_vec.size();
    unsigned int rel_vec_size = rel_tolerance_error_control_vec.size();

    if(num_odes < rel_vec_size)
    {
        rel_vec_size = num_odes;
        er7_utils::MessageHandler::error(__FILE__,
                                         __LINE__,
                                         er7_utils::IntegrationMessages::invalid_request,
                                         "The sim has been configured with %d equations, but the relative \n"
                                         "error-control vector has been populated with %d entries.\n"
                                         "Only the first %d of those values will be copied into the usable \n"
                                         "error-control array.\n",
                                         num_odes,
                                         rel_vec_size,
                                         num_odes);
    }
    if(num_odes < abs_vec_size)
    {
        abs_vec_size = num_odes;
        er7_utils::MessageHandler::error(__FILE__,
                                         __LINE__,
                                         er7_utils::IntegrationMessages::invalid_request,
                                         "The sim has been configured with %d equations, but the absolute \n"
                                         "error-control vector has been populated with %d entries.\n"
                                         "Only the first %d of those values will be copied into the usable \n"
                                         "error-control array.\n",
                                         num_odes,
                                         abs_vec_size,
                                         num_odes);
    }
    abs_tolerance_error_control = er7_utils::alloc::allocate_array<double>(num_odes);
    rel_tolerance_error_control = er7_utils::alloc::allocate_array<double>(num_odes);

    for(unsigned int ii = 0; ii < abs_vec_size; ii++)
    {
        abs_tolerance_error_control[ii] = abs_tolerance_error_control_vec[ii];
    }
    for(unsigned int ii = 0; ii < rel_vec_size; ii++)
    {
        rel_tolerance_error_control[ii] = rel_tolerance_error_control_vec[ii];
    }
    error_control_vector_copied_over = true;
    num_odes_at_alloc = num_odes; // Counts for the data arrays also since
                                  //   they are done at the same time.
}

/**
 * De-allocates allocated array.
 */
void LsodeControlDataInterface::destroy_allocated_arrays()
{
    if(error_control_vector_copied_over)
    {
        er7_utils::alloc::deallocate_array<double>(abs_tolerance_error_control);
        er7_utils::alloc::deallocate_array<double>(rel_tolerance_error_control);
    }
    error_control_vector_copied_over = false;
}

/**
 * set values from external
 */
void LsodeControlDataInterface::set_rel_tol(int index, double value)
{
    if(index < 0)
    {
        er7_utils::MessageHandler::error(__FILE__,
                                         __LINE__,
                                         er7_utils::IntegrationMessages::invalid_request,
                                         "Error in configuration of Lsode integrator:\n"
                                         "Attempt to set rel_tolerance_error_control element with negative index.\n"
                                         "Index = %d.   Attempt failed.  \n",
                                         index);
        return;
    }

    auto uint_index = static_cast<unsigned int>(index);

    if(error_control_vector_copied_over)
    {
        if(uint_index < num_odes_at_alloc)
        {
            rel_tolerance_error_control[uint_index] = value;
        }
        else
        {
            er7_utils::MessageHandler::error(__FILE__,
                                             __LINE__,
                                             er7_utils::IntegrationMessages::invalid_request,
                                             "Error in configuration of Lsode integrator:\n"
                                             "Attempt to set rel_tolerance_error_control with index that exceeds\n"
                                             "the number of ODEs at allocation. Index = %u.  num_odes = %u.\n"
                                             "Attempt failed.\n",
                                             index,
                                             num_odes_at_alloc);
        }
    }
    else
    {
        unsigned int size = rel_tolerance_error_control_vec.size();
        if(uint_index < size)
        {
            rel_tolerance_error_control_vec[uint_index] = value;
        }
        else if(uint_index == size)
        {
            rel_tolerance_error_control_vec.push_back(value);
        }
        else
        {
            for(unsigned int ii = size; ii < uint_index; ii++)
            {
                rel_tolerance_error_control_vec.push_back(-1.0);
            }
            rel_tolerance_error_control_vec.push_back(value);
        }
    }
}

void LsodeControlDataInterface::set_abs_tol(int index, double value)
{
    if(index < 0)
    {
        er7_utils::MessageHandler::error(__FILE__,
                                         __LINE__,
                                         er7_utils::IntegrationMessages::invalid_request,
                                         "Error in configuration of Lsode integrator:\n"
                                         "Attempt to set abs_tolerance_error_control element with negative index.\n"
                                         "Index = %d.   Attempt failed.  \n",
                                         index);
        return;
    }

    auto uint_index = static_cast<unsigned int>(index);

    if(error_control_vector_copied_over)
    {
        if(uint_index < num_odes_at_alloc)
        {
            abs_tolerance_error_control[uint_index] = value;
        }
        else
        {
            er7_utils::MessageHandler::error(__FILE__,
                                             __LINE__,
                                             er7_utils::IntegrationMessages::invalid_request,
                                             "Error in configuration of Lsode integrator:\n"
                                             "Attempt to set abs_tolerance_error_control with index that exceeds\n"
                                             "the number of ODEs at allocation. Index = %u.  num_odes = %u.\n"
                                             "Attempt failed.\n",
                                             index,
                                             num_odes_at_alloc);
        }
    }
    else
    {
        unsigned int size = abs_tolerance_error_control_vec.size();
        if(uint_index < size)
        {
            abs_tolerance_error_control_vec[uint_index] = value;
        }
        else if(uint_index == size)
        {
            abs_tolerance_error_control_vec.push_back(value);
        }
        else
        {
            for(unsigned int ii = size; ii < uint_index; ii++)
            {
                abs_tolerance_error_control_vec.push_back(-1.0);
            }
            abs_tolerance_error_control_vec.push_back(value);
        }
    }
}

/**
 * @}
 * @}
 * @}
 * @}
 */
