#include <cstdio>
#include <cmath>
#include "../include/orientation.hh"
#include "../include/io_utils.hh"
using namespace std;

int main (int arg_c, char ** arg_v) {
double eigen_angle_in_deg;
double eigen_axis[3];

if (arg_c != 5) {
cerr << "Usage: " << arg_v[0] << " <ANGLE> <EIGENAXIS[0-2]>" <<endl;
return -1;
} // end if

if (sscanf(arg_v[1], "%lf", &eigen_angle_in_deg) != 1) {
cerr << arg_v[1] << " is not a number"<<endl;
return -2;
} // end if
for (int i = 0; i < 3; ++i)
if (sscanf(arg_v[i+2], "%lf", &eigen_axis[i]) != 1) {
cerr << arg_v[i+2] << " is not a number" << endl;
return -3;
} // end if

double eigen_angle = DEG2RAD(eigen_angle_in_deg);
double vmag = sqrt(eigen_axis[0]*eigen_axis[0]+eigen_axis[1]*eigen_axis[1]+
eigen_axis[2]*eigen_axis[2]);

if (vmag == 0.0) {
cerr <<"Eigen axis cannot be the zero vector"<<endl;
return -4;
} // end if
for (int i = 0; i < 3; ++i)
eigen_axis[i]/=vmag;
cout << "Eigen angle = "<< eigen_angle<<endl;
cout <<"Eigen axis = ";
PRINT_VEC(eigen_axis);

double mat[3][3];

Orientation::compute_matrix_from_eigen_rotation(
eigen_angle, eigen_axis, mat);

cout << "Transformation matrix:"<<endl;

PRINT_MAT(mat);

double new_eigen_angle;
double new_eigen_axis[3];

Orientation::compute_eigen_rotation_from_matrix(
mat, &new_eigen_angle, new_eigen_axis);

double new_eigen_angle_in_deg = RAD2DEG(new_eigen_angle);

cout << "New eigen angle = "<< new_eigen_angle << endl;
cout << "New eigen axis = ";
PRINT_VEC(new_eigen_axis);

for (int i = 0; i < 3; ++i)
new_eigen_axis[i]*=vmag;

cout << "Unnormalized new eigen axis = ";
PRINT_VEC(new_eigen_axis);
cout<< "New eigen angle in degrees = "<<new_eigen_angle_in_deg<<endl;

return 0;
} // end main
