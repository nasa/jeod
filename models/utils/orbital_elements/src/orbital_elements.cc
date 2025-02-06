/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup OrbitalElements
 * @{
 *
 * @file models/utils/orbital_elements/src/orbital_elements.cc
 * Define methods for the OrbitalElements class.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
   (((Vallado, D.) (Fundamentals of Astrodynamics and Applications, 2nd Ed.)
     (Kluwer Acedemic Publishers) (2001) (Pages 135-230) (ISBN:0-07-066834-5))
    ((Montenbruck, O. and Gill, E.) (Satellite Orbits)
     (Springer) (2000) (Pages 157-190) (ISBN:3 540 67280))
    ((Pedro Ramon Escobal) (Methods of Orbital Determination)
     (Krieger Publishing) (1976) (pages 393-422) (ISBN:0-88275-319-3)))

Assumptions and limitations:
  ((This class works for all types of orbits.))

Library dependencies:
  ((orbital_elements.cc)
   (orbital_elements_messages.cc)
   (utils/sim_interface/src/memory_interface.cc)
   (utils/message/src/message_handler.cc))



*******************************************************************************/


// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/orbital_elements.hh"
#include "../include/orbital_elements_messages.hh"



//! Namespace jeod
namespace jeod {

/**
 * Construct a OrbitalElements object.
 */
OrbitalElements::OrbitalElements (
   void )
{
   // Orbit Definition Parameters
   semi_major_axis  = 0.0; // M     Semi-major-axis (a)
   semiparam        = 0.0; // M     Semiparameter (p)
   e_mag            = 0.0; // one   Magnitude of eccentricity (e)
   inclination      = 0.0; // r     Orbit inclination (i)
   arg_periapsis    = 0.0; // r     Argument of periapsis (w)
   long_asc_node    = 0.0; // r     Longitude of ascending node (Omega)

   // Orbital Position Parameters
   r_mag            = 0.0; // M     Magnitude of orbital radius
   vel_mag          = 0.0; // M/s   Magnitude of orbital velocity
   true_anom        = 0.0; // r     True Anomaly (v)
   mean_anom        = 0.0; // r     Mean Anomaly (M)
   mean_motion      = 0.0; // r/s   Mean motion of orbit (n)
   orbital_anom     = 0.0; /* r     Eccentric (E), Hyperbolic (H), or
                                    Parabolic (B) anomaly */
   // "Global" Working Parameters
   sin_v            = 0.0; // --    Sine of the true anomaly
   cos_v            = 1.0; // --    Cosine of the true anomaly
   orb_energy       = 0.0; // M2/s2 Specific orbital energy
   orb_ang_momentum = 0.0; // M2/s  Specific orbital angular momentum

   return;
}


/**
 * Destroy a OrbitalElements object.
 */
OrbitalElements::~OrbitalElements (
   void )
{
  return;
}


/**
 * Set the object name.
 * \param[in] name Orbital object name
 */
void OrbitalElements::set_object_name (
   const char * name)
{
   // Make sure the string exists
   if (name != nullptr) {

      object_name = name;

   }

   return;
}


/**
 * Set the planet name.
 * \param[in] name Name of planet about which the object orbits
 */
void OrbitalElements::set_planet_name (
   const char * name )
{
   // Make sure the string exists
   if (name != nullptr) {

      planet_name = name;

   }

   return;
}


/**
 * Return the object name.
 * @return Const pointer to name
 */
const char *
OrbitalElements::get_object_name (
   void )
const
{
   return object_name.c_str();
}


/**
 * Return the planet name.
 * @return Const pointer to name
 */
const char *
OrbitalElements::get_planet_name (
   void )
const
{
   return planet_name.c_str();
}



/******************************************************************************
PURPOSE:
   (Calculation of Orbital Elements from Inertial Cartesian Coordinates and the
    planetary gravitational parameter. Elements include semi-major-axis -a-,
    semiparameter -p-, eccentricity -e-, inclination -i-, longitude of the
    ascending node -Omega-, argument of periapsis -w-, true anomaly -v-,
    mean anomaly -M, and eccentric -E-, hyperbolic -H-, or parabolic anomaly
    -B-.)

REFERENCE:
   (((Vallado, David A.) (Fundamentals of Astrodynamics and Applications)
     (McGraw-Hill) (New York) (1997)))

ASSUMPTIONS AND LIMITATIONS:
   ((Input cartesian coordinates are assumed to be in an inertial frame.))

CLASS:
   (scheduled)


******************************************************************************/
int OrbitalElements::from_cartesian(
         double mu,
   const double pos[3],
   const double vel[3])
{

   // Declare local variables
   int      i;                  // Loop counter
   double   ang_momntm[3];      // Angular momentum vector
   double   line_of_nodes[3];   // Line-of-nodes vector
   double   pos_dot_vel;        // Dot product of the position and velocity
   double   eccentricity[3];    // Orbit eccentricity vector
   double   K[3];               // Unit vector in K direction of inertial frame
   double   I[3];               // Unit vector in I direction of inertial frame
   double   v2;                 // Magnitude of the velocity squared
   double   tolerance;          // Tolerance for e and i
   double   orbit_switch_tol;   // Tolerance for orbit type switch
   double   cross_vec[3];       // cross product used for angle calculations
   double   sin_in;             // sine value for input to atan2
   double   cos_in;             // cosine value for input to atan2

   // Set tolerance values
   tolerance = 1.0e-13;
   orbit_switch_tol = 1.0e-2;

   // Define unit vector in K direction of inertial frame
   K[0] = 0.0;
   K[1] = 0.0;
   K[2] = 1.0;

   /* NOTE: In December of 2005, the code was changed to calculate
      angles using atan2 instead of acos for robustness.  Some new
      variables were added and some unecessary ones were removed.
      The changes to the code are denoted by SWP 2005 */

   // Define unit vector in I direction of inertial frame (SWP 2005)
   I[0] = 1.0;
   I[1] = 0.0;
   I[2] = 0.0;

   // Compute the magnitude of position and velocity
   r_mag = Vector3::vmag (pos);
   vel_mag = Vector3::vmag (vel);

   // Compute the angular momentum vector and magnitude
   Vector3::cross (pos, vel, ang_momntm);
   orb_ang_momentum = Vector3::vmag (ang_momntm);

   // Compute the line of nodes vector
    Vector3::cross (K, ang_momntm, line_of_nodes);

   // Compute the eccentricity vector and magnitude
   pos_dot_vel = Vector3::dot (pos, vel);
   v2 = vel_mag * vel_mag;
   for (i = 0; i < 3; i++) {
      eccentricity[i] = (v2 - mu/r_mag) * pos[i]/mu -
                        (pos_dot_vel * vel[i]/mu);
   }
   e_mag = Vector3::vmag (eccentricity);

   // Compute the specific mechanical energy
   orb_energy = (v2 / 2.0) - (mu / r_mag);


   // Compute semi-major-axis, semiparameter, and mean motion
   if (e_mag < tolerance) {   // Circular orbit

      semi_major_axis = r_mag;
      semiparam = r_mag;
      mean_motion = sqrt(mu/semi_major_axis) / semi_major_axis;
   }
   else if (e_mag < (1.0 - orbit_switch_tol)) {   // Eccentric orbit

      semi_major_axis = - mu / (2.0 * orb_energy);
      semiparam = semi_major_axis * (1.0 - e_mag*e_mag);
      mean_motion = sqrt(mu/semi_major_axis) / semi_major_axis;
   }
   else if (e_mag > (1.0 + orbit_switch_tol)) {   // Hyperbolic orbit

      semi_major_axis = - mu / (2.0 * orb_energy);
      semiparam = semi_major_axis * (1.0 - e_mag*e_mag);
      mean_motion = sqrt(mu / -semi_major_axis) / -semi_major_axis;
   }
   else {   // Parabolic orbit

      // Null out semi-major axis since it is actually infinity
      semi_major_axis = 0.0;

      semiparam = orb_ang_momentum*orb_ang_momentum / mu;
      mean_motion = 2.0 * sqrt(mu/semiparam) / semiparam;
   }


   // Compute the orbital inclination (SWP 2005)
   Vector3::cross (K, ang_momntm, cross_vec);
   sin_in = Vector3::vmag (cross_vec);
   cos_in = Vector3::dot(K, ang_momntm);
   inclination = atan2(sin_in, cos_in);

   // Compute longitude of ascending node, argument of periapsis, & true anomaly
   if ((inclination < tolerance) ||
        ((M_PI-tolerance) < inclination)) {   // Equatorial orbit

      if (e_mag < tolerance) {   // Equatorial and circular orbit
         long_asc_node = 0.0;
         arg_periapsis = 0.0;

         // SWP 2005
         Vector3::cross (I, pos, cross_vec);
         sin_in = Vector3::vmag (cross_vec);
         cos_in = Vector3::dot (I, pos);
         true_anom = atan2(sin_in, cos_in);

         if (inclination < tolerance) {
            if (pos[1] < 0.0) {
               true_anom = 2.0*M_PI - true_anom;
            }
         }
         else {
            if (pos[1] > 0.0) {
               true_anom = 2.0*M_PI - true_anom;
            }
         }
      }

      else {   // Equatorial and non-circular orbit
         long_asc_node = 0.0;

         // SWP 2005
         Vector3::cross (I, eccentricity, cross_vec);
         sin_in = Vector3::vmag (cross_vec);
         cos_in = Vector3::dot (I, eccentricity);
         arg_periapsis = atan2(sin_in, cos_in);

         if (inclination < tolerance) {
            if (eccentricity[1] < 0.0) {
               arg_periapsis  =  2.0*M_PI - arg_periapsis;
            }
         }
         else {
            if (eccentricity[1] > 0.0) {
               arg_periapsis  =  2.0*M_PI - arg_periapsis;
            }
         }

         // SWP 2005
         Vector3::cross (eccentricity, pos, cross_vec);
         sin_in = Vector3::vmag (cross_vec);
         cos_in = Vector3::dot (eccentricity, pos);
         true_anom = atan2(sin_in, cos_in);

         if (pos_dot_vel < 0.0) {
            true_anom = 2.0*M_PI - true_anom;
         }
      }
   }

   else if (e_mag < tolerance) {   // Circular and non-equatorial orbit

      // SWP 2005
      Vector3::cross (I, line_of_nodes, cross_vec);
      sin_in = Vector3::vmag (cross_vec);
      cos_in = Vector3::dot (I, line_of_nodes);
      long_asc_node = atan2(sin_in, cos_in);

      if (line_of_nodes[1] < 0.0) {
         long_asc_node = 2.0*M_PI - long_asc_node;
      }

      arg_periapsis = 0.0;

      // SWP 2005
      Vector3::cross (line_of_nodes, pos, cross_vec);
      sin_in = Vector3::vmag (cross_vec);
      cos_in = Vector3::dot (line_of_nodes, pos);
      true_anom = atan2(sin_in, cos_in);

      if (pos[2] < 0.0) {
         true_anom = 2.0*M_PI - true_anom;
      }
   }

   else {   // Non-circular and non-equatorial orbit

      // SWP 2005
      Vector3::cross (I, line_of_nodes, cross_vec);
      sin_in = Vector3::vmag (cross_vec);
      cos_in = Vector3::dot (I, line_of_nodes);
      long_asc_node = atan2(sin_in, cos_in);

      if (line_of_nodes[1] < 0.0) {
         long_asc_node = 2.0*M_PI - long_asc_node;
      }

      // SWP 2005
      Vector3::cross (line_of_nodes, eccentricity, cross_vec);
      sin_in = Vector3::vmag (cross_vec);
      cos_in = Vector3::dot (line_of_nodes, eccentricity);
      arg_periapsis = atan2(sin_in, cos_in);

      if (eccentricity[2] < 0.0) {
         arg_periapsis  =  2.0*M_PI - arg_periapsis;
      }

      // SWP 2005
      Vector3::cross (eccentricity, pos, cross_vec);
      sin_in = Vector3::vmag (cross_vec);
      cos_in = Vector3::dot (eccentricity, pos);
      true_anom = atan2(sin_in, cos_in);

      if (pos_dot_vel < 0.0) {
         true_anom = 2.0*M_PI - true_anom;
      }
   }

   // Compute the mean anomaly & the eccentric, hyperbolic, or parabolic anomaly
   nu_to_anomalies();

   return (0);
}


/******************************************************************************
PURPOSE:
    (Calculation of Inertial Cartesian Coordinates from Orbital Elements
     semiparameter -p-, eccentricity -e-, inclination -i-, longitude of the
     ascending node -Omega-, argument of periapsis -w-, mean anomaly -M-,
     and gravitational parameter -mu-.)

REFERENCE:
    (((Vallado, D.) (Fundamentals of Astrodynamics and Applications, 2nd Ed.)
      (Kluwer Acedemic Publishers) (2001) (Pages 135-230) (ISBN:0-07-066834-5))
     ((Montenbruck, O. and Gill, E.) (Satellite Orbits)
      (Springer) (2000) (Pages 157-190) (ISBN:3 540 67280))
     ((Pedro Ramon Escobal) (Methods of Orbital Determination)
      (Krieger Publishing) (1976) (pages 393-422) (ISBN:0-88275-319-3)))

ASSUMPTIONS AND LIMITATIONS:
    ((This routine is intended to be used at initialization only.)
     (Output cartesian coordinates are in an inertial frame.)
     (This routine works for all types of orbits.))

CLASS:
    (initialization)


******************************************************************************/
int OrbitalElements::to_cartesian(
    double mu,
    double pos[3],
    double vel[3])
{

   // Declare local variables
   double tolerance = 1.0e-13;    // Tolerance for e and i
   double p;                      // Semiparameter
   double e;                      // Magnitude of eccentricity
   double r_pqw[3];               // Position vector in the PQW coordinate frame
   double v_pqw[3];               // Velocity vector in the PQW coordinate frame
   double T_PQW_Inrtl[3][3];      // Transformation matrix from PQW to Inertial
   double sO;                     // Sine of the longitude of ascending node
   double cO;                     // Cosine of the longitude of ascending node
   double sw;                     // Sine of the argument of periapsis
   double cw;                     // Cosine of the argument of periapsis
   double si;                     // Sine of the inclination
   double ci;                     // Cosine of the inclination

   // Assign input values for clarity
   p = semiparam;
   e = e_mag;

   // Check for "meaningful" value of semi-parameter
   if (p <= 0.0) {
      MessageHandler::fail (
         __FILE__, __LINE__, OrbitalElementsMessages::domain_error,
         "Semi-parameter %g is zero or negative.", p);
      return (-1);
   }

   // Check for "meaningful" values of sin_v and cos_v
   double rss_sincos_v = sqrt(sin_v*sin_v + cos_v*cos_v);
   if (fabs (rss_sincos_v - 1.0) > tolerance) {
      MessageHandler::fail (
         __FILE__, __LINE__, OrbitalElementsMessages::domain_error,
         "Sine (%g) and cosine (%g) of the true anomaly are inconsistent.",
         sin_v, cos_v);
      return (-1);
   }

   // Check for meaningful value of central mass
   if (mu <= 0.0) {
      MessageHandler::fail (
      __FILE__, __LINE__, OrbitalElementsMessages::domain_error,
         "Grav mass constant (mu) is zero or negative (%f).\n"
         "This is likely a user-induced initialization error.\n",mu);
      return (-1);
   }

   // Test for equatorial and circular orbits
   if ((inclination < tolerance) ||
       ((M_PI-tolerance) < inclination)) {   // Equatorial orbit

      if (e < tolerance) {   // Equatorial and Circular orbit

         sO = 0.0;
         cO = 1.0;
         sw = 0.0;
         cw = 1.0;
      }
      else {   // Equatorial and non-circular orbit

         sO = 0.0;
         cO = 1.0;
         sw = sin(arg_periapsis);
         cw = cos(arg_periapsis);
      }

      if (inclination < tolerance) {   // Inclination is approx. zero

         si = 0.0;
         ci = 1.0;
      }
      else {   // Inclination is approx. pi

         si =  0.0;
         ci = -1.0;
      }
   }

   else if (e < tolerance) {   // Circular and non-equatorial orbit

      sO = sin(long_asc_node);
      cO = cos(long_asc_node);
      sw = 0.0;
      cw = 1.0;
      si = sin(inclination);
      ci = cos(inclination);
   }

   else {   // Non-circular and non-equatorial orbit

      sO = sin(long_asc_node);
      cO = cos(long_asc_node);
      sw = sin(arg_periapsis);
      cw = cos(arg_periapsis);
      si = sin(inclination);
      ci = cos(inclination);
   }

   // Compute position and velocity in perifocal frame (PQW)
   r_pqw[0] = (p * cos_v) / (1.0 + e*cos_v);
   r_pqw[1] = (p * sin_v) / (1.0 + e*cos_v);
   r_pqw[2] = 0.0;

   v_pqw[0] = -sqrt(mu/p) * sin_v;
   v_pqw[1] =  sqrt(mu/p) * (e+cos_v);
   v_pqw[2] =  0.0;

   // Compute transformation matrix from PQW to inertial
   T_PQW_Inrtl[0][0] =  cO * cw - sO * sw * ci;
   T_PQW_Inrtl[0][1] = -cO * sw - sO * cw * ci;
   T_PQW_Inrtl[0][2] =  sO * si;
   T_PQW_Inrtl[1][0] =  sO * cw + cO * sw * ci;
   T_PQW_Inrtl[1][1] = -sO * sw + cO * cw * ci;
   T_PQW_Inrtl[1][2] = -cO * si;
   T_PQW_Inrtl[2][0] =  sw * si;
   T_PQW_Inrtl[2][1] =  cw * si;
   T_PQW_Inrtl[2][2] =  ci;

   // Compute position and velocity in inertial frame
   Vector3::transform (T_PQW_Inrtl, r_pqw, pos);
   Vector3::transform (T_PQW_Inrtl, v_pqw, vel);

   return (0);
}


/******************************************************************************
PURPOSE:
    (Calculation of the Mean Anomaly and the Eccentric, Hyperbolic, or
     Parabolic Anomalies, given the True Anomaly and eccentricity)

REFERENCE:
    (((Vallado, D.) (Fundamentals of Astrodynamics and Applications, 2nd Ed.)
      (Kluwer Acedemic Publishers) (2001) (Pages 135-230) (ISBN:0-07-066834-5))
     ((Montenbruck, O. and Gill, E.) (Satellite Orbits)
      (Springer) (2000) (Pages 157-190) (ISBN:3 540 67280))
     ((Pedro Ramon Escobal) (Methods of Orbital Determination)
      (Krieger Publishing) (1976) (pages 393-422) (ISBN:0-88275-319-3)))

ASSUMPTIONS AND LIMITATIONS:
    ((This routine works for all types of orbits.))

CLASS:
    (N/A)


******************************************************************************/
int OrbitalElements::nu_to_anomalies()
{

   // Declare local variables
   double  cir_tol = 1.0e-13 ;  // Tolerance for circular orbit
   double  switch_tol = 1.0e-2; // Tolerance for use of ecc, par, hyp anom
   double  v;                   // True Anomaly
   double  e;                   // Magnitude of eccentricity
   double  sin_E;               // Sine of the Eccentric Anomaly
   double  cos_E;               // Cosine of the Eccentric Anomaly
   double  sinh_H;              // Hyperbolic sine of the Hyperbolic Anomaly

   // Assign input parameters for clarity
   v = true_anom;
   e = e_mag;
   sin_v = sin(true_anom);
   cos_v = cos(true_anom);

   // Circular orbit
   if (e < cir_tol) {

      orbital_anom = v;
      mean_anom = v;
   }

   // Eccentric orbit
   else if (e < (1.0 - switch_tol)) {

      sin_E = sqrt(1.0 - e*e)*sin_v / (1.0 + e*cos_v);
      cos_E = (e + cos_v) / (1.0 + e*cos_v);
      orbital_anom = atan2(sin_E, cos_E);

      if (orbital_anom < 0.0) {
         orbital_anom += 2.0*M_PI;
      }

      mean_anom = orbital_anom - e*sin(orbital_anom);
   }

   // Hyperbolic orbit
   else if (e > (1.0 + switch_tol)) {

      sinh_H = sqrt(e*e - 1.0) * sin_v / (1.0 + e*cos_v);
      orbital_anom  = asinh(sinh_H);
      mean_anom = e*sinh(orbital_anom) - orbital_anom;
   }

   // Parabolic orbit
   else {
      orbital_anom = tan( v / 2.0);
      mean_anom = orbital_anom + pow(orbital_anom, 3.0)/ 3.0;
   }

   return (0);
}


/******************************************************************************
PURPOSE:
    (Calculation of the True Anomaly from the Mean Anomaly and Eccentricity.)

REFERENCE:
    (((Vallado, David A.) (Fundamentals of Astrodynamics and Applications)
      (McGraw-Hill) (New York) (1997)))

ASSUMPTIONS AND LIMITATIONS:
    ((This routine works for all types of orbits.))

CLASS:
    (N/A)


******************************************************************************/
int OrbitalElements::mean_anom_to_nu()
{

   // Declare local variables
   int     Converge;            // Convergence flag for iteration
   double  cir_tol = 1.0e-13 ;  // Tolerance for circular orbit
   double  switch_tol = 1.0e-2; // Tolerance for use of ecc, par, hyp anom
   double  M;                   // Mean Anomaly
   double  e;                   // Eccentricity magnitude
   double  B;                   // Parabolic Anomaly
   double  sin_E;               // Sine of the Eccentric Anomaly
   double  cos_E;               // Cosine of the Eccentric Anomaly
   double  sinh_H;              // Hyperbolic Sine of the Hyperbolic Anomaly
   double  cosh_H;              // Hyperbolic Cosine of Hyperbolic Anomaly

   // Assign input parameters for clarity
   M = mean_anom;
   e = e_mag;

   // Circular orbit
   if (e < cir_tol) {

      // Compute the eccentric anomaly
      orbital_anom = M;

      // Compute the true anomaly
      sin_v = sin(M);
      cos_v = cos(M);
      true_anom = M;
   }

   // Eccentric orbit
   else if (e < (1.0 - switch_tol)) {

      // Use Kepler's Equation to compute the eccentric anomaly
      Converge = KepEqtnE(M, e, &(orbital_anom));

      // Check iteration convergence
      if (Converge == -1) {
         MessageHandler::fail (
            __FILE__, __LINE__, OrbitalElementsMessages::convergence_error,
            "Attempted solution for eccentric Kepler's equation "
            "failed to converge.");
         return (-1);
      }

      // Compute the true anomaly
      sin_E = sin(orbital_anom);
      cos_E = cos(orbital_anom);
      sin_v = sqrt(1.0 - e*e)*sin_E / (1.0 - e*cos_E);
      cos_v = (cos_E - e) / (1.0 - e*cos_E);
      true_anom = atan2(sin_v, cos_v);

      if (true_anom < 0.0) {
         true_anom += 2.0 * M_PI;
      }
   }

   // Hyperbolic orbit
   else if (e > (1.0 + switch_tol)) {

      // Use Kepler's Equation to compute the hyperbolic anomaly
      Converge = KepEqtnH(M, e, &(orbital_anom));

      // Check iteration convergence
      if (Converge == -1) {
         MessageHandler::fail (
            __FILE__, __LINE__, OrbitalElementsMessages::convergence_error,
            "Attempted solution for hyperbolic Kepler's equation "
            "failed to converge.");
         return (-1);
      }

      // Compute the true anomaly
      sinh_H = sinh(orbital_anom);
      cosh_H = cosh(orbital_anom);
      sin_v = -sqrt(e*e - 1.0)*sinh_H / (1.0 - e*cosh_H);
      cos_v = (cosh_H - e) / (1.0 - e*cosh_H);
      true_anom = atan2(sin_v, cos_v);

      if (true_anom < 0.0) {
         true_anom += 2.0 * M_PI;
      }
   }

   // Parabolic orbit
   else {

      // Use Kepler's Equation to compute the parabolic anomaly
      KepEqtnB(M, &(orbital_anom));

      // Assign input parameter for clarity
      B = orbital_anom;

      // Compute the true anomaly
      sin_v  = 2*B / (1.0 + B*B);
      cos_v  = (1.0 - B*B) / (1.0 + B*B);
      true_anom = atan2(sin_v, cos_v);

      if (true_anom < 0.0) {
         true_anom += 2.0 * M_PI;
      }
   }

   return (0);
}


/******************************************************************************
PURPOSE:
    (Calculation of Eccentric Anomaly from the Mean Anomaly and Eccentricity)

REFERENCE:
    (((Vallado, David A.) (Fundamentals of Astrodynamics and Applications)
      (McGraw-Hill) (New York) (1997)))

ASSUMPTIONS AND LIMITATIONS:
    ((Only call this routine for elliptical orbits: e < 1.0)
     (If a "-1" is returned, the solution did not converge; otherwise, the
      number of iterations required is returned))

CLASS:
    (N/A)


******************************************************************************/
int OrbitalElements::KepEqtnE(
   double   M,
   double   e,
   double * E)
{

   // Declare local variables
   int     i;                       // Loop counter
   double  tolerance = 1.0e-8;      // Iteration tolerance for eccentric anomaly
   double  pi = 3.141592653589793;  /* Value of pi use for computing initial
                                       guess for eccentric anomaly */

   // Compute initial guess for E
   if ( ((-pi < M) && (M < 0.0)) || (M > pi) ) {
      *E = M - e;
   }
   else {
      *E = M + e;
   }

   // Begin looping through iterations
   for (i = 1; i <= 1000; i++) {
      double E_prev = *E; // Previous value of the eccentric anomaly
      *E = E_prev + ((M - E_prev + e*sin(E_prev)) / (1.0 - e*cos(E_prev)));

      // If iteration has converged, return number of iterations
      if (fabs(*E - E_prev) < tolerance) {
         return (i);
      }
   }

   // If the iteration does not converge, return -1
   return (-1);
}


/******************************************************************************
PURPOSE:
    (Calculation of the Hyperbolic Anomaly from the Mean Anomaly and the
     Eccentricity)

REFERENCE:
    (((Vallado, David A.) (Fundamentals of Astrodynamics and Applications)
      (McGraw-Hill) (New York) (1997)))

ASSUMPTIONS AND LIMITATIONS:
    ((Only call this routine for hyperbolic orbits: e > 1.0)
     (If a "-1" is returned, the solution did not converge; otherwise, the
      number of iterations required is returned))

CLASS:
    (N/A)


******************************************************************************/

int OrbitalElements::KepEqtnH(
   double   M,
   double   e,
   double * H)
{

   // Declare local variables
   int     i;                      // Loop counter
   double  tolerance = 1.0e-08;    // Iteration tolerance for hyperbolic anomaly
   double  pi = 3.141592653589793; /* Value of pi use for computing initial
                                      guess for hyperbolic anomaly */

   // Compute initial guess for H
   if (e < 1.6) {
      if ( ((-pi < M) && (M < 0.0)) || (M > pi) ) {
         *H = M - e;
      }
      else {
         *H = M + e;
      }
   }
   else {
      if ((e < 3.6) && (fabs(M) > pi)) {
         if (M >= 0.0) {
            *H = M - e;
         }
         else {
            *H = M + e;
         }
      }
      else {
         *H = M / (e - 1.0);
      }
   }

   // Begin looping through iterations
   for (i = 1; i <= 1000; i++) {
      double H_prev = *H; // Previous value of hyperbolic anomaly
      *H = H_prev + ( (M + H_prev - e*sinh(H_prev)) / (e*cosh(H_prev) - 1.0));

      // If iteration has converged, return number of iterations
      if (fabs(*H - H_prev) < tolerance) {
         return (i);
      }
   }

   // If the iteration does not converge, return -1
   return (-1);
}


/******************************************************************************
PURPOSE:
    (Calculation of the Parabolic Anomaly from the Mean Anomaly)

REFERENCE:
    (((Vallado, David A.) (Fundamentals of Astrodynamics and Applications)
      (McGraw-Hill) (New York) (1997)))

ASSUMPTIONS AND LIMITATIONS:
    ((Only call this routine for Parabolic orbits: e = 1.0))

CLASS:
    (N/A)

 
******************************************************************************/
int OrbitalElements::KepEqtnB(
   double   M,
   double * B)
{

   // Declare local variables
   double M_in ; // Used when M is less than zero SWP 12/05
   double s, w;  // Intermediate computational parameters

   // SWP 12/05
   if (M < 0)
   {
      M_in = M + 2*M_PI;
   }
   else
   {
      M_in = M;
   }

   // Compute the parabolic anomaly
   s = 0.5 * atan2(1.0, (1.5*M_in));
   w = atan2(pow(tan(s), (1.0/3.0)), 1);
   *B = 2.0 / tan(2.0*w);

   return (0);
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
