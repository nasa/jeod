#include "../../../include/gauss_quadrature.hh"
// #include "utils/math/include/gauss_quadrature.hh"
#include <cmath>
#include <iostream>

double poly(size_t order, double var)
{
    double sum = 0.5;
    double var_ix = 1.0;
    for(size_t ix = 1; ix <= order; ++ix)
    {
        var_ix *= var;
        sum += (ix + 0.5) * var_ix;
    }
    return sum;
}

double int_poly(size_t order, double lo, double hi)
{
    double sum = 0.0;
    double lo_ix = 1.0;
    double hi_ix = 1.0;
    for(size_t ix = 1; ix <= order + 1; ++ix)
    {
        lo_ix *= lo;
        hi_ix *= hi;
        sum += (ix + 0.5 - 1) * (hi_ix - lo_ix) / ix;
    }
    return sum;
}

void gauss_quad_poly(size_t order, double lo, double hi)
{
    size_t gauss_order = (order + 2) / 2;
    std::cout << "Gauss quadrature order " << gauss_order << "\n";
    if(gauss_order > 8)
    {
        std::cout << "Order is too high for gauss-quadrature coefficients.\n";
        return;
    }
    std::cout << "Integrating order " << order << " from " << lo << " to " << hi << "\n";
    double int_result = int_poly(order, lo, hi);
    std::cout << "Integrated value is " << int_result << "\n";

    double sum = 0.0;
    for(size_t ix = 0; ix <= gauss_order; ++ix)
    {
        double true_x = lo + ((1 + jeod::GaussQuadrature::gauss_xvalues[gauss_order][ix]) / 2.0) * (hi - lo);
        sum += ((hi - lo) / 2) * poly(order, true_x) * jeod::GaussQuadrature::gauss_weights[gauss_order][ix];
    }
    std::cout << "Gauss quad value is " << sum << "\n";
}

int main()
{
    double lo_arr[4] = {-2, -1, 1, 2};
    double hi_arr[4] = {-1, 1, 5, 10};
    for(size_t ix = 0; ix < 4; ++ix)
    {
        for(size_t ix_order = 2; ix_order < 17; ++ix_order)
        {
            gauss_quad_poly(ix_order, lo_arr[ix], hi_arr[ix]);
        }
    }

    std::cout << "Integrating sin(x) from 0 to pi/2.  Integrated value = 1.\n";
    for(size_t gauss_order = 1; gauss_order <= 8; ++gauss_order)
    {
        double sum = 0.0;
        for(size_t ix = 0; ix < gauss_order; ++ix)
        {
            double true_x = (1 + jeod::GaussQuadrature::gauss_xvalues[gauss_order][ix]) * M_PI_4;
            sum += M_PI_4 * sin(true_x) * jeod::GaussQuadrature::gauss_weights[gauss_order][ix];
        }
        std::cout << "Gauss order " << gauss_order << " produces " << sum << "\n";
    }

    std::cout << "Integrating cos(x) from 0 to pi.  Integrated value = 0.\n";
    for(size_t gauss_order = 1; gauss_order <= 8; ++gauss_order)
    {
        double sum = 0.0;
        for(size_t ix = 0; ix < gauss_order; ++ix)
        {
            double true_x = (1 + jeod::GaussQuadrature::gauss_xvalues[gauss_order][ix]) * M_PI_2;
            sum += M_PI_2 * cos(true_x) * jeod::GaussQuadrature::gauss_weights[gauss_order][ix];
        }
        std::cout << "Gauss order " << gauss_order << " produces " << sum << "\n";
    }

    std::cout << "Integrating sqrt(x) from 0 to 4.  Integrated value = 5.333333.\n";
    for(size_t gauss_order = 1; gauss_order <= 8; ++gauss_order)
    {
        double sum = 0.0;
        for(size_t ix = 0; ix < gauss_order; ++ix)
        {
            double true_x = (1 + jeod::GaussQuadrature::gauss_xvalues[gauss_order][ix]) * 2;
            sum += 2 * sqrt(true_x) * jeod::GaussQuadrature::gauss_weights[gauss_order][ix];
        }
        std::cout << "Gauss order " << gauss_order << " produces " << sum << "\n";
    }

    std::cout << "Integrating 1/x from 1 to 2.  Integrated value = 0.69315.\n";
    for(size_t gauss_order = 1; gauss_order <= 8; ++gauss_order)
    {
        double sum = 0.0;
        for(size_t ix = 0; ix < gauss_order; ++ix)
        {
            double true_x = 1 + (1 + jeod::GaussQuadrature::gauss_xvalues[gauss_order][ix]) * 0.5;
            sum += 0.5 * (1 / true_x) * jeod::GaussQuadrature::gauss_weights[gauss_order][ix];
        }
        std::cout << "Gauss order " << gauss_order << " produces " << sum << "\n";
    }

    std::cout << "Integrating 1/x from -1 to 2.  Integrated value = undefined.\n";
    for(size_t gauss_order = 1; gauss_order <= 8; ++gauss_order)
    {
        double sum = 0.0;
        for(size_t ix = 0; ix < gauss_order; ++ix)
        {
            double true_x = -1.0 + (1 + jeod::GaussQuadrature::gauss_xvalues[gauss_order][ix]) * 1.5;
            sum += 1.5 * (1 / true_x) * jeod::GaussQuadrature::gauss_weights[gauss_order][ix];
        }
        std::cout << "Gauss order " << gauss_order << " produces " << sum << "\n";
    }
    return 1;
}
