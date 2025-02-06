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
 * @addtogroup EphemInterface
 * @{
 *
 * @file models/environment/ephemerides/ephem_interface/include/ephem_messages.hh
 * Define the class EphemeridesMessages, the class that specifies the message
 * IDs used in the JEOD ephemerides model.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/ephem_messages.cc))



*******************************************************************************/


#ifndef JEOD_EPHEMERIDES_MESSAGES_HH
#define JEOD_EPHEMERIDES_MESSAGES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"



//! Namespace jeod
namespace jeod {

/**
 * Specifies the message IDs used in the Ephemerides model.
 *
 * \par Assumptions and Limitations
 *  - This is a complete catalog of the messages sent by the ephemerides model.
 *  - This is not an exhaustive list of all the things that can go awry.
 */
class EphemeridesMessages {
JEOD_MAKE_SIM_INTERFACES(EphemeridesMessages)

public:

   // Static member data

   /**
    * Error issued when the ephemeris model configuration is inconsistent.
    */
   static char const * inconsistent_setup; //!< trick_units(--)

   /**
    * Error issued when the ephemeris file cannot be opened for input.
    */
   static char const * file_error; //!< trick_units(--)

   /**
    * Error issued for machine architectures that do not conform to
    * the architecture assumptions:
    * - char    = 8 bits
    * - int32_t = 4 bytes (32 bits)
    * - double  = 8 bytes (64 bits)
    */
   static char const * unsupported_architecture; //!< trick_units(--)

   /**
    * Error issued when the ephemeris file appears to be garbage.
    */
   static char const * garbage_file; //!< trick_units(--)

   /**
    * Error issued when the ephemeris file does not contain data for
    * the requested time.
    */
   static char const * time_not_in_range; //!< trick_units(--)

   /**
    * Error issued when the ephemeris file does not contain data for
    * the requested item.
    */
   static char const * item_not_in_file; //!< trick_units(--)

   /**
    * Issued when a pointer should be non-NULL but isn't.
    */
   static char const * null_pointer; //!< trick_units(--)

   /**
    * Issued on request to add a pointer to a list a second time.
    */
   static char const * duplicate_entry; //!< trick_units(--)

   /**
    * Issued when a name is invalid -- empty, a duplicate, ...
    */
   static char const * invalid_name; //!< trick_units(--)

   /**
    * Issued when something other than a name is invalid.
    */
   static char const * invalid_item; //!< trick_units(--)

   /**
    * Issued when the ephemeris manager is rejecting add_ephemeris calls.
    */
   static char const * single_ephem_mode; //!< trick_units(--)

   /**
    * Issued when some internal error occurred.
    * These errors should never happen.
    */
   static char const * internal_error; //!< trick_units(--)


   /**
    * Used to send a message about a non-error condition.
    */
   static char const * debug; //!< trick_units(--)


private:
   // Member functions

   // This class is not instantiable.
   // The constructors and assignment operator for this class are declared
   // private and are not implemented.

   ///
   /// Not implemented.
   EphemeridesMessages (void);
   ///
   /// Not implemented.
   EphemeridesMessages (const EphemeridesMessages &);
   ///
   /// Not implemented.
   EphemeridesMessages & operator= (const EphemeridesMessages &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
