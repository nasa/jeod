#include "utils/trick_csv/include/read_trk_csv.hh"
#include <cmath>
#include <iostream>

double dot(const double * v1, const double * v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

double mag2(const double * v)
{
    return dot(v, v);
}

double mag(const double * v)
{
    return std::sqrt(mag2(v));
}

void cross(double * prod, const double * left, const double * right)
{
    prod[0] = left[1] * right[2] - left[2] * right[1];
    prod[1] = left[2] * right[0] - left[0] * right[2];
    prod[2] = left[0] * right[1] - left[1] * right[0];
}

void sProd(double * p, const double s, const double * v)
{
    p[0] = s * v[0];
    p[1] = s * v[1];
    p[2] = s * v[2];
}

void difference(double * diff, const double * minuend, const double * subtrahend)
{
    diff[0] = minuend[0] - subtrahend[0];
    diff[1] = minuend[1] - subtrahend[1];
    diff[2] = minuend[2] - subtrahend[2];
}

// Find the projection (proj and perpendicular (perp) of v relative to unit u
void project(double * proj, double * perp, const double * u, const double * v)
{
    double uDotV = dot(u, v);

    sProd(proj, uDotV, u);
    difference(perp, v, proj);
}

double findAngle(const double * a, const double * b, const double * u)
{
    double x = dot(a, b);
    double temp[3];

    cross(temp, a, b);

    double y = dot(temp, u);
    return std::atan2(y, x);
}

void xFormRecti2Curvi(double * curvi, const double theta, const double * recti)
{
    double c = std::cos(theta);
    double s = std::sin(theta);

    curvi[0] = c * recti[0] + s * recti[2];
    curvi[1] = recti[1];
    curvi[2] = c * recti[2] - s * recti[0];
}

void grabMatrix(double m[3][3], int row, int col, double ** array)
{
    int ii, jj, kk;

    for(ii = 0, kk = col; ii < 3; ++ii)
    {
        for(jj = 0; jj < 3; ++jj, ++kk)
        {
            m[ii][jj] = array[row][kk];
        }
    }
}

void compare(const int numRows, const int numCols, double ** p1, double ** p2, const int o1, const int o2)
{
    double maxErr = 0.0;
    double t;
    int maxErrRow = 0;
    int maxErrCol = 0;

    for(int ii = 0; ii < numRows; ++ii)
    {
        for(int jj = 0; jj < numCols; ++jj)
        {
            if((t = std::abs(p1[ii][jj + o1] - p2[ii][jj + o2])) > maxErr)
            {
                maxErr = t;
                maxErrRow = ii;
                maxErrCol = jj;
            }
        }
    }

    std::cout << "Maximum error = " << maxErr << std::endl;
    std::cout << "Error found at element [" << maxErrRow << "][" << maxErrCol << "]\n";
}

int main(int arg_c, char ** arg_v)
{
    (void)arg_c;
    (void)arg_v;

    ReadTrkCsv vehState("log_VehState.csv");
    ReadTrkCsv relState("log_RelState.csv");

    // First check for internal consistency
    double ** relValues = relState.getValues();
    int numCols = relState.getNumCols();
    int numRows = relState.getNumRows();

    std::cout << "Checking for consistency with existing code\n";
    compare(numRows, 36, relValues, relValues, 1, 37);

    // Check curvilinear calculations

    double temp[3];
    double hA[3];
    double hB[3];
    double normalToPlaneOfA[3];
    double normalToPlaneOfB[3];
    double proj[3];
    double perp[3];
    double * positionA;
    double * velocityA;
    double * positionB;
    double * velocityB;
    double * rectiVelAinB;
    double * rectiVelBinA;
    double theta;
    double thetaDot;
    double tpt[3][3]; // T_parent_this
    auto ** computedCurviPosAinB = new double *[numRows];
    auto ** computedCurviVelAinB = new double *[numRows];
    auto ** computedCurviAngVelAinB = new double *[numRows];
    auto ** computedCurviPosBinA = new double *[numRows];
    auto ** computedCurviVelBinA = new double *[numRows];
    auto ** computedCurviAngVelBinA = new double *[numRows];
    double ** stateValues = vehState.getValues();

    for(int ii = 0; ii < numRows; ++ii)
    {
        positionA = &stateValues[ii][1];
        velocityA = positionA + 3;
        positionB = positionA + 9;
        velocityB = positionB + 3;
        rectiVelAinB = relValues[ii] + 4;
        rectiVelBinA = rectiVelAinB + 18;

        // Compute normals to planes of A and B
        cross(hA, positionA, velocityA);
        sProd(normalToPlaneOfA, 1 / mag(hA), hA);
        cross(hB, positionB, velocityB);
        sProd(normalToPlaneOfB, 1 / mag(hB), hB);

        // Compute curviPos|VelAinB
        computedCurviPosAinB[ii] = new double[3];
        computedCurviVelAinB[ii] = new double[3];
        computedCurviAngVelAinB[ii] = new double[3];
        project(proj, perp, normalToPlaneOfB, positionA);
        theta = findAngle(positionB, perp, normalToPlaneOfB);
        if(relValues[ii][numCols - 2] == 1.0)
        {
            thetaDot = dot(hA, normalToPlaneOfB) / mag2(perp) - mag(hB) / mag2(positionB);
        }
        else if(relValues[ii][numCols - 2] == 0.0)
        {
            thetaDot = 0.0;
        }
        else
        {
            std::cerr << "Illegal value " << relValues[ii][numCols - 2] << " for boolean\n";
            return 1;
        }
        computedCurviPosAinB[ii][0] = mag(positionB) * theta;
        computedCurviPosAinB[ii][1] = -dot(proj, normalToPlaneOfB);
        computedCurviPosAinB[ii][2] = mag(positionB) - mag(perp);
        xFormRecti2Curvi(computedCurviVelAinB[ii], theta, rectiVelAinB);
        grabMatrix(tpt, ii, 10, relValues);
        for(int jj = 0; jj < 3; ++jj)
        {
            computedCurviAngVelAinB[ii][jj] = relValues[ii][jj + 7] + tpt[jj][1] * thetaDot;
        }

        // Compute curviPos|VelBinA
        computedCurviPosBinA[ii] = new double[3];
        computedCurviVelBinA[ii] = new double[3];
        computedCurviAngVelBinA[ii] = new double[3];
        project(proj, perp, normalToPlaneOfA, positionB);
        theta = findAngle(positionA, perp, normalToPlaneOfA);
        if(relValues[ii][numCols - 1] == 1.0)
        {
            thetaDot = dot(hB, normalToPlaneOfA) / mag2(perp) - mag(hA) / mag2(positionA);
        }
        else if(relValues[ii][numCols - 1] == 0.0)
        {
            thetaDot = 0.0;
        }
        else
        {
            std::cerr << "Illegal value " << relValues[ii][numCols - 1] << " for boolean\n";
            return 1;
        }
        computedCurviPosBinA[ii][0] = theta * mag(positionA);
        computedCurviPosBinA[ii][1] = -dot(proj, normalToPlaneOfA);
        computedCurviPosBinA[ii][2] = mag(positionA) - mag(perp);
        xFormRecti2Curvi(computedCurviVelBinA[ii], theta, rectiVelBinA);
        grabMatrix(tpt, ii, 28, relValues);
        for(int jj = 0; jj < 3; ++jj)
        {
            computedCurviAngVelBinA[ii][jj] = relValues[ii][jj + 25] + tpt[jj][1] * thetaDot;
        }
    }

    std::cout << "Checking curvilinear position of A wrt B\n";
    compare(numRows, 3, relValues, computedCurviPosAinB, 73, 0);

    std::cout << "Checking curvilinear velocity of A wrt B\n";
    compare(numRows, 3, relValues, computedCurviVelAinB, 76, 0);

    std::cout << "Checking curvilinear angular velocity of A wrt B\n";
    compare(numRows, 3, relValues, computedCurviAngVelAinB, 79, 0);

    std::cout << "Checking curvilinear position of B in A\n";
    compare(numRows, 3, relValues, computedCurviPosBinA, 91, 0);

    std::cout << "Checking curvilinear velocity of B in A\n";
    compare(numRows, 3, relValues, computedCurviVelBinA, 94, 0);

    std::cout << "Checking curvilinear angular velocity of B wrt A\n";
    compare(numRows, 3, relValues, computedCurviAngVelBinA, 97, 0);

    std::cout << "Checking conversion from circ to rect\n";
    compare(numRows, 18, relValues, relValues, 19, 109);

    // Clean up
    // First release inner allocations
    for(int ii = 0; ii < numRows; ++ii)
    {
        delete[] computedCurviPosAinB[ii];
        delete[] computedCurviVelAinB[ii];
        delete[] computedCurviAngVelAinB[ii];
        delete[] computedCurviPosBinA[ii];
        delete[] computedCurviVelBinA[ii];
        delete[] computedCurviAngVelBinA[ii];
    }

    // Now the outer pointers
    delete[] computedCurviPosAinB;
    delete[] computedCurviVelAinB;
    delete[] computedCurviAngVelAinB;
    delete[] computedCurviPosBinA;
    delete[] computedCurviVelBinA;
    delete[] computedCurviAngVelBinA;

    return 0;
}
