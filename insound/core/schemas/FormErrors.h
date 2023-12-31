/**
 * @file FormErrors.h
 *
 * FormErrors is a struct that indicates which fields have errors and what
 * about those fields is invalid. This is json-stringified and sent to inform
 * the client.
 *
 * Please include FormErrors.json.h to include json stringification bindings.
 */
#pragma once

#include <unordered_map>
#include <vector>

namespace Insound {
    /**
     * Provides descriptive form field errors to the frontend
     */
    class FormErrors
    {
    public:
        std::unordered_map<std::string, std::vector<std::string>> errors;

        /**
         * Append an error to the container
         */
        inline
        void append(std::string_view fieldName, std::string_view errMsg)
        {
            errors[fieldName.data()].emplace_back(errMsg);
        }

        /**
         * Check if the error container is empty
         */
        [[nodiscard]]
        inline
        bool empty() const
        {
            return errors.empty();
        }
    };
}
