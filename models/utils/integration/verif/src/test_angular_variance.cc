/*******************************************************************************

Purpose:
  (Define the class AngularVariance, which is used to compute the error between
   the true and integrated rotational state and to model the behavior of the
   rotational simple harmonic oscillator.)

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((angular_variance.cc))



*******************************************************************************/

// System includes
#include <cmath>
#include <cstdio>

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/quaternion/include/quat.hh"

// Model includes
#include "../include/angular_variance.hh"

//! Namespace jeod
namespace jeod
{

int main()
{
    double a = 1;
    double b = 0.025;
    double w = M_PI / 64.0;
    double phi_0 = -M_PI / 4.0;
    double theta, omega, theta_prev, omega_prev;
    double uhat[3], angvel[3];
    Quaternion qu;
    AngularVariance angvar;

    Vector3::initialize(uhat);
    uhat[0] = 2.0 / 3.0;
    uhat[1] = -1.0 / 3.0;
    uhat[2] = 2.0 / 3.0;

    theta_prev = omega_prev = 0.0;

    for(int ii = 0; ii < 10000; ii++)
    {
        auto t = static_cast<double>(ii);
        double phi = w * t + phi_0;
        double cosphi = cos(phi);
        double sinphi = sin(phi);
        int su = 0;
        int sw = 0;

        theta = (b * t - a) * sinphi;
        omega = b * sinphi + w * (b * t - a) * cosphi;

        if(theta * theta_prev < 0.0)
        {
            su = 1;
        }

        if(omega * omega_prev < 0.0)
        {
            sw = 1;
        }

        qu.left_quat_from_eigen_rotation(theta, uhat);
        Vector3::scale(uhat, omega, angvel);

        if(ii == 0)
        {
            angvar.initialize(qu, angvel);
        }
        else
        {
            angvar.update(qu, angvel);
        }

        printf("%5d  %d %d %3d  %7.4f    %7.4f  %7.4f  %7.4f\n",
               ii,
               su,
               sw,
               angvar.nwrap(),
               (theta - angvar.theta()) / M_PI,
               (theta - theta_prev) / M_PI,
               theta / M_PI,
               angvar.theta() / M_PI);

        theta_prev = theta;
        omega_prev = omega;
    }

    return 0;
}
} // namespace jeod
