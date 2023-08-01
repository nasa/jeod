/*
PURPOSE:
  (Single Rigid Body Gravity Gradient Torque Computation)

REFERENCE:
  (((Hughes, Peter) (Spacecraft Attitude Dynamics)
    (Dover Publications) (2004) (Pages 281-346) (ISBN:0-486-43925-9)))

ASSUMPTIONS AND LIMITATIONS:
  ((Uniform mass distribution)
   (Inertia tensor input is referenced to the composite body frame)
   (Spherical Planet)
   (Position vector is inertial referenced)
   (Symmetric Inertia tensor - only upper diagonal is used)
   (Output torques are structural frame referenced about the composite C.G.))

CLASS:
  (derivative)

LIBRARY DEPENDENCY:
  ((verif_grav_torque.cc))


*/

// System includes
#include <math.h>
#include "environment/gravity/verif/include/point_mass.hh"

// JEOD includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"

//! Namespace jeod
namespace jeod {


void verif_grav_torque(
  const PointMass & ptmass,
  const double sc_mass[3][4],
  const double sc_att[3],
  const double true_inertial_pos[3],
  const double RNP[3][3],
  const double T_pf_bdy[3][3],
  const double sim_time,
  const double grav_accel[3],
  const double grav_torque[3],
  const double grav_pot)

{

  double deg2rad = ptmass.deg2rad;
  /******************************************************************************/
  /* compute acceleration due to point mass system */

  //initialize acceleration vector
  double pt_accel[3];
  Vector3::initialize(pt_accel);

  for (int ii=0; ii<ptmass.num_pts; ii++) {

   // compute cartestian coordinates of point mass
   double r = ptmass.pt[ii].posn[2];
   double x = r * cos(ptmass.pt[ii].posn[0] * deg2rad) *
             cos(ptmass.pt[ii].posn[1] * deg2rad);
   double y = r * cos(ptmass.pt[ii].posn[0] * deg2rad) *
             sin(ptmass.pt[ii].posn[1] * deg2rad);
   double z = r * sin(ptmass.pt[ii].posn[0] * deg2rad);

   // distance between point of interest and point mass
   double del_pos[3];
   del_pos[0] = x - true_inertial_pos[0];
   del_pos[1] = y - true_inertial_pos[1];
   del_pos[2] = z - true_inertial_pos[2];

   r = Vector3::vmag(del_pos);

   // compute and accumulate acceleration by component
   double G_scale = ptmass.G * ptmass.pt[ii].mass/ (r*r*r);

   Vector3::scale_incr( del_pos,
                 G_scale,
                 pt_accel);
  }

  double error[3];
  for (unsigned int ii = 0; ii < 3; ii++) {
   error[ii] = fabs( grav_accel[ii] - pt_accel[ii]);
  }

  /******************************************************************************/
  /* display results */
  printf("Acceleration    Sph. Harm. Model      Pt. Mass Model        Difference\n");
  printf("x m/s2  %25.15f%25.15f%25.15E\n",grav_accel[0],pt_accel[0],error[0]);
  printf("y m/s2  %25.15f%25.15f%25.15E\n",grav_accel[1],pt_accel[1],error[1]);
  printf("z m/s2  %25.15f%25.15f%25.15E\n",grav_accel[2],pt_accel[2],error[2]);

  double rad_per_deg = M_PI / 180.0;

  double s1 = sin(sc_att[0] * rad_per_deg);  // pitch
  double c1 = cos(sc_att[0] * rad_per_deg);
  double s2 = sin(sc_att[1] * rad_per_deg);  // yaw
  double c2 = cos(sc_att[1] * rad_per_deg);
  double s3 = sin(sc_att[2] * rad_per_deg);  // roll
  double c3 = cos(sc_att[2] * rad_per_deg);

  // build transformation matrix (planet-fixed to s/c body)
  // (Euler sequence 2-3-1, Schaub p.680)
  double T_pfix_to_body[3][3];  // body to planet-fixed transformation
  T_pfix_to_body[0][0] =  c2*c1;
  T_pfix_to_body[0][1] =  s2;
  T_pfix_to_body[0][2] = -c2*s1;

  T_pfix_to_body[1][0] = -c3*s2*c1+s3*s1;
  T_pfix_to_body[1][1] =  c3*c2;
  T_pfix_to_body[1][2] =  c3*s2*s1+s3*c1;

  T_pfix_to_body[2][0] =  s3*s2*c1+c3*s1;
  T_pfix_to_body[2][1] = -s3*c2;
  T_pfix_to_body[2][2] = -s3*s2*s1+c3*c1;

  double torque[3];
  Vector3::initialize(torque);
  for (unsigned int ii_sc = 0; ii_sc < 3; ii_sc++) {
    double F[3]; // Force[spacecraft-mass-point-number][component]
    Vector3::initialize(F);
    // position of s/c point mass in s/c body frame
    double r_sc_bdy[3];
    r_sc_bdy[0] = sc_mass[ii_sc][1];
    r_sc_bdy[1] = sc_mass[ii_sc][2];
    r_sc_bdy[2] = sc_mass[ii_sc][3];

    //transform s/c mass position to planet-fixed
    double r_pt_pf[3];
    Vector3::transform_transpose(T_pfix_to_body, r_sc_bdy, r_pt_pf);

    for(int ii=0; ii<ptmass.num_pts; ii++) {
      double r_scpt_pt[3];
      // vector from s/c pt mass to planet pt mass (in planet-fixed)
      r_scpt_pt[0] = ptmass.pt[ii].posn[0] - true_inertial_pos[0] - r_pt_pf[0];
      r_scpt_pt[1] = ptmass.pt[ii].posn[1] - true_inertial_pos[1] - r_pt_pf[1];
      r_scpt_pt[2] = ptmass.pt[ii].posn[2] - true_inertial_pos[2] - r_pt_pf[2];

      double r_mag = Vector3::vmag( r_scpt_pt);

      // compute force in planet-fixed
      double G_scale = ptmass.G * ptmass.pt[ii].mass * sc_mass[ii_sc][0] /
                       (r_mag*r_mag*r_mag);
      Vector3::scale_incr( r_scpt_pt,
                           G_scale,
                           F);
    }
    // transform each force vector to body
    double F_bdy[3]; // likewise but in body-frame
    Vector3::transform(T_pfix_to_body, F, F_bdy);
    Vector3::cross_incr (r_sc_bdy, F_bdy, torque);
  }

  printf("pt mass torque  %25.15E%25.15E%25.15E\n",torque[0],torque[1],torque[2]);
  printf("sph harm torque %25.15E%25.15E%25.15E\n",grav_torque[0],grav_torque[1],grav_torque[2]);
}
} // End JEOD namespace
