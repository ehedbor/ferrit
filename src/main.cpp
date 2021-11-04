#include <cstdlib>
#include <filesystem>
#include <iostream>

#include <cxxopts.hpp>
#include <utility>

#include "AstPrinter.h"
#include "Compiler.h"
#include "CompileOptions.h"
#include "Lexer.h"
#include "Parser.h"

namespace fs = std::filesystem;


std::optional<std::vector<ferrit::Token>> lexLine(
    std::shared_ptr<const ferrit::CompileOptions> options,
    std::shared_ptr<ferrit::ErrorReporter> errorReporter,
    const std::string &line
) {
    ferrit::Lexer lexer(std::move(options), std::move(errorReporter));
    auto tokens = lexer.lex(line);
    return tokens;
}

std::optional<std::vector<ferrit::StatementPtr>> parseLine(
    const std::shared_ptr<const ferrit::CompileOptions> &options,
    std::shared_ptr<ferrit::ErrorReporter> errorReporter,
    const std::vector<ferrit::Token> &tokens
) {
    ferrit::Parser parser(options, std::move(errorReporter));

    auto result = parser.parse(tokens);
    if (!result) return {};

    if (options->printAst()) {
        ferrit::AstPrinter astPrinter(std::cout);
        astPrinter.print(result.value());
    }
    return result;
}

std::shared_ptr<ferrit::CompileOptions> parseArguments(int argc, char *argv[]) {
    cxxopts::Options options("ferritc", "Compiler and interpreter for the Ferrit programming language.");

    options.add_options()
        ("h,help", "display this shortMessage")
        ("print-ast", "show program AST")
        ("silent", "disable error logging")
        ("plain", "disable colors in output")
        ("vm-trace", "trace virtual machine execution")
        ("file", "files to compile", cxxopts::value<std::vector<std::string>>());

    options.parse_positional("file");
    options.positional_help("FILE...");

    cxxopts::ParseResult result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    auto compileOpts = std::make_shared<ferrit::CompileOptions>();
    (*compileOpts)
        .setPrintAst(result.count("print-ast") > 1)
        .setSilentErrors(result.count("silent-errors") > 1)
        .setPlainOutput(result.count("plain-output") > 1)
        .setVmTraceExecution(result.count("vm-trace") > 1);

    if (result.count("file")) {
        // TODO: add file support
        std::vector<std::string> files = result["file"].as<std::vector<std::string>>();
        std::cout << "Got a file argument: ";
        for (const auto &file: files) {
            std::cout << "'" << file << "' ";
        }
        std::cout << std::endl;
    }

    return compileOpts;
}

int main(int argc, char *argv[]) {
    try {
        auto compileOpts = parseArguments(argc, argv);
        auto errorReporter = std::make_shared<ferrit::ErrorReporter>(compileOpts, std::cout);

        std::cout << "Ferrit Interpreter 0.0.0" << std::endl;
        std::cout << R"(Available commands: "exit", "quit")" << std::endl;

        while (true) {
            std::cout << ">>> " << std::flush;
            std::string line;
            std::getline(std::cin, line);
            if (line == "exit" || line == "quit") {
                return EXIT_SUCCESS;
            }

            auto tokens = lexLine(compileOpts, errorReporter, line);
            if (!tokens) continue;

            parseLine(compileOpts, errorReporter, *tokens);
        }
    } catch (const std::exception &e) {
        std::cerr << "internal compiler error: " << e.what() << std::endl;
        throw;
    }
}