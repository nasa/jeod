//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
/*******************************************************************************
Purpose: (The DE constants are now hidden.)
*******************************************************************************/

#ifndef JEOD_EPHEM_TEST_CONSTANTS_HH
#define JEOD_EPHEM_TEST_CONSTANTS_HH

#include "environment/ephemerides/de4xx_ephem/include/de4xx_file.hh"


//! Namespace jeod 
namespace jeod {

class DE4xxConstants {
 public:
   DE4xxConstants(const De4xxFileHeader &);

   const double emrat;

   const double vlight;
   const double au;

   const double gm1;
   const double gm2;
   const double gmb;
   const double gm4;
   const double gm5;
   const double gm6;
   const double gm7;
   const double gm8;
   const double gm9;
   const double gms;
};


inline
DE4xxConstants::DE4xxConstants (
   const De4xxFileHeader & hdr)
:
  emrat(hdr.em_mass_ratio),

  vlight(hdr.vlight),
  au(hdr.au),

  gm1(hdr.gmbody[0]),
  gm2(hdr.gmbody[1]),
  gmb(hdr.gmbody[2]),
  gm4(hdr.gmbody[3]),
  gm5(hdr.gmbody[4]),
  gm6(hdr.gmbody[5]),
  gm7(hdr.gmbody[6]),
  gm8(hdr.gmbody[7]),
  gm9(hdr.gmbody[8]),
  gms(hdr.gmbody[10])
{
   // Empty
}

} // End JEOD namespace


#endif
