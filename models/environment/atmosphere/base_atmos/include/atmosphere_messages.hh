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
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/base_atmos/include/atmosphere_messages.hh
 * Implement atmosphere_messages
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((This is a complete catalog of all the messages sent by the atmosphere
    model.)
   (This is not an exhaustive list of all the things that can go awry.))

Library dependencies:
  ((../src/atmosphere_messages.cc))

*******************************************************************************/

#ifndef JEOD_ATMOSPHERE_MESSAGES_HH
#define JEOD_ATMOSPHERE_MESSAGES_HH

#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod {

/**
 * Describes messages used in the Atmosphere model.
 */
class AtmosphereMessages {

   JEOD_MAKE_SIM_INTERFACES(AtmosphereMessages)

   // Static member data

public:

   // Errors

   /**
    * Indicates an error during initialization.
    */
   static char const * initialization_error;   //!< trick_units(--)

   /**
    * Indicates an error during use of the generic framework.
    */
   static char const * framework_error;   //!< trick_units(--)

   // Warnings

   /**
    * Indicates a warning associated with the generic framework.
    */
   static char const * framework_warning; //!< trick_units(--)

   /**
    * Indicates a warning associated with numerical values
    */
   static char const * numerical_warning; //!< trick_units(--)

private:

   // Class is not instantiable, operator = and copy constructor are
   // hidden from use.
   AtmosphereMessages (void);
   AtmosphereMessages (const AtmosphereMessages& rhs);
   AtmosphereMessages& operator = (const AtmosphereMessages& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
