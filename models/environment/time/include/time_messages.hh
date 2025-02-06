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
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/include/time_messages.hh
 * Define the class TimeMessages, the class that specifies the message IDs
 * used in the Time model.
 */

/*******************************************************************************
Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((This is a complete catalog of all the messages sent by the Time model.)
   (This is not an exhaustive list of all the things that can go awry.))

Library dependencies:
  ((../src/time_messages.cc))


*******************************************************************************/

#ifndef JEOD_TIME_MESSAGES_HH
#define JEOD_TIME_MESSAGES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Specify the message IDs used in the Time model.
 */
class TimeMessages
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TimeMessages)

    // Static member data
public:
    // Errors
    /**
     * Error issued when intialization fails due to some non-obvious cause.  This
     * error is likely due to an algorithm flaw.
     */
    static const char * initialization_error; //!< trick_units(--)

    /**
     * Error issued when system fails because something is not where it should
     * be, or has a value other than its assumed value.
     */
    static const char * memory_error; //!< trick_units(--)

    /**
     * Error issued when user tries to use something that doesn't exist in the
     * model.  This is usually a user error.  Note the distinction between
     * invalid and incomplete
     */
    static const char * invalid_setup_error; //!< trick_units(--)

    /**
     * Error issued when a variable is found with an illegal value.  This is
     * usually a user error, having set some value externally to some
     * unrecognizable value.
     */
    static const char * invalid_data_error; //!< trick_units(--)

    /**
     * Issued when a TimeLinks node is improperly linked
     */
    static const char * invalid_node; //!< trick_units(--)

    /**
     * Error issued when user tries to use something that doesn't exist.n the
     * simulation  This is usually a user error, brought about by not having
     * registered something that is later needed (e.g. not registering a TAI-UTC
     * converter, but specifying that UTC updates from TAI).  Note the
     * distinction between invalid (typically, cannot exist) and incomplete
     * (typically, did not define)
     */
    static const char * incomplete_setup_error; //!< trick_units(--)

    /**
     * Error issued when some value is multiply defined, and the code cannot
     * determine which value to use.  Usually a user-error, from attempting to
     * use too many of the initialization options simultaneously.
     */
    static const char * redundancy_error; //!< trick_units(--)

    /**
     * Informational only.  Issued when there are multiple equivalent methods for
     * doing something, and one method is chosen over another.  Wherever the code
     * fills in data to accommodate, it sends this informational broadcast.
     */
    static const char * duplicate_methods; //!< trick_units(--)

    /**
     * Issued when some functionality relies heavily on the release architecture,
     * and is likely to break with inconsistent extensions.
     */
    static const char * extension_error; //!< trick_units(--)

    // The constructors and assignment operator for this class are deleted.
    TimeMessages() = delete;
    TimeMessages(const TimeMessages &) = delete;
    TimeMessages & operator=(const TimeMessages &) = delete;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
