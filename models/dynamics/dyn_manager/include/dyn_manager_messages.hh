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
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/include/dyn_manager_messages.hh
 * Define the class DynManagerMessages, the class that specifies the message IDs
 * used in the DynManager model.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((This is a complete catalog of all messages sent by the DynManager model.)
   (This is not an exhaustive list of all the things that can go awry.))

Library dependencies:
  ((../src/dyn_manager_messages.cc))



*******************************************************************************/

#ifndef JEOD_DYNBODY_MESSAGES_HH
#define JEOD_DYNBODY_MESSAGES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Specifies the message IDs used in the DynManager model.
 */
class DynManagerMessages
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DynManagerMessages)

    // Static member data
public:
    /**
     * Issued when a pointer should be non-NULL but isn't.
     */
    static const char * null_pointer; //!< trick_units(--)

    /**
     * Issued on request to add a pointer to a list a second time.
     */
    static const char * duplicate_entry; //!< trick_units(--)

    /**
     * Issued when a name is invalid -- empty, a duplicate, ...
     */
    static const char * invalid_name; //!< trick_units(--)

    /**
     * Issued when a frame is invalid -- not an integ frame, ...
     */
    static const char * invalid_frame; //!< trick_units(--)

    /**
     * Issued when an object of an unexpected type is encountered.
     */
    static const char * invalid_type; //!< trick_units(--)

    /**
     * Issued when some conditions are inconsistent.
     */
    static const char * inconsistent_setup; //!< trick_units(--)

    /**
     * Error issued when multiple instance of a class that should be a singleton
     * are created or when no such instance exists (but should).
     */
    static const char * singleton_error; //!< trick_units(--)

    /**
     * Error issued when some internal error occurred.
     * These errors should never happen.
     */
    static const char * internal_error; //!< trick_units(--)

    // Member functions
    // This class is not instantiable.
    // The constructors and assignment operator for this class are deleted.
    DynManagerMessages() = delete;
    DynManagerMessages(const DynManagerMessages &) = delete;
    DynManagerMessages & operator=(const DynManagerMessages &) = delete;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
