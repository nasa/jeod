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
 * @file models/environment/RNP/RNPJ2000/include/nutation_j2000.hh
 * Model the nutation portion of the RNP routine for the Standard Epoch J2000
 * This is form of the IAU-76/FK5 RNP model, as implemented in Jeod 1.52
 */

/*****************************************************************************

Purpose:
  ()

Reference:
      (((Mulcihy, David D. and Bond, Victor R.)
        (The RNP Routine for the STandard Epoch J2000)
        (NASA:JSC-24574) (NASA Engineering Directorate, Navigation, Control,
         and Aeronautics Division) (September 1990) (--)))

Assumptions and limitations:
  ((Earth specific)(must be initialized))

Library dependencies:
  ((../src/nutation_j2000.cc))

 

*****************************************************************************/


#ifndef NUTATION_J2000_HH
#define NUTATION_J2000_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "environment/RNP/GenericRNP/include/planet_rotation.hh"



//! Namespace jeod
namespace jeod {

// Models the nutation component of the J2000 model
/**
 * Implements the nutation portion of the J2000 RNP.
 */
class NutationJ2000 : public PlanetRotation {

   JEOD_MAKE_SIM_INTERFACES(NutationJ2000)

public: // public data members


   /**
    * The number of coefficients in the 9 arrays
    * directly following this decleration
    */
   unsigned int num_coeffs; //!< trick_units(count)

   /**
    * The coefficients to calculate mean anomaly of the moon
    */
   double* L_coeffs; //!< trick_units(--)
   /**
    * The coefficients to calculate mean anomaly of the sun
    */
   double* M_coeffs; //!< trick_units(--)
   /**
    * The coefficients to calculate mean argument of lattitude of the moon
    */
   double* F_coeffs; //!< trick_units(--)
   /**
    * The coefficients to calculate mean elongation from the sun
    */
   double* D_coeffs; //!< trick_units(--)
   /**
    * The coefficients to calculate the right
    * ascension of the ascending node of the mean
    * mean lunar orbit
    */
   double* omega_coeffs; //!< trick_units(--)

   /**
    * Zero order (in time) coefficients for
    * calculating the nutation in longitude
    */
   double* long_coeffs; //!< trick_units(--)
   /**
    * The first order (in time) coefficients for
    * calculating the nutation in longitude
    */
   double* long_t_coeffs; //!< trick_units(--)
   /**
    * Zero order coefficients for calculating the
    * nutation in obliquity
    */
   double* obliq_coeffs; //!< trick_units(--)
   /**
    * First order coefficnets for calculating the
    * nutation in obliquity
    */
   double* obliq_t_coeffs; //!< trick_units(--)

   /**
    * The nutation in longitude from the last
    * call of update
    */
   double nutation_in_longitude; //!< trick_units(--)
   /**
    * The nutation in obliquity from the last
    * call of update
    */
   double nutation_in_obliquity; //!< trick_units(--)

   /**
    * Last calculated mean anomaly of the moon
    */
   double L; //!< trick_units(--)
   /**
    * Last calculated mean anomaly of the sun
    */
   double M; //!< trick_units(--)
   /**
    * Last calculated mean argument of latitude of the moon
    */
   double F; //!< trick_units(--)
   /**
    * Last calculated mean elongation from the sun
    */
   double D; //!< trick_units(--)
   /**
    * Last calculated ascension of the ascending node of the
    * mean lunar orbit
    */
   double omega; //!< trick_units(--)

   /**
    * Last calculated mean obliqutiy of the ecliptic
    */
   double epsilon_bar; //!< trick_units(--)

   /**
    * Last calculated equations of the equinox
    */
   double equa_of_equi; //!< trick_units(--)

private: // private data members

public: // public member functions

   NutationJ2000 ();

   ~NutationJ2000 () override;

   // Specific implemtation of update_rotation, from the polymorphic pure
   // virtual base class PlanetRotation. Before this is called, the
   // current_time parameter must be set to julian centures since J2000,
   // in the terrestrial time format.
   void update_rotation () override;

   // Initialize the various coefficients needed for the calculation of
   // nutation. init must be of type NutationJ2000Init or an exec_terminate
   // will occur
   void initialize (PlanetRotationInit* init) override;

private: // private member functions

   // lock away the copy constructor and operator = by making them private

   NutationJ2000& operator = (const NutationJ2000& rhs);
   NutationJ2000 (const NutationJ2000& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
