/*
 * message_handler_mock.hh
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_MESSAGE_VERIF_MOCK_MESSAGE_HANDLER_MOCK_HH_
#define JEOD_MODELS_UTILS_MESSAGE_VERIF_MOCK_MESSAGE_HANDLER_MOCK_HH_

#include "gmock/gmock.h"

#include "utils/message/include/message_handler.hh"

MATCHER_P(IsStringInArguments, expStr, "")
{
    va_list args2;
    va_copy(args2, arg);
    const char * nextStr = va_arg(args2, const char *);
    while(nextStr != nullptr)
    {
        if(std::strncmp(expStr, nextStr, strlen(expStr)) == 0)
        {
            va_end(args2);
            return true;
        }
        else
        {
            nextStr = va_arg(args2, const char *);
        }
    }
    va_end(args2);
    return false;
}

MATCHER_P(AreVarArgumentsEqual, expStr, "")
{
    va_list args2;
    size_t numExpStrings = expStr.size();
    va_copy(args2, arg);
    for(size_t ii = 0; ii < numExpStrings; ++ii)
    {
        const char * nextStr = va_arg(args2, const char *);
        if(nextStr != nullptr)
        {
            if(expStr[ii].compare(nextStr) != 0)
            {
                va_end(args2);
                return false;
            }
        }
        else
        {
            va_end(args2);
            return false;
        }
    }
    va_end(args2);
    return true;
}

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
