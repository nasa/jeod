#include <iostream>
#include <cmath>
#include "read_trk_csv.hh"
#include "utils/quaternion/include/quat.hh"


double dot (const double *v1, const double *v2) {
  return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
}

double mag2 (const double *v) {return dot (v, v);}

double mag (const double *v) { return std::sqrt(mag2(v));}

void cross (double *prod, const double *left, const double *right) {
  prod[0] = left[1]*right[2]-left[2]*right[1];
  prod[1] = left[2]*right[0] - left[0]*right[2];
  prod[2] = left[0]*right[1] - left[1]*right[0];
}

void sProd(double *p, const double s, const double *v) {
  p[0] = s*v[0];
  p[1] = s*v[1];
  p[2] = s*v[2];
}

void vec_copy(double *r, const double *v) {
  r[0] = v[0];
  r[1] = v[1];
  r[2] = v[2];
}

void difference (
        double *diff, const double *minuend, const double *subtrahend) {
  diff[0] = minuend[0]-subtrahend[0];
  diff[1] = minuend[1]-subtrahend[1];
  diff[2] = minuend[2]-subtrahend[2];
}

void normalize(double *v) {

   double vmag = mag(v);
   sProd(v, 1.0 / vmag, v);
}


void compare (const int numRows, const int numCols, double **p1,
          double **p2, const int o1, const int o2) {
  double maxErr=0.0;
  double t;
  int maxErrRow=0;
  int maxErrCol=0;

  for (int ii = 0; ii < numRows; ++ii) {
    for (int jj = 0; jj < numCols; ++jj) {
      if ((t=std::abs(p1[ii][jj+o1]-p2[ii][jj+o2])) > maxErr) {
        maxErr = t;
        maxErrRow = ii;
        maxErrCol = jj;
      }
    }
  }

  std::cout << "Maximum error = " << maxErr << std::endl;
  std::cout << "Error found at element [" << maxErrRow << "][" << maxErrCol << "]\n";
}


int main (int arg_c, char ** arg_v) {
  ReadTrkCsv vehState("log_VehState.csv");
  ReadTrkCsv lvlhState("log_LvlhState.csv");

  // First check for internal consistency
  double **stateValues = vehState.getValues();
  double **lvlhValues = lvlhState.getValues();
  int numCols = lvlhState.getNumCols();
  int numRows = lvlhState.getNumRows();

  std::cout << "Checking for consistency with existing code\n";
  compare (numRows, 36, lvlhValues, lvlhValues, 1, 37);

  double *position;
  double *velocity;
  double *angvel;

// Check lvlh frame orientation calculations
  Quaternion att_quat;
  double angmom[3];
  double hmag;
  double rmag;
  double rmagsq;
  double wmag;

  double **computedLvlhPos = new double *[numRows];
  double **computedLvlhVel = new double *[numRows];
  double **computedLvlhAngVel = new double *[numRows];
  double ***computedLvlhOrient = new double **[numRows];
  Quaternion **computedLvlhQuat = new Quaternion *[numRows];
 
std::cout << "numRows: " << numRows << std::endl;

  for (int ii = 0; ii < numRows; ++ii) {
    position = &stateValues[ii][1];
    velocity = position + 3;
    att_quat.scalar = lvlhValues[ii][7];
    att_quat.vector[0] = lvlhValues[ii][8];
    att_quat.vector[1] = lvlhValues[ii][9];
    att_quat.vector[2] = lvlhValues[ii][10];
    angvel = &lvlhValues[ii][11];

    // Compute values based on vehicle position/velocity
    cross (angmom, position, velocity);
    hmag = mag(angmom);
    rmagsq = mag2(position);
    rmag = std::sqrt(rmagsq);
    wmag = hmag / rmagsq;

    // Pos/vel same as host vehicle
    computedLvlhPos[ii] = new double [3];
    computedLvlhVel[ii] = new double [3];
    vec_copy(computedLvlhPos[ii], position);
    vec_copy(computedLvlhVel[ii], velocity);

    // Create orientation matrix and corresponding quaternion
    computedLvlhOrient[ii] = new double *[3];
    for (int jj = 0; jj < 3; ++jj) {
      computedLvlhOrient[ii][jj] = new double [3];
    }
    sProd(computedLvlhOrient[ii][2], -1.0 / rmag, position);
    sProd(computedLvlhOrient[ii][1], -1.0 / hmag, angmom);
    cross(computedLvlhOrient[ii][0], computedLvlhOrient[ii][1], computedLvlhOrient[ii][2]);
    normalize(computedLvlhOrient[ii][0]);

    computedLvlhQuat[ii] = new Quaternion(*computedLvlhOrient[ii][0]);

    // Ang velocity is only about y-axis by definition
    computedLvlhAngVel[ii] = new double [3];
    computedLvlhAngVel[ii][0] = 0.0;
    computedLvlhAngVel[ii][1] = -wmag;
    computedLvlhAngVel[ii][2] = 0.0;

std::cout << "Given angular velocity: " << angvel[0] << ", " << angvel[1] << ", " << angvel[2]  << std::endl;
std::cout << "Calculated: " << computedLvlhAngVel[ii][0] << ", " << computedLvlhAngVel[ii][1] << ", " << computedLvlhAngVel[ii][2]  << std::endl;
  }


  // Now check results
  std::cout << "Checking position of LVLH frame\n";
  compare (numRows, 3, lvlhValues, computedLvlhPos, 1, 0);

  std::cout << "Checking velocity of LVLH frame\n";
  compare (numRows, 3, lvlhValues, computedLvlhVel, 4, 0);

  std::cout << "Checking angular velocity of LVLH frame\n";
  compare (numRows, 3, lvlhValues, computedLvlhAngVel, 11, 0);

  // std::cout << "Checking first row of orientation matrix of LVLH frame\n";
  // compare (numRows, 3, lvlhValues, computedLvlhOrient[0], 7, 0);
  // std::cout << "Checking second row of orientation matrix of LVLH frame\n";
  // compare (numRows, 3, lvlhValues, &computedLvlhOrient[0][1], 10, 0);
  // std::cout << "Checking third row of orientation matrix of LVLH frame\n";
  // compare (numRows, 3, lvlhValues, &computedLvlhOrient[0][2], 13, 0);


  // Clean up
  // First release inner allocations
  for (int ii = 0; ii < numRows; ++ii) {
    delete [] computedLvlhPos[ii];
    delete [] computedLvlhVel[ii];
    delete [] computedLvlhAngVel[ii];
    delete [] computedLvlhQuat[ii];
  }

  // Now the outer pointers
  delete [] computedLvlhPos;
  delete [] computedLvlhVel;
  delete [] computedLvlhAngVel;
  delete [] computedLvlhQuat;

return 0;
}
