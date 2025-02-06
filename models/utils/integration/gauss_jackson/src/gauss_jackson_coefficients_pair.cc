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
 * @file models/utils/integration/gauss_jackson/src/gauss_jackson_coefficients_pair.cc
 * Defines member functions for the class GaussJacksonCoefficientsPair.
 */

/*
Purpose: ()
*/

// Local includes
#include "../include/gauss_jackson_coefficients_pair.hh"
#include "../include/gauss_jackson_two_state.hh"

// ER7 utilities includes
#include "er7_utils/interface/include/alloc.hh"

// System includes
#include <algorithm>
#include <iostream>

//! Namespace jeod
namespace jeod
{

void GaussJacksonCoefficientsPair::allocate_arrays(int size)
{
    sa_coefs = er7_utils::alloc::allocate_array<double>(size);
    gj_coefs = er7_utils::alloc::allocate_array<double>(size);
}

void GaussJacksonCoefficientsPair::deallocate_arrays()
{
    er7_utils::alloc::deallocate_array(sa_coefs);
    er7_utils::alloc::deallocate_array(gj_coefs);
}

void GaussJacksonCoefficientsPair::swap(GaussJacksonCoefficientsPair & other)
{
    std::swap(sa_coefs, other.sa_coefs);
    std::swap(gj_coefs, other.gj_coefs);
}

void GaussJacksonCoefficientsPair::apply(int nelem,
                                         int ncoeff,
                                         const double * ER7_UTILS_RESTRICT const * ER7_UTILS_RESTRICT acc_hist,
                                         const GaussJacksonTwoState & state_sum) const ER7_UTILS_RESTRICT
{
    double * ER7_UTILS_RESTRICT vel_sum = state_sum.first;
    double * ER7_UTILS_RESTRICT pos_sum = state_sum.second;

    double sa_i = sa_coefs[0];
    double gj_i = gj_coefs[0];
    const double * ER7_UTILS_RESTRICT acc_i = acc_hist[0];
    for(int jj = 0; jj < nelem; ++jj)
    {
        double acc_ij = acc_i[jj];
        vel_sum[jj] = acc_ij * sa_i;
        pos_sum[jj] = acc_ij * gj_i;
    }

    for(int icoeff = 1; icoeff < ncoeff; ++icoeff)
    {
        sa_i = sa_coefs[icoeff];
        gj_i = gj_coefs[icoeff];
        acc_i = acc_hist[icoeff];
        for(int jj = 0; jj < nelem; ++jj)
        {
            double acc_ij = acc_i[jj];
            vel_sum[jj] += acc_ij * sa_i;
            pos_sum[jj] += acc_ij * gj_i;
        }
    }
}

void GaussJacksonCoefficientsPair::apply(int nelem,
                                         int ncoeff,
                                         const double * ER7_UTILS_RESTRICT const * ER7_UTILS_RESTRICT deriv_hist,
                                         const GaussJacksonOneState & state_sum) const ER7_UTILS_RESTRICT
{
    double * ER7_UTILS_RESTRICT vel_sum = state_sum.first;

    double sa_i = sa_coefs[0];
    const double * ER7_UTILS_RESTRICT deriv_i = deriv_hist[0];
    for(int jj = 0; jj < nelem; ++jj)
    {
        double deriv_ij = deriv_i[jj];
        vel_sum[jj] = deriv_ij * sa_i;
    }
    for(int icoeff = 1; icoeff < ncoeff; ++icoeff)
    {
        sa_i = sa_coefs[icoeff];
        deriv_i = deriv_hist[icoeff];
        for(int jj = 0; jj < nelem; ++jj)
        {
            double deriv_ij = deriv_i[jj];
            vel_sum[jj] += deriv_ij * sa_i;
        }
    }
}

void GaussJacksonCoefficientsPair::print(int order, std::ostream & stream) const
{
    stream << "summed_adams\n";
    for(int ii = 0; ii <= order; ++ii)
    {
        stream << sa_coefs[ii] << " ";
    }
    stream << "\n";

    stream << "gauss_jackson\n";
    for(int ii = 0; ii <= order; ++ii)
    {
        stream << gj_coefs[ii] << " ";
    }
    stream << "\n";
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
