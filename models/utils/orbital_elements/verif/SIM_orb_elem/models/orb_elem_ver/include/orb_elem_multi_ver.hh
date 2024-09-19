//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
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
  (Verification of orbital element routines)

REFERENCE:
  (((none)))

ASSUMPTIONS AND LIMITATIONS:
  (none)

LIBRARY DEPENDENCY:
  ((../src/orb_elem_multi_ver.cc))


************************************************************************/

#define DATA_POINTS 5001

// JEOD includes.
#include "utils/orbital_elements/include/orbital_elements.hh"

#ifndef ORB_ELEM_MULTI_VER_H_
#define ORB_ELEM_MULTI_VER_H_

//! Namespace jeod
namespace jeod
{

class OrbElemMultiVer
{
public:
    bool multi_use;                      /* trick_io(*i) trick_units(--) @n
      Flag for multi use. */
    double mu;                           /* trick_io(*i) trick_units(m3/s2) @n
      Gravity param. */
    int data_row;                        /* trick_io(*i) trick_units(--) @n
      Row in data array. */
    double data_vals_in[DATA_POINTS][6]; /* trick_io(*i) trick_units(--) @n
      Array of pos and vel. */
    double data_vals_out[6];             /* trick_io(*o) trick_units(--) @n
      Array of pos and vel. */
    double data_vals_inout[6];           /* trick_io(*o) trick_units(--) @n
      Array of pos and vel. */

    void update(OrbitalElements * orb_elem);
};

} // namespace jeod

#endif
