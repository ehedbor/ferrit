#pragma once

#include <ostream>
#include <sstream>
#include <format>
#include <variant>
#include "RuntimeType.h"

namespace ferrit {
    /**
     * The runtime representation of all values in the virtual machine.
     */
    class Value final {
    public:
        /**
         * Initialize the value to null.
         */
        explicit Value() noexcept;

        /**
         * Initialize a boolean value.
         */
        explicit Value(bool boolean) noexcept;

        /**
         * Initialize an integer value.
         */
        explicit Value(std::int64_t integer) noexcept;

        /**
         * Initialize a real value.
         */
        explicit Value(double real) noexcept;

        /**
         * Checks if this value contains a null pointer.
         */
        [[nodiscard]] bool isNull() const noexcept;

        /**
         * Checks if this value contains a boolean.
         */
        [[nodiscard]] bool isBoolean() const noexcept;

        /**
         * Checks if this value contains an integer.
         */
        [[nodiscard]] bool isInteger() const noexcept;

        /**
         * Checks if this value contains a real.
         */
        [[nodiscard]] bool isReal() const noexcept;

        /**
         * Returns the value's data as a boolean.
         *
         * @return the value as a boolean
         * @throws std::logic_error if the value is not a boolean
         */
        [[nodiscard]] bool asBoolean() const;

        /**
         * Returns the value's data as an integer.
         *
         * @return the value as an integer
         * @throws std::logic_error if the value is not an integer
         */
        [[nodiscard]] std::int64_t asInteger() const;

        /**
         * Returns the value's data as a real.
         *
         * @return the value as a real
         * @throws std::logic_error if the value is not a real
         */
        [[nodiscard]] double asReal() const;

        [[nodiscard]] RuntimeType runtimeType() const;

    private:
        std::variant<std::nullptr_t, bool, std::int64_t, double> m_data;
    };

    bool operator==(const Value &left, const Value &right);

    /**
     * Outputs the value to the ostream. If the value cannot be formatted, the ostream's failbit is set.
     */
    std::ostream &operator<<(std::ostream &output, const Value &value);
}

template <>
struct std::formatter<ferrit::Value> : std::formatter<std::string> {
    auto format(const ferrit::Value &value, std::format_context &ctx) {
        std::string result;
        if (value.isNull()) {
            result = "null";
        } else if (value.isBoolean()) {
            result = value.asBoolean() ? "true" : "false";
        } else if (value.isInteger()) {
            result = std::format("{}", value.asInteger());
        } else if (value.isReal()) {
            result = std::format("{:#}", value.asReal());
            // ensure that the formatted representation does not begin or end with a period
            // (eg prefer '3.0' and '0.67' over '3', '3.' or '.67')
            if (result.starts_with('.')) {
                result.insert(0, ".");
            }
            if (result.ends_with('.')) {
                result.append("0");
            }
        } else {
            throw std::format_error("unknown Value variant");
        }
        return std::formatter<std::string>::format(result, ctx);
    }
};
