//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/* *

PURPOSE:
   (Logging for the aerodynamics verification sim)

LIBRARY DEPENDENCY:
   ((../src/aero_logging.cc))

ASSUMPTIONS:

   ((ONLY TO BE USED WITH THE AERO VERIFICATION.
     THERE IS NO ERROR CHECKING))



*/

#ifndef JEOD_AERODYNAMIC_LOGGING_HH
#define JEOD_AERODYNAMIC_LOGGING_HH

#include "interactions/aerodynamics/include/flat_plate_aero_facet.hh"

//! Namespace jeod
namespace jeod
{

class Plate
{
public:
    double force_n;      /* trick_units(N) @n
            Normal force */
    double force_t;      /* trick_units(N) @n
            Tangent force */
    double angle_attack; /* trick_units(rad) @n
       Angle of attack */
    double cos_alpha;    /* trick_units(--) @n
          Cosine of angle of attack */
    double sin_alpha;    /* trick_units(--) @n
          Sine of angle of attack */
    double force[3];     /* trick_units(N) @n
           Force on plate */
    double torque[3];    /* trick_units(N) @n
          Torque on plate */
    FlatPlateAeroFacet * plate;
};

class AeroSurface;

class AeroLogging
{
public:
    double accel[3];      /* trick_units(m/s2) @n
            Acceleration in structural frame */
    double accel_x_total; /* trick_units(m/s2) @n
       Total acceleration in struct X */
    double accel_y_total; /* trick_units(m/s2) @n
       Total acceleration in struct Y */
    double accel_mag;     /* trick_units(m/s2) @n
           Total acceleration magnitude */
    double accel_ratio;   /* trick_units(--) @n
         accel_x_total/accel_y_total ratio */

    Plate plates[6];             /* trick_units(--) @n
                   The plates for parameters */
    unsigned int size_of_plates; /* trick_units(count) @n
       Size of plates */

    void aero_logging_initialize(AeroSurface & surface);

    void aero_logging(double inert_vel[3], double mass, double force[3]);
};

} // namespace jeod

#endif
