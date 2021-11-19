#pragma once

#include <iostream>
#include <optional>
#include <string>

#include "Lexer.h"
#include "Parser.h"
#include "AstPrinter.h"


namespace ferrit {
    /**
     * Compile options for the interpreter.
     */
    struct InterpretOptions final {
        bool printAst{false};         ///< Print the AST after every execution.
        bool silent{false};           ///< Do not print compile errors to the errors stream.
        bool plain{false};            ///< Do not use color codes in output.
        bool traceVm{false};          ///< Trace virtual machine execution
    };

    /**
     * Indicates the result of an interpret.
     */
    enum class InterpretResult {
        Ok,
        ParseError,
        CompileError,
        RuntimeError,
    };

    /**
     * The main Ferrit interpreter.
     */
    class Interpreter {
    public:
        /**
         * Constructs a new interpreter.
         */
        Interpreter() noexcept = default;

        /**
         * Constructs a new interpreter with the given options,
         * using the standard C++ streams for user programs.
         *
         * @param options compile options
         */
        Interpreter(InterpretOptions options) noexcept; // NOLINT(google-explicit-constructor)

        /**
         * Constructs a new interpreter with the given options,
         * using the given streams for user programs.
         *
         * @param options compile options
         * @param output the ostream used for standard output
         * @param errors the ostream used for standard errors
         * @param input the istream used for standard input
         */
        Interpreter(InterpretOptions options, std::ostream &output, std::ostream &errors, std::istream &input) noexcept;

        virtual ~Interpreter() noexcept;

        /**
         * Execute the given segment of code.
         *
         * @param code the code to execute
         * @return if there were any errors in the interpret process
         */
        virtual InterpretResult run(const std::string &code) = 0;

    protected:
        std::optional<std::vector<StatementPtr>> parse(const std::string &code);

    protected:
        InterpretOptions m_options{};
        std::ostream *m_output{&std::cout};
        std::ostream *m_errors{&std::cerr};
        std::istream *m_input{&std::cin};
        std::shared_ptr<ErrorReporter> m_errorReporter{
            std::make_shared<ErrorReporter>(*m_errors, m_options.plain)};

    private:
        Lexer m_lexer{m_options.silent ? nullptr : m_errorReporter};
        Parser m_parser{m_options.silent ? nullptr : m_errorReporter};
        AstPrinter m_astPrinter{*m_output};
    };
}
