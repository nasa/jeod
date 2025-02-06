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
   ((math_verif.o)
    (utils/math/dm_invert.o)
    (utils/math/dm_invert_symm.o))

 
*******************************************************************************/

/* System includes. */
#include <math.h>
#include <stdio.h>

#include "utils/math/include/vector3.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/numerical.hh"

//! Namespace jeod
namespace jeod {

    /* ENTRY POINT: */
int math_verif(     /* RETURN: --             */
   double A[3][3],     /* IN: -- input matrix */
   double B[3][3],     /* IN: -- input matrix */
   double C[3][3],     /* IN: -- input matrix */
   double f[3],        /* IN: -- input vector */
   double g[3],        /* IN: -- input vector */
   double s )          /* IN: -- input scalar */
{

   double M[3][3];

   fprintf(stderr, "\n********************************************************\n");
   fprintf(stderr, "MATRIX OPERATIONS\n");
   fprintf(stderr, "\nINPUT VARIABLES\n");

   fprintf(stderr, "\n");
   fprintf(stderr, "A  %10.4f %10.4f %10.4f \n",A[0][0],A[0][1],A[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[1][0],A[1][1],A[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[2][0],A[2][1],A[2][2]);

   fprintf(stderr, "\n");
   fprintf(stderr, "B  %10.4f %10.4f %10.4f \n",B[0][0],B[0][1],B[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",B[1][0],B[1][1],B[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",B[2][0],B[2][1],B[2][2]);

   fprintf(stderr, "\n");
   fprintf(stderr, "C  %10.4f %10.4f %10.4f \n",C[0][0],C[0][1],C[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",C[1][0],C[1][1],C[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",C[2][0],C[2][1],C[2][2]);

   fprintf(stderr, "\n");
   fprintf(stderr, "f  %10.4f %10.4f %10.4f \n",f[0],f[1],f[2]);

   fprintf(stderr, "\n");
   fprintf(stderr, "g  %10.4f %10.4f %10.4f \n",g[0],g[1],g[2]);

   fprintf(stderr, "\n");
   fprintf(stderr, "s  %10.4f \n",s);

   fprintf(stderr, "\nMATRIX INITIALIZE \n");
   Matrix3x3::initialize (M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX IDENTITY \n");
   Matrix3x3::identity (M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX CROSS, f \n");
   Matrix3x3::cross_matrix (f,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX OUTER PRODUCT, f x g \n");
   Matrix3x3::outer_product (f,g,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX NEGATE (in place), -A \n");
   Matrix3x3::negate (A);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[0][0],A[0][1],A[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[1][0],A[1][1],A[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[2][0],A[2][1],A[2][2]);
   Matrix3x3::negate (A);

   fprintf(stderr, "\nMATRIX NEGATE, -B \n");
   Matrix3x3::negate (B,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX TRANSPOSE (in place), A' \n");
   Matrix3x3::transpose (A);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[0][0],A[0][1],A[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[1][0],A[1][1],A[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[2][0],A[2][1],A[2][2]);
   Matrix3x3::transpose (A);

   fprintf(stderr, "\nMATRIX TRANSPOSE, B' \n");
   Matrix3x3::transpose (B,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX SCALE (in place), sA \n");
   Matrix3x3::scale (s,A);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[0][0],A[0][1],A[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[1][0],A[1][1],A[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[2][0],A[2][1],A[2][2]);
   Matrix3x3::scale (1.0/s,A);

   fprintf(stderr, "\nMATRIX SCALE, sB \n");
   Matrix3x3::scale (B,s,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX INCREMENT (in place), A+C \n");
   Matrix3x3::incr (C,A);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[0][0],A[0][1],A[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[1][0],A[1][1],A[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[2][0],A[2][1],A[2][2]);
   Matrix3x3::decr (C,A);

   fprintf(stderr, "\nMATRIX DECREMENT (in place), A-C \n");
   Matrix3x3::decr (C,A);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[0][0],A[0][1],A[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[1][0],A[1][1],A[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",A[2][0],A[2][1],A[2][2]);
   Matrix3x3::incr (C,A);

   fprintf(stderr, "\nMATRIX COPY, B \n");
   Matrix3x3::copy (B,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX ADD, B+C \n");
   Matrix3x3::add (B,C,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX SUBTRACT, B-C \n");
   Matrix3x3::subtract (B,C,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX PRODUCT, BC \n");
   Matrix3x3::product (B,C,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX PRODUCT (left transpose), B'C \n");
   Matrix3x3::product_left_transpose (B,C,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX PRODUCT (right transpose), BC' \n");
   Matrix3x3::product_right_transpose (B,C,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX PRODUCT (transpose-transpose), B'C' \n");
   Matrix3x3::product_transpose_transpose (B,C,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX TRANSFORM, BCB' \n");
   Matrix3x3::transform_matrix (B,C,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX TRANSPOSE TRANSFORM, B'CB \n");
   Matrix3x3::transpose_transform_matrix (B,C,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX INVERT, inv(B) \n");
   Matrix3x3::invert (B,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\nMATRIX INVERT SYMMETRIC, inv(B) \n");
   Matrix3x3::invert_symmetric (B,M);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[0][0],M[0][1],M[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[1][0],M[1][1],M[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",M[2][0],M[2][1],M[2][2]);

   fprintf(stderr, "\n********************************************************\n");
   fprintf(stderr, "VECTOR OPERATIONS\n");
   fprintf(stderr, "\nINPUT VARIABLES\n");

   double vA[3];
   double vB[3];
   double vC[3];
   double vD[3];
   double vsmall[3];
   double V[3];
   double T[3][3];
   double R;

   vA[0] =  2.0;
   vA[1] = -3.0;
   vA[2] =  8.0;

   vB[0] =  7.0;
   vB[1] =  2.0;
   vB[2] = -1.0;

   vC[0] =  5.0;
   vC[1] = -5.0;
   vC[2] =  7.0;

   vD[0] =  4.0;
   vD[1] =  3.0;
   vD[2] = -1.0;

   vsmall[0] =  1.0E-08;
   vsmall[1] = -1.0E-08;
   vsmall[2] =  1.0E-08;

   T[0][0] =  0.819152;
   T[0][1] =  0.573576;
   T[0][2] =  0.000000;
   T[1][0] = -0.573576;
   T[1][1] =  0.819152;
   T[1][2] =  0.000000;
   T[2][0] =  0.000000;
   T[2][1] =  0.000000;
   T[2][2] =  1.000000;


   fprintf(stderr, "\n");
   fprintf(stderr, "A  %10.4f\n   %10.4f\n   %10.4f \n",vA[0],vA[1],vA[2]);

   fprintf(stderr, "\n");
   fprintf(stderr, "B  %10.4f\n   %10.4f\n   %10.4f \n",vB[0],vB[1],vB[2]);

   fprintf(stderr, "\n");
   fprintf(stderr, "C  %10.4f\n   %10.4f\n   %10.4f \n",vC[0],vC[1],vC[2]);

   fprintf(stderr, "\n");
   fprintf(stderr, "D  %10.4f\n   %10.4f\n   %10.4f \n",vD[0],vD[1],vD[2]);

   fprintf(stderr, "\n");
   fprintf(stderr, "T  %10.4f %10.4f %10.4f \n",T[0][0],T[0][1],T[0][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",T[1][0],T[1][1],T[1][2]);
   fprintf(stderr, "   %10.4f %10.4f %10.4f \n",T[2][0],T[2][1],T[2][2]);

   fprintf(stderr, "\nVECTOR INITIALIZE \n");
   Vector3::initialize (V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR UNIT (i=1) \n");
   Vector3::unit (0,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR FILL, 3.0 \n");
   Vector3::fill (3.0,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR ZERO SMALL \n");
   Vector3::zero_small (1.0E-06,vsmall);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",vsmall[0],vsmall[1],vsmall[2]);

   fprintf(stderr, "\nVECTOR COPY, B \n");
   Vector3::copy (vB,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR DOT, B dot C \n");
   R = Vector3::dot (vB,vC);
   fprintf(stderr, "   %10.4f\n",R);

   fprintf(stderr, "\nVECTOR MAG SQUARED, |B|^2 \n");
   R = Vector3::vmagsq (vB);
   fprintf(stderr, "   %10.4f\n",R);

   fprintf(stderr, "\nVECTOR MAG, |B| \n");
   R = Vector3::vmag (vB);
   fprintf(stderr, "   %10.4f\n",R);

   fprintf(stderr, "\nVECTOR NORMALIZE (in place), A/|A| \n");
   Vector3::copy (vA,V);
   Vector3::normalize (V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR NORMALIZE, B/|B| \n");
   Vector3::normalize (vB,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR SCALE (in place), sA \n");
   Vector3::copy (vA,V);
   Vector3::scale (s,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR SCALE, sB \n");
   Vector3::scale (vB,s,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR NEGATE (in place), -A \n");
   Vector3::copy (vA,V);
   Vector3::negate (V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR NEGATE, -B \n");
   Vector3::negate (vB,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR TRANSFORM (in place), TA \n");
   Vector3::copy (vA,V);
   Vector3::transform (T,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR TRANSFORM, TB \n");
   Vector3::transform (T,vB,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR TRANSFORM TRANSPOSE (in place), T'A \n");
   Vector3::copy (vA,V);
   Vector3::transform_transpose (T,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR TRANSFORM TRANSPOSE, T'B \n");
   Vector3::transform_transpose (T,vB,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR INCREMENT (in place), A+C \n");
   Vector3::copy (vA,V);
   Vector3::incr (vC,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR INCREMENT (in place), A+C+D \n");
   Vector3::copy (vA,V);
   Vector3::incr (vC,vD,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR DECREMENT (in place), A-C \n");
   Vector3::copy (vA,V);
   Vector3::decr (vC,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR DECREMENT (in place), A-(C+D) \n");
   Vector3::copy (vA,V);
   Vector3::decr (vC,vD,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR SUM, B+C \n");
   Vector3::sum (vB,vC,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR SUM, B+C+D \n");
   Vector3::sum (vB,vC,vD,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR DIFF, B-C \n");
   Vector3::diff (vB,vC,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR CROSS, B cross C \n");
   Vector3::cross (vB,vC,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR SCALE INCREMENT (in place), A+sB \n");
   Vector3::copy (vA,V);
   Vector3::scale_incr (vB,s,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR SCALE DECREMENT (in place), A-sB \n");
   Vector3::copy (vA,V);
   Vector3::scale_decr (vB,s,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR CROSS INCREMENT (in place), A+(B cross C) \n");
   Vector3::copy (vA,V);
   Vector3::cross_incr (vB,vC,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR CROSS DECREMENT (in place), A-(B cross C) \n");
   Vector3::copy (vA,V);
   Vector3::cross_decr (vB,vC,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR TRANSFORM INCREMENT (in place), A+TB \n");
   Vector3::copy (vA,V);
   Vector3::transform_incr (T,vB,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR TRANSFORM DECREMENT (in place), A-TB \n");
   Vector3::copy (vA,V);
   Vector3::transform_decr (T,vB,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR TRANSFORM TRANSPOSE INCREMENT (in place), A+T'B \n");
   Vector3::copy (vA,V);
   Vector3::transform_transpose_incr (T,vB,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\nVECTOR TRANSFORM TRANSPOSE DECREMENT (in place), A-T'B \n");
   Vector3::copy (vA,V);
   Vector3::transform_transpose_decr (T,vB,V);
   fprintf(stderr, "   %10.4f\n   %10.4f\n   %10.4f \n",V[0],V[1],V[2]);

   fprintf(stderr, "\n********************************************************\n");
   fprintf(stderr, "NUMERICAL OPERATIONS\n");

   double a,b;

   a = -2.0E-312;

   fprintf(stderr, "\na       = %25.15E \n",a);

   fprintf(stderr, "\nNUMERICAL FABS \n");
   b = Numerical::fabs (a);
   fprintf(stderr, "fabs(a) = %25.15E\n",b);

   fprintf(stderr, "\nNUMERICAL SQUARE (with underflow protetction), a^2 \n");
   b = Numerical::square (a);
   fprintf(stderr, "a^2     = %25.15E\n",b);

   fprintf(stderr, "\nNUMERICAL SQUARE INCREMENT (with underflow protection), a^2 + 3.5 \n");
   b = 3.5;
   a = Numerical::square_incr (-2.0E-312,b);
   fprintf(stderr, "        = %25.15E\n",a);

   fprintf(stderr, "\n");

   return(0);
}
} // End JEOD namespace
