/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup RNPJ2000
 * @{
 *
 * @file models/environment/RNP/RNPJ2000/src/precession_j2000.cc
 * Implementation for PrecessionJ2000
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((Mulcihy, David D. and Bond, Victor R.)
    (The RNP Routine for the STandard Epoch J2000)
    (NASA:JSC-24574) (NASA Engineering Directorate, Navigation, Control,
     and Aeronautics Division) (September 1990) (--)))

Assumptions and limitations:
  ((Earth specific))

Class:
  (PrecessionJ2000)

Library dependencies:
  ((precession_j2000.cc)
   (environment/RNP/GenericRNP/src/planet_rotation.cc))

 

*******************************************************************************/
#include "../include/precession_j2000.hh"

//! Namespace jeod
namespace jeod {

/**
 * constructor
 */
PrecessionJ2000::PrecessionJ2000 (
   void)
{
}

/**
 * destructor
 */
PrecessionJ2000::~PrecessionJ2000 (
   void)
{
}

/**
 * Precession J2000 specific implementation of update_rotation,
 * inherited from Planetrotation
 */
void
PrecessionJ2000::update_rotation (
   void)
{

   // define the three parameters, eta theta and z, that are needed
   // for the precession matrix. Also define time2 and time3, time
   // squared and cubed

   double zeta  = 0.0;
   double theta = 0.0;
   double z     = 0.0;
   double time  = current_time;
   double time2 = time * time;
   double time3 = time2 * time;

   // based on time as centuries since julian epoch (J2000), calculate
   // those three params
   zeta  = 2306.2181 * time + 0.30188 * time2 + 0.017998 * time3;
   theta = 2004.3109 * time - 0.42665 * time2 - 0.041833 * time3;
   z     = 2306.2181 * time + 1.09468 * time2 + 0.018203 * time3;

   // convert these from seconds to degrees
   // these are * 1/DEGTOSEC in an effort to numerically match 1.52
   zeta  = (zeta / DEGTOSEC) * DEGTORAD;
   theta = (theta / DEGTOSEC) * DEGTORAD;
   z     = (z / DEGTOSEC) * DEGTORAD;
   // take trigs of the three parameters for inclusion in the matrix

   double c_zeta  = cos (zeta);
   double s_zeta  = sin (zeta);
   double c_theta = cos (theta);
   double s_theta = sin (theta);
   double c_z     = cos (z);
   double s_z     = sin (z);

   // now populate the rotation matrix with the trig functions,
   // as follows:
   // celestial frame = rot_z(zeta) * rot_y(-theta) * rot_z(z) * terrest.

   rotation[0][0] = c_theta * c_z * c_zeta - s_z * s_zeta;
   rotation[0][1] = s_z * c_theta * c_zeta + s_zeta * c_z;
   rotation[0][2] = c_zeta * s_theta;

   rotation[1][0] = -s_zeta * c_theta * c_z - c_zeta * s_z;
   rotation[1][1] = -s_z * s_zeta * c_theta + c_z * c_zeta;
   rotation[1][2] = -s_zeta * s_theta;

   rotation[2][0] = -s_theta * c_z;
   rotation[2][1] = -s_theta * s_z;
   rotation[2][2] = c_theta;

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
