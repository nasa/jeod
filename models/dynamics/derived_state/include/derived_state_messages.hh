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
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/include/derived_state_messages.hh
 * Define the class DerivedStateMessages, the class that specifies the message
 * IDs used in the DerivedState model.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((This is a complete catalog of all messages sent by the DerivedState model.)
   (This is not an exhaustive list of all the things that can go awry.))

Library dependencies:
  ((../src/derived_state_messages.cc))



*******************************************************************************/

#ifndef JEOD_DERIVED_STATE_MESSAGES_HH
#define JEOD_DERIVED_STATE_MESSAGES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The class that specifies the message IDs used in the DerivedState model.
 */
class DerivedStateMessages
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DerivedStateMessages)

    // Static member data

public:
    /**
     * Issued when performing an action results in an error return from the
     * method performing the action.
     */
    static const char * fatal_error; //!< trick_units(--)

    /**
     * Issued when a simple type (e.g. an enum) has an illegal value.
     */
    static const char * illegal_value; //!< trick_units(--)

    /**
     * Issued when a name is invalid (NULL, empty, or does not name an object
     * of the specified type).
     */
    static const char * invalid_name; //!< trick_units(--)

    /**
     * Issued when a pointer points to an object of the wrong type.
     */
    static const char * invalid_object; //!< trick_units(--)

    /**
     * Error issued when a pointer is required but was not provided.
     */
    static const char * null_pointer; //!< trick_units(--)

    /**
     * Debug message issued to trace DerivedState actions.
     */
    static const char * trace; //!< trick_units(--)

    /**
     * Fatal message when a divide by zero is encountered
     */
    static const char * divide_by_zero; //!< trick_units(--)

    // Member functions

    // This class is not instantiable.
    // The constructors and assignment operator for this class are deleted.
    DerivedStateMessages() = delete;
    DerivedStateMessages(const DerivedStateMessages &) = delete;
    DerivedStateMessages & operator=(const DerivedStateMessages &) = delete;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
