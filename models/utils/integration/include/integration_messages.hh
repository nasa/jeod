//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 *
 * @file models/utils/integration/include/integration_messages.hh
 * Define the class IntegrationMessages, the class that specifies the
 * message IDs used in the integration model.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((This is a complete catalog of all the messages sent by the integration
    model.)
   (This is not an exhaustive list of all the things that can go awry.))

Library dependencies:
  ((../src/integration_messages.cc))



*******************************************************************************/

#ifndef JEOD_INTEGRATION_MESSAGES_HH
#define JEOD_INTEGRATION_MESSAGES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Declares messages associated with the integration test model.
 */
class IntegrationMessages
{
    JEOD_MAKE_SIM_INTERFACES(jeod, IntegrationMessages)

public:
    // Static member data

    /**
     * Issued when some user input is invalid.
     */
    static const char * unsupported_option; //!< trick_units(--)

    /**
     * Issued when an item is somehow invalid; a duplicate entry for example.
     */
    static const char * invalid_item; //!< trick_units(--)

    /**
     * Issued when the JEOD programmer messed up.
     */
    static const char * internal_error; //!< trick_units(--)

    /**
     * Issued when a non-JEOD programmer messed up.
     */
    static const char * invalid_request; //!< trick_units(--)

    /**
     * Issued in non-error messages.
     */
    static const char * information; //!< trick_units(--)

    IntegrationMessages() = delete;
    IntegrationMessages(const IntegrationMessages &) = delete;
    IntegrationMessages & operator=(const IntegrationMessages &) = delete;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
