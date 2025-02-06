/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup ExpMath
 * @{
 *
 * @file
 * Implement class GaussJordanSolver.
 */

/*
Purpose: ()
*/


#include "../include/gauss_jordan_solver.hh"


//! Namespace jeod 
namespace jeod {

unsigned GaussJordanSolver::solve(DoubleVectorT& x)
{
    unsigned last_row = -1u;

    // Mark all rows as unprocessed.
    for (unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        avail_rows[ii] = ii;
    }

    // Construct the augmented matrix [A | b]
    for (unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        for (unsigned jj = 0; jj < n_dimensions; ++jj)
        {
            augmented_matrix(ii,jj) = a_matrix(ii,jj);
        }
        augmented_matrix(ii,n_dimensions) = b_vector[ii];
    }


    double max_pivot = 1e-2;
    for (unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        // Find the row containing the largest absolute value in column ii.
        // Note that column ii is zero in logical rows 0 to ii-1.
        unsigned i_pivot = avail_rows[ii];
        double col_max = std::fabs(augmented_matrix(i_pivot,ii));
        for (unsigned jj = ii+1; jj < n_dimensions; ++jj)
        {
            unsigned j_pivot = avail_rows[jj];
            double value_j = std::fabs(augmented_matrix(j_pivot,ii));
            if (value_j > col_max)
            {
                i_pivot = j_pivot;
                col_max = value_j;
            }
        }

        // Crudely check for underdetermined system.
        if (col_max < 1e-8*max_pivot)
        {
            break;
        }
        else if (col_max > max_pivot)
        {
            max_pivot = col_max;
        }

        // i_pivot is the pivot row.
        std::swap(avail_rows[ii], avail_rows[i_pivot]);
        pivot_row[ii] = i_pivot;

        // Divide the pivot row by the pivot value.
        double row_scale = 1.0 / augmented_matrix(i_pivot,ii);
        augmented_matrix(i_pivot, ii) = 1.0;
        for (unsigned kk = ii+1; kk <= n_dimensions; ++kk)
        {
            augmented_matrix(i_pivot, kk) *= row_scale;
        }

        // Remove the pivot row from the other rows.
        for (unsigned jj = 0; jj < n_dimensions; ++jj)
        {
            unsigned j_pivot = avail_rows[jj];
            if (j_pivot == i_pivot)
            {
                continue;
            }
            row_scale = augmented_matrix(j_pivot, ii);
            augmented_matrix(j_pivot, ii) = 0.0;
            for (unsigned kk = ii+1; kk <= n_dimensions; ++kk)
            {
                augmented_matrix(j_pivot, kk) -=
                    row_scale * augmented_matrix(i_pivot, kk);
            }
        }

        last_row = ii;
    }

    for (unsigned int ii = last_row+1u; ii < n_dimensions; ++ii) {
        pivot_row[ii] = avail_rows[ii];
        augmented_matrix(avail_rows[ii],n_dimensions) = 0.0;
    }

    x.resize(n_dimensions);
    for (unsigned int ii = 0; ii < n_dimensions; ++ii) {
        x[ii] = augmented_matrix(pivot_row[ii],n_dimensions);
    }

    return last_row + 1u;
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
