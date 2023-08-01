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
 * @addtogroup GenericRNP
 * @{
 *
 * @file models/environment/RNP/GenericRNP/include/RNP_messages.hh
 * Implement RNP_messages
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((This is a complete catalog of all the messages sent by the RNP model.)
   (This is not an exhaustive list of all the things that can go awry.))

Class:
   (None)

Library dependencies:
  ((../src/RNP_messages.cc))

 

*******************************************************************************/

#ifndef JEOD_RNP_MESSAGES_HH
#define JEOD_RNP_MESSAGES_HH

#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod {

/**
 * Describes messages used in the RNP model.
 */
class RNPMessages {

   JEOD_MAKE_SIM_INTERFACES(RNPMessages)

   // Static member data

public:

   // Errors

   /**
    * Indicates an error during initialization.
    */
   static char const * initialization_error;   //!< trick_units(--)

   /**
    * Indicates a mismatch between the requested fidelity and
    * what is available to the model.
    */
   static char const * fidelity_error;   //!< trick_units(--)

   /**
    * Indicates an error during setup of the RNP model.
    */
   static char const * setup_error;   //!< trick_units(--)

   // Warnings

   /**
    * Indicates a problem with the interpolation table commonly
    * found in polar motion implementations.
    */
   static char const * polar_motion_table_warning;   //!< trick_units(--)

private:

   // Class is not instantiable, operator = and copy constructor are
   // hidden from use.
   RNPMessages (void);
   RNPMessages (const RNPMessages& rhs);
   RNPMessages& operator = (const RNPMessages& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
