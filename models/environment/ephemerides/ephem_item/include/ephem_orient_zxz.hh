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
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemItem
 * @{
 *
 * @file models/environment/ephemerides/ephem_item/include/ephem_orient_zxz.hh
 * Define classes for items represented in some ephemeris model.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/ephem_orient_zxz.cc)
   (../src/ephem_orient.cc)
   (../src/ephem_item.cc))



*******************************************************************************/


#ifndef JEOD_EPHEM_ORIENT_ZXZ_HH
#define JEOD_EPHEM_ORIENT_ZXZ_HH


// System includes

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_interface.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "ephem_orient.hh"


//! Namespace jeod
namespace jeod {

/**
 * The EphemerisZXZOrientation is an EphemerisOrientation subclass that updates
 * orientation based on an Z-X-Z Euler sequence and the time derivatives of
 * this sequence.
 */
class EphemerisZXZOrientation : public EphemerisOrientation {
JEOD_MAKE_SIM_INTERFACES(EphemerisZXZOrientation)

public:

   // Member functions
   // Note: The copy constructor and assignment operator are deleted.

   // Constructor and destructor
   EphemerisZXZOrientation ();
   ~EphemerisZXZOrientation () override;

   // Getters

   // Get the Euler angles
   const double * get_euler_angles () const;
   void get_euler_angles (double * angles) const;

   // Get the Euler angle time derivatives
   const double * get_euler_rates () const;
   void get_euler_rates (double * rates) const;


   // Setters and other non-const methods

   // Update the state
   virtual void update (
      const double * angles, const double * derivs, double time);

   // Propagate the state to some specified dynamic time
   virtual void propagate (double to_time);


protected:

   // Member data

   /**
    * Astronomical (zxz) Euler angles.
    */
   double euler_angle_313[3]; //!< trick_units(rad)

   /**
    * Time derivatives of the zyz Euler angles.
    */
   double euler_rate_313[3];  //!< trick_units(rad/s)


private:

   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   ///
   /// Not implemented.
   EphemerisZXZOrientation (const EphemerisZXZOrientation &);
   ///
   /// Not implemented.
   EphemerisZXZOrientation & operator= (const EphemerisZXZOrientation &);
};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
