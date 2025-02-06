
// Local definitions
#define NUM_CASES 40
#define STRING_LENGTH 1024
#define JEOD_CLASS_FRIEND_CLASS Tester

// System includes
#include <cstdlib>
#include<iostream>
#include <vector>

// JEOD includes
#include "environment/gravity/include/gravity_manager.hh"
#include "environment/gravity/include/spherical_harmonics_gravity_source.hh"
#include "environment/gravity/include/spherical_harmonics_gravity_controls.hh"
#include "environment/planet/include/planet.hh"
#include "environment/planet/data/include/earth.hh"
#include "environment/gravity/data/include/earth_GGM02C.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "test_harness/include/test_sim_interface.hh"
#include "test_harness/include/cmdline_parser.hh"

using namespace std;

static bool verbose=false;

typedef char my_string[STRING_LENGTH];

typedef struct {
int caseNum;
int degree;
int order;
int perturbOnly;
int gradActive;
double pos[3];
double accel[3];
double dgdx[3][3];
double pot[1];
} TestRec;

namespace jeod {

class Tester {
public :
TestRec testRecs[NUM_CASES];

int readTestRecs();
void calcGradientNumerically(
SphericalHarmonicsGravityControls &gc,
double pos[3],
double dhdx[3][3]);
void gradDiff(
double dgdx1[3][3],
double dgdx2[3][3],
double &maxGrad,
double &maxGradDiff);
void doTest(int testNum,
SphericalHarmonicsGravityControls &gc,
Planet &earth,
double &rInEarthRadii,
double &maxAccel,
double &maxGrad,
double &maxAccelDiff,
double &maxGradDiff,
double &maxPotDiff);
}; // end class Tester

}

using namespace jeod;

int
main (
int argc,
char * argv[])
{
  vector<EphemerisRefFrame *> frameVector;
  Tester tester;
  TestSimInterface test_sim_interface;
  CmdlineParser cmdline_parser;
     Planet_earth_default_data earth_planet_init;
     SphericalHarmonicsGravitySource_earth_GGM02C_default_data  earth_gravity_init;
  GravityManager gravModel;
  SphericalHarmonicsGravitySource gravBody;
  SphericalHarmonicsGravityControls gravControls;
  Planet planet;
  double potentialTolerance;
  double accelTolerance;
  double gradTolerance;
  double globalMaxPotDiff=0.0;
  double globalMaxAccelDiff=0.0;
  double globalMaxGradDiff=0.0;

  // Grab stuff from command line
  cmdline_parser.add_double ("PotentialTolerance", 0, &potentialTolerance);
  cmdline_parser.add_double("AccelerationTolerance", 0, &accelTolerance);
  cmdline_parser.add_double("GradientTolerance", 0, &gradTolerance);
  cmdline_parser.add_switch("Verbose", &verbose);
  cmdline_parser.parse(argc, argv);

  gravBody.tide_free = true;
  gravControls.active = true;
         earth_planet_init.initialize( &planet );
         earth_gravity_init.initialize( &gravBody );
  gravBody.initialize_body();

  planet.grav_source = &gravBody;
  gravBody.inertial = &planet.inertial;
  gravBody.pfix = &planet.pfix;
  planet.initialize();
   gravControls.source_name = gravBody.name;
  gravModel.add_grav_source(gravBody);
  frameVector.push_back(&planet.inertial);
  gravBody.initialize_state (frameVector, gravModel);
  gravControls.initialize_control(gravModel);

  if (tester.readTestRecs() != 0) {
    cerr<<"Did not read data file successfully."<<endl;
    return 1;
  } // end if

  cout<<"Running "<<NUM_CASES<<" cases"<<endl;
  cout<<"Potential tolerance = "<<potentialTolerance<<endl;
  cout<<"Acceleration tolerance = "<<accelTolerance<<endl;
  cout<<"Gradient tolerance = "<<gradTolerance<<endl;
  if (verbose) {
    cout<<"Calculating finite difference approximations to gravity gradient"<<endl;
    cout<<"as a consistency check on tabular and computed values."<<endl;
  } // end if

  for (int caseNum = 0;caseNum < NUM_CASES;++caseNum) {
    double rInEarthRadii;
    double maxAccel;
    double maxGrad;
    double maxAccelDiff;
    double maxGradDiff;
    double maxPotDiff;

    if (verbose)
    cout<<"Case number "<<(1+caseNum)<<":"<<endl;
    tester.doTest(caseNum,
    gravControls,
    planet,
    rInEarthRadii,
    maxAccel,
    maxGrad,
    maxAccelDiff,
    maxGradDiff,
    maxPotDiff);

    if (verbose) {
      cout<<"R in earth radii = "<<rInEarthRadii<<endl;
      cout<<"Maximum acceleration = "<<maxAccel<<endl;
      cout<<"Maximum gradient = "<<maxGrad<<endl;
      cout << "Maximum accelleration difference = "<<maxAccelDiff << endl;
      cout << "Maximum gradient difference = "<<maxGradDiff<<endl;
      cout << "Potential difference = "<<maxPotDiff<<endl;
    } // end if

    if (maxPotDiff > globalMaxPotDiff)
    globalMaxPotDiff = maxPotDiff;
    if (maxAccelDiff > globalMaxAccelDiff)
    globalMaxAccelDiff = maxAccelDiff;
    if (maxGradDiff > globalMaxGradDiff)
    globalMaxGradDiff = maxGradDiff;
  } // end for i

  int rv=0;

  if (globalMaxPotDiff > potentialTolerance) {
    cout<<"Failed potential tolerance "<<globalMaxPotDiff<<endl;
    rv = 1;
  } // end if

  if (globalMaxAccelDiff > accelTolerance) {
    cout<<"Failed acceleration tolerance "<<globalMaxAccelDiff<<endl;
    rv = 1;
  } // end if

  if (globalMaxGradDiff > gradTolerance) {
    cout<<"Failed gradient tolerance "<<globalMaxGradDiff<<endl;
    rv = 1;
  } // end if

  return rv;
} // end main

int Tester::readTestRecs() {
FILE *f=fopen("data/verif_out.txt", "r");

if (f == NULL) {
cerr << "Data file not found" << endl;
return 1;
} // end if

my_string str;
int caseNum=0;

while (fgets(str, STRING_LENGTH, f) != NULL) {
TestRec *tr_ptr=&testRecs[caseNum++];
int numFields=
sscanf(str, "%d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
&tr_ptr->caseNum, &tr_ptr->degree, &tr_ptr->order,
&tr_ptr->perturbOnly, &tr_ptr->gradActive,
&tr_ptr->pos[0], &tr_ptr->pos[1], &tr_ptr->pos[2],
&tr_ptr->pot[0],
&tr_ptr->accel[0], &tr_ptr->accel[1], &tr_ptr->accel[2],
&tr_ptr->dgdx[0][0], &tr_ptr->dgdx[0][1], &tr_ptr->dgdx[0][2],
&tr_ptr->dgdx[1][1], &tr_ptr->dgdx[1][2],
&tr_ptr->dgdx[2][2]);

if (numFields < 16) {
cerr << "Bad input at line "<< caseNum << endl;
fclose (f);
return 1;
} // end if
} // end while
fclose (f);

return 0;
} // end readTestRecs

void Tester::calcGradientNumerically (
SphericalHarmonicsGravityControls &gc,
double pos[3],
double result[3][3])
{
double gPlus[3][3];
double gMinus[3][3];
double tempPlus[3];
double tempMinus[3];
double accelPlus[3];
double accelMinus[3];
double gradDummy[3][3];
double potDummy[1];

for (int j = 0; j < 3; j++) {
for (int i = 0; i < 3; i++) {
tempPlus[i] = tempMinus[i] = pos[i];
if (i == j) {
tempPlus[i]+=0.5;
tempMinus[i]-=0.5;
} // end if
} // end for i
gc.gravitation(tempPlus, 0, accelPlus, gradDummy, potDummy);
gc.gravitation(tempMinus, 0, accelMinus, gradDummy, potDummy);
for (int i = 0; i < 3; i++) {
gPlus[i][j] = accelPlus[i];
gMinus[i][j] = accelMinus[i];
} // end for i
} // end for j
Matrix3x3::subtract(gPlus, gMinus, result);
} // end calcGradientNumerically

void Tester::gradDiff(
double dgdx1[3][3],
double dgdx2[3][3],
double&maxGrad,
double &maxGradDiff)
{
maxGrad = maxGradDiff = 0.0;
for (int i = 0; i < 3; i++)
for (int j = i; j < 3; j++) {
double t = fabs(dgdx1[i][j]);
if (t > maxGrad)
maxGrad = t;

t = fabs(dgdx1[i][j]-dgdx2[i][j]);
if (t > maxGradDiff)
maxGradDiff = t;
} // end for j
} // end gradDiff

void Tester::doTest(int caseNum,
SphericalHarmonicsGravityControls &gc,
Planet& earth,
double &rInEarthRadii,
double &maxAccel,
double &maxGrad,
double &maxAccelDiff,
double &maxGradDiff,
double &maxPotDiff)
{
TestRec *trptr = &testRecs[caseNum];
double accel[3];
double dgdx[3][3];
double numericalDgdx[3][3];
double pot[1];
double diffNumerical2Table;
double diffNumerical2Computed;
double dummy;

gc.gradient = trptr->gradActive;
gc.perturbing_only = trptr->perturbOnly;
gc.set_grad_degree_order(0,0);
gc.set_degree_order(0,0);
gc.set_degree(trptr->degree);
gc.set_grad_degree(trptr->degree);
gc.set_order(trptr->order);
gc.set_grad_order(trptr->order);
gc.gravitation(trptr->pos, 0, accel, dgdx, pot);

if (verbose) {
cout<<"Perturbing only = "<<gc.perturbing_only<<endl;
cout<<"Gradient = "<<gc.gradient<<endl;
cout<<"Degree = "<<gc.get_degree()<<endl;
cout<<"Order = "<<gc.get_order()<<endl;
} // end if

rInEarthRadii = maxAccel = maxAccelDiff = maxPotDiff = 0.0;

for (int i = 0; i < 3; i++) {
double t=fabs(trptr->accel[i]);

if (t > maxAccel)
maxAccel = t;

t=fabs(accel[i]-trptr->accel[i]);
if (t > maxAccelDiff)
maxAccelDiff = t;

rInEarthRadii+=(trptr->pos[i]*trptr->pos[i]);

} // end for i
rInEarthRadii = sqrt(rInEarthRadii)/earth.r_eq;
gradDiff(trptr->dgdx, dgdx, maxGrad, maxGradDiff);
maxPotDiff = fabs(pot[0]-trptr->pot[0]);

if (verbose) {
calcGradientNumerically(gc, trptr->pos, numericalDgdx);
gradDiff(numericalDgdx, trptr->dgdx, dummy, diffNumerical2Table);
gradDiff(numericalDgdx, dgdx, dummy, diffNumerical2Computed);
if (diffNumerical2Table < diffNumerical2Computed)
cout << "Table is closer to finite difference."<<endl;
else
cout << "Computed is closer to finite difference."<<endl;

cout<<"Difference with table = "<<diffNumerical2Table<<endl;
cout << "Difference with computed = "<<diffNumerical2Computed<<endl;
} // end if
} // end doTest
