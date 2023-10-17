#pragma once
#include <fsbank.h>
#include <fsbank_errors.h>
#include <iostream>
#include "platform.h"

#define FSB_CHECK(result) do { \
        const FSBANK_RESULT res = (result); \
        const char *message = res ? FSBank_ErrorString(res) : nullptr; \
        if ( (res) != FSBANK_OK ) { \
            std::cerr << "FSBank Error: " << message << '\n'; \
            return message; \
        } \
    } while(0)

namespace Insound {
    using Result = const char *;

    const Result OK = nullptr;
}