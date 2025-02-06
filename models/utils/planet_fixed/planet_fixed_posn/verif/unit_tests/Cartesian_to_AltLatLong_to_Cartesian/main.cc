// System includes
#include<iostream>
#include <stdlib.h>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/gravity/include/spherical_harmonics_gravity_source.hh"
#include "environment/planet/include/planet.hh"
#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_posn.hh"
#include "utils/math/include/vector3.hh"
#include "test_harness/include/test_sim_interface.hh"
#include "test_harness/include/cmdline_parser.hh"
using namespace std;
using namespace jeod;

int testPFP (PlanetFixedPosition &pfp, int numTests, double tolerance);
void doTest(PlanetFixedPosition &p, double v[3], double &e1, double &e2);
void randomVector
(PlanetFixedPosition &p, double r1, double r2, double rv[3]);
int getEllipticalParams(Planet &p, double r, double z, double &f, double &h,
int maxIters=10);

int
main (
int argc,
char * argv[])
{
TestSimInterface test_sim_interface;
CmdlineParser cmdline_parser;
SphericalHarmonicsGravitySource gravBody;
Planet planet;
PlanetFixedPosition planetFixedPosition;
int numCases;
double tolerance;


// Grab stuff from command line
cmdline_parser.add_int ("NumCases", 0, &numCases);
cmdline_parser.add_double("Tolerance", 0, &tolerance);
cmdline_parser.add_double("Eq", 6378.137, &planet.r_eq);
cmdline_parser.add_double("FlatInv", 298.257223563, &planet.flat_inv);
cmdline_parser.parse(argc, argv);
planet.name = "Nine from Outer Space";
// planet.flat_inv = 298.257223563;
// planet.r_eq /* {km} */ = 6378.137;
gravBody.name = planet.name;
planet.grav_source = &gravBody;
planet.initialize();
planetFixedPosition.initialize(&planet);
cout<<"Running "<<numCases<<" cases"<<endl;
cout<<"at tolerance "<<tolerance<<endl;
return testPFP(planetFixedPosition, numCases, tolerance);
} // end main

int testPFP (PlanetFixedPosition &pfp, int numCases, double tol) {
double esph, eell, worstSphere=0.0, worstEllipse=0.0, worst;
double testVector[3];

for (int i = 0; i < numCases; i++) {
randomVector(pfp, 0.9, 5.0, testVector);
doTest(pfp, testVector, esph, eell);
if(esph > worstSphere) worstSphere = esph;
if(eell > worstEllipse) worstEllipse = eell;
} // end for i

worst = fmax(worstSphere, worstEllipse);
cout<<"Worst spherical error = "<<worstSphere<<endl;
cout<<"Worst elliptical error = "<<worstEllipse<<endl;
if (worst > tol)
return 1;

return 0;
} // end testPFP

 void doTest(PlanetFixedPosition &pfp, double carts[3],
double &eSphere, double &eEllipse) {
AltLatLongState spherical;
AltLatLongState elliptical;
double error[3];

pfp.update_from_cart(carts);
spherical.altitude = pfp.sphere_coords.altitude;
elliptical.altitude = pfp.ellip_coords.altitude;
spherical.latitude = pfp.sphere_coords.latitude;
elliptical.latitude = pfp.ellip_coords.latitude;
spherical.longitude = pfp.sphere_coords.longitude;
elliptical.longitude = pfp.ellip_coords.longitude;

pfp.update_from_spher(spherical);
for (int i = 0; i < 3; i++) {
error[i] = pfp.cart_coords[i]-carts[i];
} // end for i
eSphere = Vector3::vmag(error);

pfp.update_from_ellip(elliptical);
for (int i = 0; i < 3; i++) {
error[i] = pfp.cart_coords[i]-carts[i];
} // end for i
eEllipse = Vector3::vmag(error);
} // end doTest

void randomVector
(PlanetFixedPosition &pfp, double lowR, double highR, double rv[3]) {
double r=pfp.planet->r_eq*(lowR+(highR-lowR)*(double)rand()/(double)RAND_MAX);

for (int i = 0; i < 3; i++) {
rv[i] = 2.0*(double)rand()/(double)RAND_MAX-1.0;
} // end for i
Vector3::normalize(rv);
Vector3::scale(r, rv);
} // end randomVector
