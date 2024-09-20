/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/spherical_harmonics_gravity_controls.cc
 * Define member functions for the SphericalHarmonicsGravityControls class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((spherical_harmonics_gravity_controls.cc)
   (spherical_harmonics_calc_nonspherical.cc)
   (gravity_controls.cc)
   (spherical_harmonics_delta_coeffs.cc)
   (spherical_harmonics_gravity_source.cc)
   (gravity_manager.cc)
   (gravity_messages.cc)
   (utils/message/src/message_handler.cc))



*******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/gravity_manager.hh"
#include "../include/gravity_messages.hh"
#include "../include/spherical_harmonics_delta_coeffs.hh"
#include "../include/spherical_harmonics_delta_controls.hh"
#include "../include/spherical_harmonics_gravity_controls.hh"
#include "../include/spherical_harmonics_gravity_source.hh"

//! Namespace jeod
namespace jeod
{

/**
 * SphericalHarmonicsGravityControls constructor.
 */
SphericalHarmonicsGravityControls::SphericalHarmonicsGravityControls()
{
    JEOD_REGISTER_CLASS(SphericalHarmonicsGravityControls);
    JEOD_REGISTER_CLASS(SphericalHarmonicsDeltaControls);
    JEOD_REGISTER_CHECKPOINTABLE(this, var_effects);
}

/**
 * SphericalHarmonicsGravityControls destructor.
 */
SphericalHarmonicsGravityControls::~SphericalHarmonicsGravityControls()
{
    // Note: The planet_name is set externally.
    // This is a simple POD class; deleting the memory is the responsibility
    // of the external agent that set the planet_name.

    var_effects.clear();
    JEOD_DEREGISTER_CHECKPOINTABLE(this, var_effects);

    // Array Pnm was allocated to degree/order specified in harmonics body,
    // which is the size of the entire spherical harmonics model, not to the
    // to-be-used degree/order specified in this control.
    JEOD_DELETE_2D(Pnm, harmonics_source->degree + 1, true);
    JEOD_DELETE_2D(delta_Cnm, delta_degree + 1, true);
    JEOD_DELETE_2D(delta_Snm, delta_degree + 1, true);
}

/**
 * Initialize this GravityControl.
 * \param[in] grav_manager Ref to Gravity Manager
 */
void SphericalHarmonicsGravityControls::initialize_control(GravityManager & grav_manager)
{
    // Perform basic initializations from base class
    GravityControls::initialize_control(grav_manager);

    // Cast the base class GravitySource pointer to derived type
    harmonics_source = dynamic_cast<SphericalHarmonicsGravitySource *>(body);

    // Check the validity of the provided spherical harmonics controls settings
    check_validity();

    // If degree > 0 in the gravity body then create and fill Gottlieb
    // LeGendre polynomial array.  Otherwise only spherical gravity available.
    unsigned int alloc_degree = harmonics_source->degree;

    if(alloc_degree > 0)
    {
        Pnm = JEOD_ALLOC_PRIM_ARRAY(alloc_degree + 1, double *);
        double * int_to_double = JEOD_ALLOC_PRIM_ARRAY(alloc_degree + 2, double);

        for(unsigned int ii = 0; ii <= alloc_degree; ++ii)
        {
            int_to_double[ii] = static_cast<double>(ii);
            Pnm[ii] = JEOD_ALLOC_PRIM_ARRAY(ii + 3, double);
        }

        int_to_double[alloc_degree + 1] = static_cast<double>(alloc_degree + 1);

        // In the code below, the equation numbers and page numbers refer to
        // the Gottlieb 1993 paper.

        // Bottom of page 47 and page 48, and see equation (7-8)
        Pnm[0][0] = 1.0;
        Pnm[0][1] = 0.0;
        Pnm[0][2] = 0.0;
        Pnm[1][1] = sqrt(3.0);
        Pnm[1][2] = 0.0;
        Pnm[1][3] = 0.0;

        // Pages 46-47
        for(unsigned int ii = 2; ii <= alloc_degree; ++ii)
        {
            // P(n,n) term, equation (7-8)
            Pnm[ii][ii] = sqrt((2.0 * int_to_double[ii] + 1.0) / (2.0 * int_to_double[ii])) * Pnm[ii - 1][ii - 1];

            // P(n,n+1) and P(n,n+2) terms, table 1 (p. 14)
            Pnm[ii][ii + 1] = 0.0;
            Pnm[ii][ii + 2] = 0.0;
        }

        // Clean up local variable int_to_double (needed to init Pnm above)
        JEOD_DELETE_ARRAY(int_to_double);
    }
}

/**
 * Add a new GravityDeltaControls to the var_effects list.
 * \param[in] delta_control Control to be added
 */
void SphericalHarmonicsGravityControls::add_deltacontrol(SphericalHarmonicsDeltaControls * delta_control)
{
    SphericalHarmonicsDeltaCoeffs * temp_deltacoeff;
    temp_deltacoeff = delta_control->grav_effect;

    // Add the gravity control to end of list
    var_effects.push_back(delta_control);

    /* If delta_Cnm & delta_Snm need larger dimensions than currently exist,
       delete and re-allocate them */
    if((temp_deltacoeff->degree > delta_degree) || (temp_deltacoeff->order > delta_order))
    {
        unsigned int new_degree = temp_deltacoeff->degree;
        unsigned int new_order = temp_deltacoeff->order;

        JEOD_DELETE_2D(delta_Cnm, delta_degree, true);
        JEOD_DELETE_2D(delta_Snm, delta_degree, true);

        delta_Cnm = JEOD_ALLOC_PRIM_ARRAY(new_degree, double *);
        delta_Snm = JEOD_ALLOC_PRIM_ARRAY(new_degree, double *);

        for(unsigned int ii = 0; ii < new_degree; ++ii)
        {
            delta_Cnm[ii] = JEOD_ALLOC_PRIM_ARRAY(new_order, double);
            delta_Snm[ii] = JEOD_ALLOC_PRIM_ARRAY(new_order, double);
        }
    }

    // Update top-level max degree & order and array size if needed
    if(temp_deltacoeff->degree > delta_degree)
    {
        delta_degree = temp_deltacoeff->degree;
    }
    if(temp_deltacoeff->order > delta_order)
    {
        delta_order = temp_deltacoeff->order;
    }
}

/**
 * Output the current functional degree.
 * @return Current degree
 */
unsigned int SphericalHarmonicsGravityControls::get_degree()
{
    return degree;
}

/**
 * Output the current functional order.
 * @return Current order
 */
unsigned int SphericalHarmonicsGravityControls::get_order()
{
    return order;
}

/**
 * Output the current functional degree and order.
 * \param[out] current_degree Current degree
 * \param[out] current_order Current order
 */
void SphericalHarmonicsGravityControls::get_degree_order(unsigned int & current_degree, unsigned int & current_order)
{
    current_degree = degree;
    current_order = order;
}

/**
 * Output the current functional gradient degree.
 * @return Current gradient degree
 */
unsigned int SphericalHarmonicsGravityControls::get_grad_degree()
{
    return gradient_degree;
}

/**
 * Output the current functional gradient order.
 * @return Current gradient order
 */
unsigned int SphericalHarmonicsGravityControls::get_grad_order()
{
    return gradient_order;
}

/**
 * Output the current functional gradient degree and order.
 * \param[out] curr_grad_degree Current gradient degree
 * \param[out] curr_grad_order Current gradient order
 */
void SphericalHarmonicsGravityControls::get_grad_degree_order(unsigned int & curr_grad_degree,
                                                              unsigned int & curr_grad_order)
{
    curr_grad_degree = gradient_degree;
    curr_grad_order = gradient_order;
}

/**
 * Update the functional degree.
 * \param[in] new_degree New desired degree
 */
void SphericalHarmonicsGravityControls::set_degree(unsigned int new_degree)
{
    degree = new_degree;

    // Check the validity of the new settings
    check_validity();
}

/**
 * Update the functional order.
 * \param[in] new_order New desired order
 */
void SphericalHarmonicsGravityControls::set_order(unsigned int new_order)
{
    order = new_order;

    // Check the validity of the new settings
    check_validity();
}

/**
 * Update the functional degree and order.
 * \param[in] new_degree New desired degree
 * \param[in] new_order New desired order
 */
void SphericalHarmonicsGravityControls::set_degree_order(unsigned int new_degree, unsigned int new_order)
{
    degree = new_degree;
    order = new_order;

    // Check the validity of the new settings
    check_validity();
}

/**
 * Update the functional gradient degree.
 * \param[in] new_grad_degree New desired degree
 */
void SphericalHarmonicsGravityControls::set_grad_degree(unsigned int new_grad_degree)
{
    gradient_degree = new_grad_degree;

    // Check the validity of the new settings
    check_validity();
}

/**
 * Update the functional gradient order.
 * \param[in] new_grad_order New desired order
 */
void SphericalHarmonicsGravityControls::set_grad_order(unsigned int new_grad_order)
{
    gradient_order = new_grad_order;

    // Check the validity of the new settings
    check_validity();
}

/**
 * Update the functional gradient degree and order.
 * \param[in] new_grad_degree New desired degree
 * \param[in] new_grad_order New desired order
 */
void SphericalHarmonicsGravityControls::set_grad_degree_order(unsigned int new_grad_degree, unsigned int new_grad_order)
{
    gradient_degree = new_grad_degree;
    gradient_order = new_grad_order;

    // Check the validity of the new settings
    check_validity();
}

/**
 * Check the validity of the gravity controls.
 */
void SphericalHarmonicsGravityControls::check_validity()
{
    if(harmonics_source == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             GravityMessages::invalid_object,
                             "Gravity field '%s' has no associated source.",
                             source_name.c_str());
        return;
    }

    if(!spherical)
    {
        // Check if maximum degree to be used for computations is greater than
        // degree of gravity field
        if(degree > harmonics_source->degree)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 GravityMessages::invalid_limit,
                                 "Gravity field degree requested(%i) for %s "
                                 "is greater than max gravity field degree(%i).",
                                 degree,
                                 harmonics_source->name.c_str(),
                                 harmonics_source->degree);
            return;
        }

        // check if maximum order to be used for computations is greater than
        // maximum order
        if(order > harmonics_source->order)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 GravityMessages::invalid_limit,
                                 "Gravity field order requested(%i) for %s "
                                 "is greater than max gravity field order(%i).",
                                 order,
                                 harmonics_source->name.c_str(),
                                 harmonics_source->order);
            return;
        }

        // check if maximum order to be used for computations is greater than
        // maximum degree
        if(order > degree)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 GravityMessages::invalid_limit,
                                 "Gravity field order(%i) for %s is "
                                 "greater than gravity field degree(%i).",
                                 order,
                                 harmonics_source->name.c_str(),
                                 degree);
            return;
        }

        // GRADIENT ERRORS
        // These are all recoverable.
        if(gradient)
        {
            // check if gradient degree exceeds accel degree
            if(gradient_degree > degree)
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     GravityMessages::invalid_limit,
                                     "Gravity gradient degree (%i) for %s is "
                                     "greater than gravity degree (%i).\n"
                                     "Setting the gradient degree to the gravity degree.",
                                     gradient_degree,
                                     harmonics_source->name.c_str(),
                                     degree);
                gradient_degree = degree;
            }

            // check if gradient degree equals 1
            if(gradient_degree == 1)
            {
                MessageHandler::error(__FILE__,
                                      __LINE__,
                                      GravityMessages::invalid_limit,
                                      "Gravity gradient degree for %s must not equal 1.\n"
                                      "Resetting to zero.\n",
                                      harmonics_source->name.c_str());
                gradient_degree = 0;
            }

            // check if gradient order to be used for computations is
            // greater than the gradient degree
            if(gradient_order > gradient_degree)
            {
                MessageHandler::error(__FILE__,
                                      __LINE__,
                                      GravityMessages::invalid_limit,
                                      "Gravity gradient order (%i) for %s is "
                                      "greater than gravity gradient degree (%i).\n"
                                      "Setting the gradient order to the gradient degree.",
                                      gradient_order,
                                      harmonics_source->name.c_str(),
                                      gradient_degree);
                gradient_order = gradient_degree;
            }

            // check if gradient order exceeds accel order
            if(gradient_order > order)
            {
                MessageHandler::error(__FILE__,
                                      __LINE__,
                                      GravityMessages::invalid_limit,
                                      "Gravity gradient order (%i) for %s is "
                                      "greater than gravity order (%i).\n"
                                      "Setting the gradient order to the gravity order.",
                                      gradient_order,
                                      harmonics_source->name.c_str(),
                                      order);
                gradient_order = order;
            }
        }
    }
}

/**
 * Command all of the gravitational variation effects to update
 * themselves.
 */
void SphericalHarmonicsGravityControls::update_deltacoeffs()
{
    // Iterate over the list of delta-controls, telling each to update itself
    unsigned int n_deltacoeffs = harmonics_source->delta_coeffs.size();

    for(unsigned int ii = 0; ii < n_deltacoeffs; ++ii)
    {
        if(var_effects[ii]->active)
        {
            harmonics_source->delta_coeffs[ii]->update(*this);
        }
    }
}

/**
 * Loop over all of the active gravitational variation effects models
 * and aggregate their changes to the gravity coefficients into the
 * top-level delta-coeffs "bin" for this gravity body.
 */
void SphericalHarmonicsGravityControls::sum_deltacoeffs()
{
    unsigned int n_deltacoeffs = harmonics_source->delta_coeffs.size();
    SphericalHarmonicsDeltaControls * temp_delta_control;
    SphericalHarmonicsDeltaCoeffs * temp_deltacoeff;

    // Zero out Cnm, Snm elements and top-level dC20 term
    total_dC20 = 0.0;
    for(unsigned int ii = 0; ii < delta_degree; ++ii)
    {
        for(unsigned int jj = 0; jj < delta_order; ++jj)
        {
            delta_Cnm[ii][jj] = 0;
            delta_Snm[ii][jj] = 0;
        }
    }

    // Loop over all active delta-coeffs and sum the coefficient-changing values
    for(unsigned int ii = 0; ii < n_deltacoeffs; ++ii)
    {
        temp_delta_control = var_effects[ii];
        if(temp_delta_control->active)
        {
            temp_deltacoeff = temp_delta_control->grav_effect;

            if(temp_delta_control->first_order_only)
            {
                total_dC20 += temp_deltacoeff->dC20;
            }
            else
            {
                for(unsigned int jj = 0; jj < temp_delta_control->degree; ++jj)
                {
                    for(unsigned int kk = 0; kk < temp_delta_control->order; ++kk)
                    {
                        delta_Cnm[jj][kk] += temp_deltacoeff->delta_Cnm[jj][kk];
                        delta_Snm[jj][kk] += temp_deltacoeff->delta_Snm[jj][kk];
                    }
                }
            }
        }
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
