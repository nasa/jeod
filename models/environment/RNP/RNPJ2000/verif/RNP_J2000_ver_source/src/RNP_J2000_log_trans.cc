/********************************* TRICK HEADER ***************************

PURPOSE:
   (Verification of J2000 RNP, this structure stores transposes of the
    individual matrices stored in the nutation, precession, rotation,
    and polar motion objects in the RNP.)

REFERENCE:
   (((none)))

ASSUMPTIONS AND LIMITATIONS:
   (none)



************************************************************************/

#include "../include/RNP_J2000_log_trans.hh"

#include "utils/math/include/matrix3x3.hh"

//! Namespace jeod
namespace jeod
{

J2000RNPLogging::J2000RNPLogging(const J2000RNPLogging & rhs)
{
    Matrix3x3::copy(rhs.prec_trans, prec_trans);
    Matrix3x3::copy(rhs.nut_trans, nut_trans);
    Matrix3x3::copy(rhs.pm_trans, pm_trans);
    Matrix3x3::copy(rhs.rot_trans, rot_trans);
}

J2000RNPLogging & J2000RNPLogging::operator=(const J2000RNPLogging & rhs)
{
    if(this == &rhs)
    {
        return *this;
    }

    Matrix3x3::copy(rhs.prec_trans, prec_trans);
    Matrix3x3::copy(rhs.nut_trans, nut_trans);
    Matrix3x3::copy(rhs.pm_trans, pm_trans);
    Matrix3x3::copy(rhs.rot_trans, rot_trans);

    return *this;
}

void J2000RNPLogging::copy_rotation_transposes(PlanetRotation & precession,
                                               PlanetRotation & nutation,
                                               PlanetRotation & rotation,
                                               PlanetRotation & polar_motion)
{
    precession.get_rotation_transpose(prec_trans);
    nutation.get_rotation_transpose(nut_trans);
    rotation.get_rotation_transpose(rot_trans);
    polar_motion.get_rotation_transpose(pm_trans);
}
} // namespace jeod
