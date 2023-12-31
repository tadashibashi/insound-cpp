#include "CurlError.h"
#include "curl/curl.h"
#include <stdexcept>

namespace Insound {
    CurlError::CurlError(int curlError) : std::runtime_error(
        sf("Curl Error: code: {}: {}", curlError,
            curl_easy_strerror( CURLcode(curlError) )
        )
    )
    { }
}
