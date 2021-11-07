#include "AstElements.h"


namespace ferrit {
    SimpleType::SimpleType(Token name) noexcept : m_name(std::move(name)) {
    }

    const Token &SimpleType::name() const noexcept {
        return m_name;
    }

    FunctionType::FunctionType(
        Token errorToken,
        std::vector<DeclaredType> parameters,
        std::unique_ptr<DeclaredType> returnType) noexcept :
        m_errorToken(std::move(errorToken)),
        m_parameters(std::move(parameters)),
        m_returnType(std::move(returnType)) {
    }

    FunctionType::FunctionType(const FunctionType &other) noexcept :
        m_parameters(other.parameters()),
        m_returnType(std::make_unique<DeclaredType>(other.returnType())) {
    }

    FunctionType &FunctionType::operator=(const FunctionType &other) noexcept {
        if (this == &other) return *this;
        m_parameters = other.parameters();
        m_returnType = std::make_unique<DeclaredType>(other.returnType());
        return *this;
    }

    const Token &FunctionType::errorToken() const noexcept {
        return m_errorToken;
    }

    const std::vector<DeclaredType> &FunctionType::parameters() const noexcept {
        return m_parameters;
    }

    const DeclaredType &FunctionType::returnType() const noexcept {
        return *m_returnType;
    }

    DeclaredType::DeclaredType(Token simpleName) noexcept :
        m_data(SimpleType(std::move(simpleName))) {
    }

    DeclaredType::DeclaredType(
        Token errorToken,
        std::vector<DeclaredType> parameters,
        DeclaredType returnType) noexcept :
        m_data(FunctionType(
            std::move(errorToken),
            std::move(parameters),
            std::make_unique<DeclaredType>(std::move(returnType)))) {
    }

    bool DeclaredType::isSimple() const noexcept {
        return std::holds_alternative<SimpleType>(m_data);
    }

    bool DeclaredType::isFunction() const noexcept {
        return std::holds_alternative<FunctionType>(m_data);
    }

    const SimpleType &DeclaredType::simple() const {
        return std::get<SimpleType>(m_data);
    }

    const FunctionType &DeclaredType::function() const {
        return std::get<FunctionType>(m_data);
    }

    const Token &DeclaredType::errorToken() const noexcept {
        if (isSimple()) {
            return simple().name();
        } else {
            return function().errorToken();
        }
    }

    bool DeclaredType::operator==(const DeclaredType &other) const noexcept { // NOLINT(misc-no-recursion)
        if (m_data.index() != other.m_data.index()) {
            return false;
        }
        if (isSimple()) {
            return simple().name().lexeme == other.simple().name().lexeme;
        } else {
            const auto &thisFunc = function();
            const auto &otherFunc = other.function();
            return thisFunc.returnType() == otherFunc.returnType() &&
                thisFunc.parameters() == otherFunc.parameters();
        }
    }

    std::ostream &operator<<(std::ostream &output, const DeclaredType &declaredType) {
        try {
            output << std::format("{}", declaredType);
        } catch (std::format_error &) {
            output.setstate(std::ios::failbit);
        }
        return output;
    }

    Parameter::Parameter(Token name, DeclaredType type) noexcept :
        m_name(std::move(name)), m_type(std::move(type)) {
    }

    const Token &Parameter::name() const noexcept {
        return m_name;
    }

    const DeclaredType &Parameter::type() const noexcept {
        return m_type;
    }

    bool Parameter::operator==(const Parameter &other) const noexcept {
        if (this == &other) return true;
        return name() == other.name() && type() == other.type();
    }
}
