/* Generated from environment/gravity/verif/data/grav_verif.d */

/*******************************************************************************

Purpose:
  (Gravitation potential function parameter initialization.)

References:
  (((Gottlieb, Robert G.) (Fast Gravity, Gravity Partials, Normalized
     Gravity, Gravity Gradient Torque and Magnetic Field: Derivation,
     Code and Data)(NASA Contractor Report 188243)(NASA Johnson Space
     Center Document 23762) (Feb 1993) (--)))

Assumptions and Limitations:
  ((Fictitious gravity coefficients for verification only - do NOT use for
    actual orbit sim work))


*******************************************************************************/


class SphericalHarmonicsGravitySource;
#define JEOD_FRIEND_CLASS SphericalHarmonicsGravitySource_grav_verif_default_data

// JEOD includes
#include "environment/gravity/include/gravity_source.hh"
#include "environment/gravity/include/spherical_harmonics_gravity_source.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/grav_verif.hh"



//! Namespace jeod
namespace jeod {

void
SphericalHarmonicsGravitySource_grav_verif_default_data::initialize (
   SphericalHarmonicsGravitySource * SphericalHarmonicsGravitySource_ptr)
{
   SphericalHarmonicsGravitySource_ptr->name = "Earth";   // fictitious body

   SphericalHarmonicsGravitySource_ptr->degree = 5;
   SphericalHarmonicsGravitySource_ptr->order = 5;
   SphericalHarmonicsGravitySource_ptr->mu = 3.986004415E+14;
   SphericalHarmonicsGravitySource_ptr->radius = 6378136.3;

   SphericalHarmonicsGravitySource_ptr->Cnm =
      JEOD_ALLOC_PRIM_ARRAY (5 + 1, double*);
   SphericalHarmonicsGravitySource_ptr->Snm =
      JEOD_ALLOC_PRIM_ARRAY (5 + 1, double*);

   /* FULLY NORMALIZED GRAVITY COEFFICIENTS (unitless) */
   SphericalHarmonicsGravitySource_ptr->Cnm[  2] =
      JEOD_ALLOC_PRIM_ARRAY (3, double);
   SphericalHarmonicsGravitySource_ptr->Cnm[  2][  0] =  3.340052631095518E-08;
   SphericalHarmonicsGravitySource_ptr->Cnm[  2][  1] =  1.656759356170754E-08;
   SphericalHarmonicsGravitySource_ptr->Cnm[  2][  2] = -8.176775024351829E-09;
   SphericalHarmonicsGravitySource_ptr->Cnm[  3] =
      JEOD_ALLOC_PRIM_ARRAY (4, double);
   SphericalHarmonicsGravitySource_ptr->Cnm[  3][  0] =  1.759117523226707E-12;
   SphericalHarmonicsGravitySource_ptr->Cnm[  3][  1] =  3.832276836797579E-12;
   SphericalHarmonicsGravitySource_ptr->Cnm[  3][  2] =  8.885825377841406E-14;
   SphericalHarmonicsGravitySource_ptr->Cnm[  3][  3] = -1.081882767554426E-12;
   SphericalHarmonicsGravitySource_ptr->Cnm[  4] =
      JEOD_ALLOC_PRIM_ARRAY (5, double);
   SphericalHarmonicsGravitySource_ptr->Cnm[  4][  0] = -9.608634091968111E-15;
   SphericalHarmonicsGravitySource_ptr->Cnm[  4][  1] =  1.532755741714554E-15;
   SphericalHarmonicsGravitySource_ptr->Cnm[  4][  2] =  1.514697168098408E-15;
   SphericalHarmonicsGravitySource_ptr->Cnm[  4][  3] =  1.212195117185278E-14;
   SphericalHarmonicsGravitySource_ptr->Cnm[  4][  4] =  1.098630790621571E-15;
   SphericalHarmonicsGravitySource_ptr->Cnm[  5] =
      JEOD_ALLOC_PRIM_ARRAY (6, double);
   SphericalHarmonicsGravitySource_ptr->Cnm[  5][  0] =  7.689778516079726E-19;
   SphericalHarmonicsGravitySource_ptr->Cnm[  5][  1] =  5.103903289685398E-19;
   SphericalHarmonicsGravitySource_ptr->Cnm[  5][  2] =  1.288578200168744E-18;
   SphericalHarmonicsGravitySource_ptr->Cnm[  5][  3] =  4.127243225918561E-19;
   SphericalHarmonicsGravitySource_ptr->Cnm[  5][  4] =  6.153203543016434E-19;
   SphericalHarmonicsGravitySource_ptr->Cnm[  5][  5] =  7.718691941720859E-19;

   SphericalHarmonicsGravitySource_ptr->Snm[  2] =
      JEOD_ALLOC_PRIM_ARRAY (3, double);
   SphericalHarmonicsGravitySource_ptr->Snm[  2][  0] =  0.000000000000000E+00;
   SphericalHarmonicsGravitySource_ptr->Snm[  2][  1] =  9.855566979885050E-09;
   SphericalHarmonicsGravitySource_ptr->Snm[  2][  2] =  9.269248429468815E-09;
   SphericalHarmonicsGravitySource_ptr->Snm[  3] =
      JEOD_ALLOC_PRIM_ARRAY (4, double);
   SphericalHarmonicsGravitySource_ptr->Snm[  3][  0] =  0.000000000000000E+00;
   SphericalHarmonicsGravitySource_ptr->Snm[  3][  1] = -1.471910512326285E-12;
   SphericalHarmonicsGravitySource_ptr->Snm[  3][  2] =  1.828488643978194E-12;
   SphericalHarmonicsGravitySource_ptr->Snm[  3][  3] = -9.045529521634635E-13;
   SphericalHarmonicsGravitySource_ptr->Snm[  4] =
      JEOD_ALLOC_PRIM_ARRAY (5, double);
   SphericalHarmonicsGravitySource_ptr->Snm[  4][  0] =  0.000000000000000E+00;
   SphericalHarmonicsGravitySource_ptr->Snm[  4][  1] = -3.539212041089171E-15;
   SphericalHarmonicsGravitySource_ptr->Snm[  4][  2] =  4.836572455402800E-16;
   SphericalHarmonicsGravitySource_ptr->Snm[  4][  3] = -1.135418848925473E-15;
   SphericalHarmonicsGravitySource_ptr->Snm[  4][  4] = -1.950021729407439E-15;
   SphericalHarmonicsGravitySource_ptr->Snm[  5] =
      JEOD_ALLOC_PRIM_ARRAY (6, double);
   SphericalHarmonicsGravitySource_ptr->Snm[  5][  0] =  0.000000000000000E+00;
   SphericalHarmonicsGravitySource_ptr->Snm[  5][  1] = -1.270409281937703E-18;
   SphericalHarmonicsGravitySource_ptr->Snm[  5][  2] = -3.356781430612713E-19;
   SphericalHarmonicsGravitySource_ptr->Snm[  5][  3] =  3.200323268358486E-19;
   SphericalHarmonicsGravitySource_ptr->Snm[  5][  4] = -6.131745252702638E-19;
   SphericalHarmonicsGravitySource_ptr->Snm[  5][  5] =  1.485405551920983E-19;
}
} // End JEOD namespace
