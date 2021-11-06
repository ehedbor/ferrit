#pragma once

#include <ostream>
#include <format>

namespace ferrit {
    /**
     * The runtime representation of all values in the virtual machine.
     */
    class Value {
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

        friend bool operator==(Value lhs, Value rhs) = default;

        friend std::ostream &operator<<(std::ostream &output, Value value);

    private:
        double m_real;
    };

}

template <>
struct std::formatter<ferrit::Value> : public std::formatter<std::string> {
public:
    auto format(ferrit::Value value, std::format_context &context) { //NOLINT(readability-convert-member-functions-to-static)
        return std::formatter<std::string>::format(
            std::format("{:#}", value.asReal()), context);
    }
};