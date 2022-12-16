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
/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup RNPJ2000
 * @{
 *
 * @file models/environment/RNP/RNPJ2000/include/precession_j2000.hh
 * Model the precession for the RNP routine for the Standard Epoch J2000
 * This is a form of the IAU-76/FK5 RNP model, as implemented in Jeod 1.52
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((Mulcihy, David D. and Bond, Victor R.)
    (The RNP Routine for the STandard Epoch J2000)
    (NASA:JSC-24574) (NASA Engineering Directorate, Navigation, Control,
     and Aeronautics Division) (September 1990) (--)))

Assumptions and limitations:
  ((Earth specific))

Library dependencies:
  ((precession_j2000.o))

 

*******************************************************************************/
#ifndef PRECESSION_J2000_HH
#define PRECESSION_J2000_HH

// System includes

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rotation.hh"
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

// Class to model the precession portion of the J2000 RNP model
/**
 * Implements the precession portion of the J2000 RNP.
 */
class PrecessionJ2000 : public PlanetRotation {

   JEOD_MAKE_SIM_INTERFACES(PrecessionJ2000)

public: // public data members

private: // private data members

public: // public member functions

   PrecessionJ2000 ();

   virtual ~PrecessionJ2000 ();

   // precession J2000 specific implementation of update_rotation, inherited
   // from PlanetRotation. Before this is called, the
   // current_time parameter must be set to julian centures since J2000,
   // in the terrestrial time format.
   virtual void update_rotation ();

private: // private member functions

   // lock away the copy constructor and operator =
   PrecessionJ2000& operator = (const PrecessionJ2000& rhs);
   PrecessionJ2000 (const PrecessionJ2000& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
