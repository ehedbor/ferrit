#pragma once

#include <ostream>
#include <sstream>
#include <format>

namespace ferrit {
    /**
     * The runtime representation of all values in the virtual machine.
     */
    class Value final {
    public:
        /**
         * Initialize the value to 0.0.
         */
        Value() noexcept = default;

        /**
         * Constructs a value containing a real.
         *
         * @param real the value to store
         */
        explicit Value(double real) noexcept;

        /**
         * Checks if this value contains a real.
         *
         * @return true if the value is a real
         */
        [[nodiscard]] bool isReal() const noexcept;

        /**
         * Returns the value's data as a real.
         *
         * @return the value as a real
         * @throws std::invalid_state if the value is not a real
         */
        [[nodiscard]] double asReal() const;

    private:
        double m_real;
    };

    bool operator==(Value left, Value right);

    /**
     * Outputs the value to the ostream. If the value cannot be formatted, the ostream's failbit is set.
     */
    std::ostream &operator<<(std::ostream &output, Value value);
}

template <>
struct std::formatter<ferrit::Value> : std::formatter<std::string> {
    auto format(ferrit::Value value, std::format_context &ctx) {
        if (value.isReal()) {
            std::string formattedString = std::format("{:#}", value.asReal());
            // ensure that the formatted representation does not begin or end with a period
            // (eg prefer '3.0' and '0.67' over '3', '3.' or '.67')
            if (formattedString.starts_with('.')) {
                formattedString.insert(0, ".");
            }
            if (formattedString.ends_with('.')) {
                formattedString.append("0");
            }
            return std::formatter<std::string>::format(formattedString, ctx);
        } else {
            throw std::format_error("unknown Value variant");
        }
    }
};
