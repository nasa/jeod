/*******************************TRICK HEADER******************************
Purpose:
  (Class for testing gravity models.)
***********************************************************************/

#include "../include/gravity_verif.hh"

// JEOD includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"

#define _USE_MATH_DEFINES_ // M_PI
#include <cmath> // M_PI sin, cos.
#include <iostream>


//! Namespace jeod 
namespace jeod {

/*****************************************************************************
generate_pfix_position
Purpose:()
*****************************************************************************/
void
PointMassProperties::generate_position()
{
   position[0] = std::cos( latitude) * std::cos( longitude);
   position[1] = std::cos( latitude) * std::sin( longitude);
   position[2] = std::sin( latitude);
   Vector3::scale( radius,
                   position);
}


/*****************************************************************************
cosntructor
*****************************************************************************/
GravityVerification::GravityVerification()
  :
  rad_per_deg(M_PI / 180.0),
  G(6.673E-11)
{

  double baseline_mass  = (3.986004415E+14/ G) / num_pts;

  gravity_pt[0].mass       =  baseline_mass;
  gravity_pt[0].latitude = 45.0 * rad_per_deg;
  gravity_pt[0].longitude =  0.0 * rad_per_deg;
  gravity_pt[0].radius =  4000.0;

  gravity_pt[1].mass       =  baseline_mass;
  gravity_pt[1].latitude = 45.0 * rad_per_deg;
  gravity_pt[1].longitude =  120.0 * rad_per_deg;
  gravity_pt[1].radius =  4000.0;

  gravity_pt[2].mass       =  baseline_mass;
  gravity_pt[2].latitude = 45.0 * rad_per_deg;
  gravity_pt[2].longitude =  240.0 * rad_per_deg;
  gravity_pt[2].radius =  4000.0;

  gravity_pt[3].mass       =  baseline_mass;
  gravity_pt[3].latitude = -45.0 * rad_per_deg;
  gravity_pt[3].longitude =  180.0 * rad_per_deg;
  gravity_pt[3].radius =  4000.0;

  gravity_pt[4].mass       =  baseline_mass;
  gravity_pt[4].latitude = -45.0 * rad_per_deg;
  gravity_pt[4].longitude =  300.0 * rad_per_deg;
  gravity_pt[4].radius =  4000.0;

  gravity_pt[5].mass       =  baseline_mass;
  gravity_pt[5].latitude = -45.0 * rad_per_deg;
  gravity_pt[5].longitude =  60.0 * rad_per_deg;
  gravity_pt[5].radius =  4000.0;

  gravity_pt[6].mass       =  0.8*baseline_mass;
  gravity_pt[6].latitude = 23.0 * rad_per_deg;
  gravity_pt[6].longitude =  73.0 * rad_per_deg;
  gravity_pt[6].radius =  4000.0;

  gravity_pt[7].mass       =  1.2*baseline_mass;
  gravity_pt[7].latitude = -23.0 * rad_per_deg;
  gravity_pt[7].longitude =  253.0 * rad_per_deg;
  gravity_pt[7].radius =  (0.8/1.2)*4000.0;

  gravity_pt[8].mass       =  0.6*baseline_mass;
  gravity_pt[8].latitude = 77.0 * rad_per_deg;
  gravity_pt[8].longitude =  303.0 * rad_per_deg;
  gravity_pt[8].radius =  4000.0;

  gravity_pt[9].mass       =  1.4*baseline_mass;
  gravity_pt[9].latitude = -77.0 * rad_per_deg;
  gravity_pt[9].longitude =  123.0 * rad_per_deg;
  gravity_pt[9].radius =  (0.6/1.4)*4000.0;

  gravity_pt[10].mass      =  0.6*baseline_mass;
  gravity_pt[10].latitude = 51.0 * rad_per_deg;
  gravity_pt[10].longitude =  12.0 * rad_per_deg;
  gravity_pt[10].radius =  4000.0;

  gravity_pt[11].mass      =  1.4*baseline_mass;
  gravity_pt[11].latitude = -51.0 * rad_per_deg;
  gravity_pt[11].longitude =  192.0 * rad_per_deg;
  gravity_pt[11].radius =  (0.6/1.4)*4000.0;

  for (unsigned int ii = 0; ii < num_pts; ++ii) {
    gravity_pt[ii].generate_position();
  }
}

/*****************************************************************************
verify_acc_magnitude
Purpose:(comapres the magnitudes of the acceleration)
*****************************************************************************/
void
GravityVerification::verify_acc_magnitudes(
   const double grav_accel[3],
   const double inertial_position[3])
{
  gravity_magnitude = Vector3::vmag(grav_accel);
  verif_gravity_magnitude = G / (Vector3::vmagsq(inertial_position));
  gravity_magnitude_error = verif_gravity_magnitude - gravity_magnitude;
}

/*****************************************************************************
verify_grav_accel
Purpose:(verifies the gravity acceleration)
*****************************************************************************/
void
GravityVerification::verify_grav_accel(
    const double veh_position[3],
    const double grav_accel[3])
{
  //initialize acceleration vector; this is the acceleration towards the 12
  //points.
  double verif_grav_accel[3];
  Vector3::initialize( verif_grav_accel);

  // accumulate over the 12 points
  for (unsigned int ii_grav=0; ii_grav < num_pts; ii_grav++) {

   // distance between point of interest and point mass
   // Note - assumes pfix and inertial are aligned.
   double del_pos[3];
   Vector3::diff( gravity_pt[ii_grav].position,
                  veh_position,
                  del_pos);

   // compute and accumulate acceleration by component
   double r = Vector3::vmag(del_pos);
   double G_scale = G * gravity_pt[ii_grav].mass/ (r*r*r);

   Vector3::scale_incr( del_pos,
                        G_scale,
                        verif_grav_accel);
  }

  Vector3::diff( verif_grav_accel,
                 grav_accel,
                 accel_error);
  printf("       Sph Harm Model     Pt. Mass Model          Error\n");
  printf("x %20.15f%20.15f%25.15E \n",
          grav_accel[0],
          verif_grav_accel[0],
          accel_error[0]);
  printf("y %20.15f%20.15f%25.15E \n",
          grav_accel[1],
          verif_grav_accel[1],
          accel_error[1]);
  printf("z %20.15f%20.15f%25.15E \n",
          grav_accel[2],
          verif_grav_accel[2],
          accel_error[2]);
}


/*****************************************************************************
verify_grav_torque
Purpose:(Verifies the gravity torque computation)
*****************************************************************************/
void
GravityVerification::verify_grav_torque(
   const double sc_attitude[3], // PYR in degrees relative to planet-frame
   const double veh_position[3],// position of vehicle relative to planet-center
   const double grav_torque[3]) // reference value for comaprison
{
  double s1 = std::sin(sc_attitude[0] * rad_per_deg);  // pitch
  double c1 = std::cos(sc_attitude[0] * rad_per_deg);
  double s2 = std::sin(sc_attitude[1] * rad_per_deg);  // yaw
  double c2 = std::cos(sc_attitude[1] * rad_per_deg);
  double s3 = std::sin(sc_attitude[2] * rad_per_deg);  // roll
  double c3 = std::cos(sc_attitude[2] * rad_per_deg);

  // build transformation matrix (planet-fixed to s/c body)
  // (Euler sequence 2-3-1, Schaub p.680)
  double T_pfix_to_body[3][3];
  T_pfix_to_body[0][0] =  c2*c1;
  T_pfix_to_body[0][1] =  s2;
  T_pfix_to_body[0][2] = -c2*s1;

  T_pfix_to_body[1][0] = -c3*s2*c1+s3*s1;
  T_pfix_to_body[1][1] =  c3*c2;
  T_pfix_to_body[1][2] =  c3*s2*s1+s3*c1;

  T_pfix_to_body[2][0] =  s3*s2*c1+c3*s1;
  T_pfix_to_body[2][1] = -s3*c2;
  T_pfix_to_body[2][2] = -s3*s2*s1+c3*c1;

  // initialize torque vector; this is the torque resulting from the 12
  // gravity-points acting on the n vehicle points
  Vector3::initialize(verif_grav_torque);
  for (unsigned int ii_veh = 0; ii_veh < num_veh_pts; ii_veh++) {

    // position of veh point mass is known in veh body frame
    // express point position wrt planet-fixed frame
    double r_veh_pt_position[3];
    Vector3::transform_transpose( T_pfix_to_body,
                                  vehicle_pt[ii_veh].position,
                                  r_veh_pt_position);
    Vector3::incr( veh_position,
                   r_veh_pt_position);

    // Generate forces between this point and all of the gravity-points
    double veh_pt_force[3]; // Force
    Vector3::initialize( veh_pt_force);
    for(unsigned int ii_grav=0; ii_grav < num_pts; ii_grav++) {

      // vector from veh-pt to gravity-pt (in planet-fixed)
      double r_veh_pt_to_grav_pt[3];
      Vector3::diff( gravity_pt[ii_grav].position,
                     r_veh_pt_position,
                     r_veh_pt_to_grav_pt);

      double r_mag = Vector3::vmag( r_veh_pt_to_grav_pt);

      // compute force in planet-fixed
      double G_scale = G * gravity_pt[ii_grav].mass * vehicle_pt[ii_veh].mass /
                       (r_mag*r_mag*r_mag);
      Vector3::scale_incr( r_veh_pt_to_grav_pt,
                           G_scale,
                           veh_pt_force);
    }
    // transform each force vector to body
    double veh_pt_force_bdy[3];
    Vector3::transform(T_pfix_to_body,
                       veh_pt_force,
                       veh_pt_force_bdy);
    Vector3::cross_incr ( vehicle_pt[ii_veh].position,
                          veh_pt_force_bdy,
                          verif_grav_torque); // body-frame, equivalent
                                              // to struc-frame
  }
  Vector3::diff ( verif_grav_torque,
                  grav_torque,
                  torque_error);

  printf("       Sph Harm Model     Pt. Mass Model          Error\n");
  printf("x %20.15f%20.15f%25.15E \n",
          grav_torque[0],
          verif_grav_torque[0],
          torque_error[0]);
  printf("y %20.15f%20.15f%25.15E \n",
          grav_torque[1],
          verif_grav_torque[1],
          torque_error[1]);
  printf("z %20.15f%20.15f%25.15E \n",
          grav_torque[2],
          verif_grav_torque[2],
          torque_error[2]);
}
} // End JEOD namespace
