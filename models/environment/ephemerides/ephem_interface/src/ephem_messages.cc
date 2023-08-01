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
 * @file models/environment/ephemerides/ephem_interface/src/ephem_messages.cc
 * Implement the class EphemeridesMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((ephem_messages.cc))



*******************************************************************************/


// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/ephem_messages.hh"

//! Namespace jeod
namespace jeod {

#define MAKE_EPHEMERIDES_MESSAGE_CODE(id) \
   JEOD_MAKE_MESSAGE_CODE(EphemeridesMessages, "environment/ephemerides/", id)


// Define EphemeridesMessages static member data
MAKE_EPHEMERIDES_MESSAGE_CODE (inconsistent_setup);
MAKE_EPHEMERIDES_MESSAGE_CODE (file_error);
MAKE_EPHEMERIDES_MESSAGE_CODE (unsupported_architecture);
MAKE_EPHEMERIDES_MESSAGE_CODE (garbage_file);
MAKE_EPHEMERIDES_MESSAGE_CODE (time_not_in_range);
MAKE_EPHEMERIDES_MESSAGE_CODE (item_not_in_file);
MAKE_EPHEMERIDES_MESSAGE_CODE (null_pointer);
MAKE_EPHEMERIDES_MESSAGE_CODE (duplicate_entry);
MAKE_EPHEMERIDES_MESSAGE_CODE (invalid_name);
MAKE_EPHEMERIDES_MESSAGE_CODE (invalid_item);
MAKE_EPHEMERIDES_MESSAGE_CODE (single_ephem_mode);
MAKE_EPHEMERIDES_MESSAGE_CODE (internal_error);
MAKE_EPHEMERIDES_MESSAGE_CODE (debug);


#undef MAKE_EPHEMERIDES_MESSAGE_CODE

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
