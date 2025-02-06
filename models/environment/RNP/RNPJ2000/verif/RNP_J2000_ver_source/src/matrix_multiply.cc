#include "utils/math/include/vector3.hh"

void matrix_multiply(      // Return: -- void
    double matrix[3][3],   // In: -- matrix M
    double to_multiply[3], // In: -- vector V
    double result[3])      // Out: -- Result of M*V
{
    jeod::Vector3::transform(matrix, to_multiply, result);
}
