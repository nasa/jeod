#include <cstdio>

#include "test_harness/include/test_sim_interface.hh"
#include "utils/math/include/matrix3x3.hh"

using namespace jeod;

inline void set_matrix(double mat[3][3],
                       double m00,
                       double m01,
                       double m02,
                       double m10,
                       double m11,
                       double m12,
                       double m20,
                       double m21,
                       double m22)
{
    mat[0][0] = m00;
    mat[0][1] = m01;
    mat[0][2] = m02;
    mat[1][0] = m10;
    mat[1][1] = m11;
    mat[1][2] = m12;
    mat[2][0] = m20;
    mat[2][1] = m21;
    mat[2][2] = m22;
}

inline void print_matrix(const char * ident, const double mat[3][3])
{
    std::printf("%s\n"
                "  %22g %22g %22g\n"
                "  %22g %22g %22g\n"
                "  %22g %22g %22g\n\n",
                ident,
                mat[0][0],
                mat[0][1],
                mat[0][2],
                mat[1][0],
                mat[1][1],
                mat[1][2],
                mat[2][0],
                mat[2][1],
                mat[2][2]);
}

inline void test_matrix(const char * ident, const double mat[3][3], double inv[3][3])
{
    double prod[3][3];
    int status;

    printf("%s test\n", ident);

    print_matrix("mat", mat);
    status = Matrix3x3::invert(mat, inv);
    if(status == 0)
    {
        print_matrix("inv", inv);
        Matrix3x3::product(mat, inv, prod);
        print_matrix("mat*inv", prod);
    }
    else
    {
        std::printf("Matrix is singular\n");
    }
    std::printf("\n");
}

inline void test_symmetric_matrix(const char * ident, const double mat[3][3], double inv[3][3])
{
    double prod[3][3];
    int status;

    printf("%s symmetric test\n", ident);

    print_matrix("mat", mat);
    status = Matrix3x3::invert_symmetric(mat, inv);
    if(status == 0)
    {
        print_matrix("inv", inv);
        Matrix3x3::product(mat, inv, prod);
        print_matrix("mat*inv", prod);
    }
    else
    {
        std::printf("Matrix is singular\n");
    }
    std::printf("\n");
}

int main()
{
    double mat[3][3];
    double inv[3][3];
    TestSimInterface sim_interface;

    set_matrix(mat, 1, 2, 0, 2, 4, 0, 0, 0, 1);
    test_matrix("Singular matrix", mat, inv);
    test_symmetric_matrix("Singular matrix", mat, inv);

    mat[1][1] = 4 + 1e-15;
    test_matrix("Ill-conditioned matrix", mat, inv);
    test_symmetric_matrix("Ill-conditioned matrix", mat, inv);

    set_matrix(mat, 1e-12, 0.9999e-6, 0.9999, 0.9999e-6, 1, 0.9999e6, 0.9999, 0.9999e6, 1e12);
    test_matrix("Very ill-conditioned matrix", mat, inv);
    test_symmetric_matrix("Very ill-conditioned matrix", mat, inv);

    return 0;
}
