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
 * @addtogroup Utils
 * @{
 * @addtogroup Message
 * @{
 *
 * @file models/utils/message/include/suppressed_code_message_handler.hh
 * Define the class SuppressedCodeMessageHandler, which adds the capability
 * to suppress messages by their message code.
 * This capability cannot be a part of the base MessageHandler class because
 * that base class needs to stand on its own.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/suppressed_code_message_handler.cc))



*******************************************************************************/

#ifndef JEOD_SUPPRESSED_CODE_MESSAGE_HANDLER_HH
#define JEOD_SUPPRESSED_CODE_MESSAGE_HANDLER_HH

// JEOD includes
#include "utils/container/include/primitive_set.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "message_handler.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Adds the capability to suppress messages by their message code to the
   base MessageHandler class.
 */
class SuppressedCodeMessageHandler : public MessageHandler
{
    JEOD_MAKE_SIM_INTERFACES(jeod, SuppressedCodeMessageHandler)

public:
    // Member functions
    SuppressedCodeMessageHandler() = default;
    ~SuppressedCodeMessageHandler() override = default;
    SuppressedCodeMessageHandler(const SuppressedCodeMessageHandler &) = delete;
    SuppressedCodeMessageHandler & operator=(const SuppressedCodeMessageHandler &) = delete;

protected:
    // Member functions

    // register_contents() registers the checkpointable contents.
    void register_contents() override;

    // deregister_contents() deregisters the checkpointable contents.
    void deregister_contents() override;

    /**
     * Add a message code to the set of messages that are to be suppressed.
     * \param[in] msg_code Message code to be suppressed
     */
    void process_add_suppressed_code(const char * msg_code) override
    {
        suppressed_codes.insert(msg_code);
    }

    /**
     * Delete a message code from the set of suppressed message codes.
     * \param[in] msg_code Message code to be unsuppressed
     */
    void process_delete_suppressed_code(const char * msg_code) override
    {
        suppressed_codes.erase(msg_code);
    }

    /**
     * Clear the set of messages that are to be suppressed.
     */
    virtual void process_clear_suppressed_code()
    {
        suppressed_codes.clear();
    }

    /**
     * Determine whether output for a message is to be printed.
     * @return True => print message
     * \param[in] severity Severity level
     * \param[in] msg_code Message code
     */
    bool message_is_to_be_printed(int severity, const char * msg_code) const
    {
        return (severity <= 0) || ((static_cast<unsigned int>(severity) <= suppression_level) &&
                                   (suppressed_codes.find(msg_code) == suppressed_codes.end()));
    }

    // Member data

    /**
     * The set of message code that are to be suppressed.
     */
    JeodPrimitiveSet<std::string>::type suppressed_codes; //!< trick_io(**)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
