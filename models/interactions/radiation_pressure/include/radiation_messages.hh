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
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/include/radiation_messages.hh
 * Define the class RadiationMessages, the class that specifies the message IDs
 * used in the Radiation model.
 */


/*******************************************************************************

Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((This is a complete catalog of all the messages sent by the Radiation model.)
   (This is not an exhaustive list of all the things that can go awry.))

Library dependencies:
  ((../src/radiation_messages.cc))



*******************************************************************************/


#ifndef JEOD_RADIATION_MESSAGES_HH
#define JEOD_RADIATION_MESSAGES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"



//! Namespace jeod
namespace jeod {

/**
 * Provides error messages
 */
class RadiationMessages {


 JEOD_MAKE_SIM_INTERFACES(RadiationMessages)


 // Static member data
 public:
   // Errors
   /**
    * The setup was not fully defined.
    */
   static char const * incomplete_setup_error; //!< trick_units(--)
   /**
    * The setup is invalid
    */
   static char const * invalid_setup_error; //!< trick_units(--)
   /**
    * Something internal went wrong in the setup methods without obvious user
    * error.
    */
   static char const * operational_setup_error; //!< trick_units(--)
   /**
    * A function was called before it was fully implemented.
    */
   static char const * invalid_function_call; //!< trick_units(--)

   /**
    * Something went horribly wrong.
    */
   static char const * unknown_numerical_error; //!< trick_units(--)

 // The constructors and assignment operator for this class are declared
 // private and are not implemented.
 private:
   RadiationMessages (void);
   RadiationMessages (const RadiationMessages &);
   RadiationMessages & operator= (const RadiationMessages &);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
