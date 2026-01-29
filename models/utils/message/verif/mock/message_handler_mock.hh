/*
 * message_handler_mock.hh
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_MESSAGE_VERIF_MOCK_MESSAGE_HANDLER_MOCK_HH_
#define JEOD_MODELS_UTILS_MESSAGE_VERIF_MOCK_MESSAGE_HANDLER_MOCK_HH_

#include "gmock/gmock.h"
#include <cstring>

#include "utils/message/include/message_handler.hh"

class MockMessageHandler : public jeod::MessageHandler
{
public:
    MOCK_CONST_METHOD7(process_message,
                       void(int severity,
                            const char * prefix,
                            const char * file,
                            unsigned int line,
                            const char * msg_code,
                            const char * format,
                            va_list args));
};

#endif /* JEOD_MODELS_UTILS_MESSAGE_VERIF_MOCK_MESSAGE_HANDLER_MOCK_HH_ */
