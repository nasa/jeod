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
 * @file models/environment/RNP/RNPJ2000/include/polar_motion_j2000.hh
 * Model the polar motion portion of the RNP routine for the Standard Epoch
 * J2000. This is a form of the IAU-76/FK5 RNP model, as implemented in Jeod
 * 1.52
 */

/*******************************************************************************

Purpose:
  ()

Reference:
    (((G.H. Kaplan)
      (U.S. Naval Observatory Vector Astrometry Subroutines)
      (NOVAS FORTRAN vers F2.0) (1 Nov 98)))

Assumptions and limitations:
  ((The matrix produced is currently an approximation, and not completely
    orthonormal.))

Library dependencies:
  ((../src/polar_motion_j2000.cc))

 

*******************************************************************************/
#ifndef J2000_POLAR_MOTION_HH
#define J2000_POLAR_MOTION_HH

// System includes

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rotation.hh"
#include "utils/sim_interface/include/jeod_class.hh"



//! Namespace jeod
namespace jeod {

/**
 * Implements the polar motion portion of the J2000 RNP.
 */
class PolarMotionJ2000 : public PlanetRotation {

   JEOD_MAKE_SIM_INTERFACES(PolarMotionJ2000)

public: // public data members

   /**
    * Current X Polar coordinate
    */
   double xp; //!< trick_units(rad)
   /**
    * Current Y Polar coordinate
    */
   double yp; //!< trick_units(rad)
   /**
    * X Polar coordinate table
    */
   double* xp_tbl; //!< trick_units(rad)
   /**
    * Y Polar coordinate table
    */
   double* yp_tbl; //!< trick_units(rad)
   /**
    * Independent variable for the XY coordinate table
    */
   double* polar_mjd; //!< trick_units(--)
   /**
    * If true, do no table lookup and use the
    * currently set xp and yp
    */
   bool override_table; //!< trick_units(--)
   /**
    * Size - 1 of xp_tbl, yp_tbl and polar_mjd (last index)
    */
   unsigned int last_table_index; //!< trick_units(count)
   /**
    * Have we warned about being off the table end?
    */
   bool warn_table; //!< trick_units(--)


private: // private data members

public: // public member functions

   PolarMotionJ2000 ();

   ~PolarMotionJ2000 () override;

   // Before update_rotation is called, the "current_time" variable
   // MUST be set to the modified julian date in the UT1 time standard
   void update_rotation () override;

   // How to initialize the PolarMotionJ2000 module with the table information.
   // init must be pointing to a PolarMotionJ2000Init object
   void initialize (PlanetRotationInit* init) override;


private: // private member functions

   // lock away the copy constructor and operator = by making them private
   PolarMotionJ2000& operator = (const PolarMotionJ2000& rhs);
   PolarMotionJ2000 (const PolarMotionJ2000& rhs);
};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
