#include "Interpreter.h"


namespace ferrit {
    Interpreter::Interpreter(InterpretOptions options) noexcept :
        m_options(options) {
    }

    Interpreter::Interpreter(InterpretOptions options, std::ostream &output, std::ostream &errors, std::istream &input) noexcept :
        m_options(options), m_output(&output), m_errors(&errors), m_input(&input) {
    }

    Interpreter::~Interpreter() noexcept = default;

    std::optional<std::vector<StatementPtr>> Interpreter::parse(const std::string &code) {
        auto tokens = m_lexer.lex(code);
        if (!tokens.has_value()) {
            return {};
        }

        auto ast = m_parser.parse(tokens.value());
        if (!ast.has_value()) {
            return {};
        }

        if (m_options.printAst) {
            m_astPrinter.print(ast.value());
        }

        return ast;
    }
}