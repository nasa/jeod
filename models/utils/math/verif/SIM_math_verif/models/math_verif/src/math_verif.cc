/*******************************************************************************
PURPOSE:
   (verification of JEOD math rountines)

REFERENCE:
   (((none)))

ASSUMPTIONS AND LIMITATIONS:
   ((none))

CLASS:
   (derivative)

LIBRARY DEPENDENCY:
   ((math_verif.cc)
    (utils/math/src/dm_invert.cc)
    (utils/math/src/dm_invert_symm.cc))


*******************************************************************************/

/* System includes. */
#include <cmath>
#include <cstdio>

#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/numerical.hh"
#include "utils/math/include/vector3.hh"

#include "math_verif.hh"

//! Namespace jeod
namespace jeod
{

static FILE * demoOutputFp = demoOutputFp;

void set_demo_output_file(const std::string & fname)
{
    if(!fname.empty())
    {
        demoOutputFp = fopen(fname.c_str(), "w");
    }
}

/* ENTRY POINT: */
int math_verif(                /* RETURN: --             */
               double A[3][3], /* IN: -- input matrix */
               double B[3][3], /* IN: -- input matrix */
               double C[3][3], /* IN: -- input matrix */
               double f[3],    /* IN: -- input vector */
               double g[3],    /* IN: -- input vector */
               double s)       /* IN: -- input scalar */
{
    double M[3][3];

    fprintf(demoOutputFp, "\n********************************************************\n");
    fprintf(demoOutputFp, "MATRIX OPERATIONS\n");
    fprintf(demoOutputFp, "\nINPUT VARIABLES\n");

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "A  %10.4f %10.4f %10.4f \n", A[0][0], A[0][1], A[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[1][0], A[1][1], A[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[2][0], A[2][1], A[2][2]);

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "B  %10.4f %10.4f %10.4f \n", B[0][0], B[0][1], B[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", B[1][0], B[1][1], B[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", B[2][0], B[2][1], B[2][2]);

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "C  %10.4f %10.4f %10.4f \n", C[0][0], C[0][1], C[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", C[1][0], C[1][1], C[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", C[2][0], C[2][1], C[2][2]);

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "f  %10.4f %10.4f %10.4f \n", f[0], f[1], f[2]);

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "g  %10.4f %10.4f %10.4f \n", g[0], g[1], g[2]);

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "s  %10.4f \n", s);

    fprintf(demoOutputFp, "\nMATRIX INITIALIZE \n");
    Matrix3x3::initialize(M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX IDENTITY \n");
    Matrix3x3::identity(M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX CROSS, f \n");
    Matrix3x3::cross_matrix(f, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX OUTER PRODUCT, f x g \n");
    Matrix3x3::outer_product(f, g, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX NEGATE (in place), -A \n");
    Matrix3x3::negate(A);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[0][0], A[0][1], A[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[1][0], A[1][1], A[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[2][0], A[2][1], A[2][2]);
    Matrix3x3::negate(A);

    fprintf(demoOutputFp, "\nMATRIX NEGATE, -B \n");
    Matrix3x3::negate(B, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX TRANSPOSE (in place), A' \n");
    Matrix3x3::transpose(A);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[0][0], A[0][1], A[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[1][0], A[1][1], A[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[2][0], A[2][1], A[2][2]);
    Matrix3x3::transpose(A);

    fprintf(demoOutputFp, "\nMATRIX TRANSPOSE, B' \n");
    Matrix3x3::transpose(B, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX SCALE (in place), sA \n");
    Matrix3x3::scale(s, A);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[0][0], A[0][1], A[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[1][0], A[1][1], A[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[2][0], A[2][1], A[2][2]);
    Matrix3x3::scale(1.0 / s, A);

    fprintf(demoOutputFp, "\nMATRIX SCALE, sB \n");
    Matrix3x3::scale(B, s, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX INCREMENT (in place), A+C \n");
    Matrix3x3::incr(C, A);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[0][0], A[0][1], A[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[1][0], A[1][1], A[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[2][0], A[2][1], A[2][2]);
    Matrix3x3::decr(C, A);

    fprintf(demoOutputFp, "\nMATRIX DECREMENT (in place), A-C \n");
    Matrix3x3::decr(C, A);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[0][0], A[0][1], A[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[1][0], A[1][1], A[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", A[2][0], A[2][1], A[2][2]);
    Matrix3x3::incr(C, A);

    fprintf(demoOutputFp, "\nMATRIX COPY, B \n");
    Matrix3x3::copy(B, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX ADD, B+C \n");
    Matrix3x3::add(B, C, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX SUBTRACT, B-C \n");
    Matrix3x3::subtract(B, C, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX PRODUCT, BC \n");
    Matrix3x3::product(B, C, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX PRODUCT (left transpose), B'C \n");
    Matrix3x3::product_left_transpose(B, C, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX PRODUCT (right transpose), BC' \n");
    Matrix3x3::product_right_transpose(B, C, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX PRODUCT (transpose-transpose), B'C' \n");
    Matrix3x3::product_transpose_transpose(B, C, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX TRANSFORM, BCB' \n");
    Matrix3x3::transform_matrix(B, C, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX TRANSPOSE TRANSFORM, B'CB \n");
    Matrix3x3::transpose_transform_matrix(B, C, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX INVERT, inv(B) \n");
    Matrix3x3::invert(B, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\nMATRIX INVERT SYMMETRIC, inv(B) \n");
    Matrix3x3::invert_symmetric(B, M);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[0][0], M[0][1], M[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[1][0], M[1][1], M[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", M[2][0], M[2][1], M[2][2]);

    fprintf(demoOutputFp, "\n********************************************************\n");
    fprintf(demoOutputFp, "VECTOR OPERATIONS\n");
    fprintf(demoOutputFp, "\nINPUT VARIABLES\n");

    double vA[3];
    double vB[3];
    double vC[3];
    double vD[3];
    double vsmall[3];
    double V[3];
    double T[3][3];
    double R;

    vA[0] = 2.0;
    vA[1] = -3.0;
    vA[2] = 8.0;

    vB[0] = 7.0;
    vB[1] = 2.0;
    vB[2] = -1.0;

    vC[0] = 5.0;
    vC[1] = -5.0;
    vC[2] = 7.0;

    vD[0] = 4.0;
    vD[1] = 3.0;
    vD[2] = -1.0;

    vsmall[0] = 1.0E-08;
    vsmall[1] = -1.0E-08;
    vsmall[2] = 1.0E-08;

    T[0][0] = 0.819152;
    T[0][1] = 0.573576;
    T[0][2] = 0.000000;
    T[1][0] = -0.573576;
    T[1][1] = 0.819152;
    T[1][2] = 0.000000;
    T[2][0] = 0.000000;
    T[2][1] = 0.000000;
    T[2][2] = 1.000000;

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "A  %10.4f\n   %10.4f\n   %10.4f \n", vA[0], vA[1], vA[2]);

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "B  %10.4f\n   %10.4f\n   %10.4f \n", vB[0], vB[1], vB[2]);

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "C  %10.4f\n   %10.4f\n   %10.4f \n", vC[0], vC[1], vC[2]);

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "D  %10.4f\n   %10.4f\n   %10.4f \n", vD[0], vD[1], vD[2]);

    fprintf(demoOutputFp, "\n");
    fprintf(demoOutputFp, "T  %10.4f %10.4f %10.4f \n", T[0][0], T[0][1], T[0][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", T[1][0], T[1][1], T[1][2]);
    fprintf(demoOutputFp, "   %10.4f %10.4f %10.4f \n", T[2][0], T[2][1], T[2][2]);

    fprintf(demoOutputFp, "\nVECTOR INITIALIZE \n");
    Vector3::initialize(V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR UNIT (i=1) \n");
    Vector3::unit(0, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR FILL, 3.0 \n");
    Vector3::fill(3.0, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR ZERO SMALL \n");
    Vector3::zero_small(1.0E-06, vsmall);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", vsmall[0], vsmall[1], vsmall[2]);

    fprintf(demoOutputFp, "\nVECTOR COPY, B \n");
    Vector3::copy(vB, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR DOT, B dot C \n");
    R = Vector3::dot(vB, vC);
    fprintf(demoOutputFp, "   %10.4f\n", R);

    fprintf(demoOutputFp, "\nVECTOR MAG SQUARED, |B|^2 \n");
    R = Vector3::vmagsq(vB);
    fprintf(demoOutputFp, "   %10.4f\n", R);

    fprintf(demoOutputFp, "\nVECTOR MAG, |B| \n");
    R = Vector3::vmag(vB);
    fprintf(demoOutputFp, "   %10.4f\n", R);

    fprintf(demoOutputFp, "\nVECTOR NORMALIZE (in place), A/|A| \n");
    Vector3::copy(vA, V);
    Vector3::normalize(V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR NORMALIZE, B/|B| \n");
    Vector3::normalize(vB, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR SCALE (in place), sA \n");
    Vector3::copy(vA, V);
    Vector3::scale(s, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR SCALE, sB \n");
    Vector3::scale(vB, s, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR NEGATE (in place), -A \n");
    Vector3::copy(vA, V);
    Vector3::negate(V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR NEGATE, -B \n");
    Vector3::negate(vB, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR TRANSFORM (in place), TA \n");
    Vector3::copy(vA, V);
    Vector3::transform(T, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR TRANSFORM, TB \n");
    Vector3::transform(T, vB, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR TRANSFORM TRANSPOSE (in place), T'A \n");
    Vector3::copy(vA, V);
    Vector3::transform_transpose(T, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR TRANSFORM TRANSPOSE, T'B \n");
    Vector3::transform_transpose(T, vB, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR INCREMENT (in place), A+C \n");
    Vector3::copy(vA, V);
    Vector3::incr(vC, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR INCREMENT (in place), A+C+D \n");
    Vector3::copy(vA, V);
    Vector3::incr(vC, vD, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR DECREMENT (in place), A-C \n");
    Vector3::copy(vA, V);
    Vector3::decr(vC, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR DECREMENT (in place), A-(C+D) \n");
    Vector3::copy(vA, V);
    Vector3::decr(vC, vD, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR SUM, B+C \n");
    Vector3::sum(vB, vC, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR SUM, B+C+D \n");
    Vector3::sum(vB, vC, vD, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR DIFF, B-C \n");
    Vector3::diff(vB, vC, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR CROSS, B cross C \n");
    Vector3::cross(vB, vC, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR SCALE INCREMENT (in place), A+sB \n");
    Vector3::copy(vA, V);
    Vector3::scale_incr(vB, s, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR SCALE DECREMENT (in place), A-sB \n");
    Vector3::copy(vA, V);
    Vector3::scale_decr(vB, s, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR CROSS INCREMENT (in place), A+(B cross C) \n");
    Vector3::copy(vA, V);
    Vector3::cross_incr(vB, vC, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR CROSS DECREMENT (in place), A-(B cross C) \n");
    Vector3::copy(vA, V);
    Vector3::cross_decr(vB, vC, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR TRANSFORM INCREMENT (in place), A+TB \n");
    Vector3::copy(vA, V);
    Vector3::transform_incr(T, vB, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR TRANSFORM DECREMENT (in place), A-TB \n");
    Vector3::copy(vA, V);
    Vector3::transform_decr(T, vB, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR TRANSFORM TRANSPOSE INCREMENT (in place), A+T'B \n");
    Vector3::copy(vA, V);
    Vector3::transform_transpose_incr(T, vB, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\nVECTOR TRANSFORM TRANSPOSE DECREMENT (in place), A-T'B \n");
    Vector3::copy(vA, V);
    Vector3::transform_transpose_decr(T, vB, V);
    fprintf(demoOutputFp, "   %10.4f\n   %10.4f\n   %10.4f \n", V[0], V[1], V[2]);

    fprintf(demoOutputFp, "\n********************************************************\n");
    fprintf(demoOutputFp, "NUMERICAL OPERATIONS\n");

    double a, b;

    a = -2.0E-312;

    fprintf(demoOutputFp, "\na       = %25.15E \n", a);

    fprintf(demoOutputFp, "\nNUMERICAL FABS \n");
    b = Numerical::fabs(a);
    fprintf(demoOutputFp, "fabs(a) = %25.15E\n", b);

    fprintf(demoOutputFp, "\nNUMERICAL SQUARE (with underflow protetction), a^2 \n");
    b = Numerical::square(a);
    fprintf(demoOutputFp, "a^2     = %25.15E\n", b);

    fprintf(demoOutputFp, "\nNUMERICAL SQUARE INCREMENT (with underflow protection), a^2 + 3.5 \n");
    b = 3.5;
    a = Numerical::square_incr(-2.0E-312, b);
    fprintf(demoOutputFp, "        = %25.15E\n", a);

    fprintf(demoOutputFp, "\n");

    return (0);
}

} // namespace jeod
