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
/********************************* TRICK HEADER ***************************

PURPOSE:
   (Verification of J2000 RNP, this structure stores transposes of the
    individual matrices stored in the nutation, precession, rotation,
    and polar motion objects in the RNP.)

REFERENCE:
   (((none)))

ASSUMPTIONS AND LIMITATIONS:
   (none)

Library dependencies:
   ((../src/RNP_J2000_log_trans.cc)
    (../src/matrix_multiply.cc))



************************************************************************/

#ifndef RNP_J2000_LOG_TRANS_H
#define RNP_J2000_LOG_TRANS_H

#include "environment/RNP/RNPJ2000/include/rnp_j2000.hh"

// Define macros
#include "utils/math/include/macro_def.hh"

void matrix_multiply(double matrix[3][3], double to_multiply[3], double result[3]);

//! Namespace jeod
namespace jeod
{

class J2000RNPLogging
{
public:
    J2000RNPLogging() = default;
    ~J2000RNPLogging() = default;
    J2000RNPLogging(const J2000RNPLogging & rhs);
    J2000RNPLogging & operator=(const J2000RNPLogging & rhs);

    void copy_rotation_transposes(PlanetRotation & precession,
                                  PlanetRotation & nutation,
                                  PlanetRotation & rotation,
                                  PlanetRotation & polar_motion);

    double prec_trans[3][3]{IDENTITY_3X3};
    double nut_trans[3][3]{IDENTITY_3X3};
    double pm_trans[3][3]{IDENTITY_3X3};
    double rot_trans[3][3]{IDENTITY_3X3};
};

} // namespace jeod

#include "utils/math/include/macro_undef.hh"

#endif
