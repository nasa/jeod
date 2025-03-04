/* Generated from environment/gravity/verif/data/mass_data.d */

/*******************************************************************************

Purpose:
  (Point mass system intialization.)

References:
  (((none)))

Assumptions and Limitations:
  ((Fictitious point mass system for verification only - do NOT use for
      actual orbit sim work))

Library dependencies:
  ((mass_data.cc))



*******************************************************************************/

class PointMass;
#define JEOD_FRIEND_CLASS PointMass_grav_verif_default_data

// JEOD includes

// Unsupported includes
#include "../include/mass_data.hh"
#include "../include/grav_verif.hh"

//! Namespace jeod
namespace jeod
{

static constexpr double ptmass_mu = 3.986004415E+14; // gravitational parameter (m3/s2)

void PointMass_grav_verif_default_data::initialize(PointMass * PointMass_ptr)
{
    PointMass_ptr->num_pts = 12;
    PointMass_ptr->deg2rad = 3.14159265358979 / 180.0;
    PointMass_ptr->G = 6.673E-11; // universal gravitational constant  (m3/kg/s2)

    /*
                               mass (kg)
    Position vector:           latitude(deg),         longitude(deg),        radius (meters)
    */

    PointMass_ptr->pt[0].mass = ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[0].posn[0] = 45.0;
    PointMass_ptr->pt[0].posn[1] = 0.0;
    PointMass_ptr->pt[0].posn[2] = 4000.0;

    PointMass_ptr->pt[1].mass = ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[1].posn[0] = 45.0;
    PointMass_ptr->pt[1].posn[1] = 120.0;
    PointMass_ptr->pt[1].posn[2] = 4000.0;

    PointMass_ptr->pt[2].mass = ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[2].posn[0] = 45.0;
    PointMass_ptr->pt[2].posn[1] = 240.0;
    PointMass_ptr->pt[2].posn[2] = 4000.0;

    PointMass_ptr->pt[3].mass = ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[3].posn[0] = -45.0;
    PointMass_ptr->pt[3].posn[1] = 180.0;
    PointMass_ptr->pt[3].posn[2] = 4000.0;

    PointMass_ptr->pt[4].mass = ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[4].posn[0] = -45.0;
    PointMass_ptr->pt[4].posn[1] = 300.0;
    PointMass_ptr->pt[4].posn[2] = 4000.0;

    PointMass_ptr->pt[5].mass = ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[5].posn[0] = -45.0;
    PointMass_ptr->pt[5].posn[1] = 60.0;
    PointMass_ptr->pt[5].posn[2] = 4000.0;

    PointMass_ptr->pt[6].mass = 0.8 * ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[6].posn[0] = 23.0;
    PointMass_ptr->pt[6].posn[1] = 73.0;
    PointMass_ptr->pt[6].posn[2] = 4000.0;

    PointMass_ptr->pt[7].mass = 1.2 * ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[7].posn[0] = -23.0;
    PointMass_ptr->pt[7].posn[1] = 253.0;
    PointMass_ptr->pt[7].posn[2] = (0.8 / 1.2) * 4000.0;

    PointMass_ptr->pt[8].mass = 0.6 * ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[8].posn[0] = 77.0;
    PointMass_ptr->pt[8].posn[1] = 303.0;
    PointMass_ptr->pt[8].posn[2] = 4000.0;

    PointMass_ptr->pt[9].mass = 1.4 * ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[9].posn[0] = -77.0;
    PointMass_ptr->pt[9].posn[1] = 123.0;
    PointMass_ptr->pt[9].posn[2] = (0.6 / 1.4) * 4000.0;

    PointMass_ptr->pt[10].mass = 0.6 * ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[10].posn[0] = 51.0;
    PointMass_ptr->pt[10].posn[1] = 12.0;
    PointMass_ptr->pt[10].posn[2] = 4000.0;

    PointMass_ptr->pt[11].mass = 1.4 * ptmass_mu / PointMass_ptr->G / 12.0;
    PointMass_ptr->pt[11].posn[0] = -51.0;
    PointMass_ptr->pt[11].posn[1] = 192.0;
    PointMass_ptr->pt[11].posn[2] = (0.6 / 1.4) * 4000.0;
}

} // namespace jeod

#undef ptmass_mu
