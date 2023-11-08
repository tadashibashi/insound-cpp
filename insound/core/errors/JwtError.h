#pragma once

#include <stdexcept>
#include <string_view>

namespace Insound::Jwt {



    /**
     * JWT error class. Reports errors during conversion and verification.
     */
    class Error : public std::runtime_error {
    public:
        enum class Code {
            // No errors
            OK,
            // Bad token string argument
            BadTokenFormat,
            // Runtime error due to faulty base64 or JSON conversion
            ConversionFailure,
            // Token converted, but failed to pass verification, due to expiration,
            // wrong algorithm, missing claims, claim type/value mismatch, etc.
            VerificationFailure,
        };

        static std::string_view errorString(Code code);

        explicit Error(Code ec) :
            std::runtime_error(errorString(ec).data()), ec(ec) { }

        explicit Error(Code ec, std::string_view detail) :
            std::runtime_error(f("{}: {}", errorString(ec).data(), detail)),
            ec(ec) { }

        Code code() const { return ec; }
    private:
        Code ec;
    };



}
