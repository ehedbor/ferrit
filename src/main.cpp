#include <filesystem>
#include <iostream>

#include <cxxopts.hpp>
#include <utility>

#include "AstPrinter.h"
#include "Lexer.h"
#include "Parser.h"

namespace fs = std::filesystem;


std::optional<std::vector<ferrit::Token>> lexLine(
    std::shared_ptr<ferrit::ErrorReporter> errorReporter,
    const std::string &line
) {
    ferrit::Lexer lexer{std::move(errorReporter)};
    auto tokens = lexer.lex(line);
    return tokens;
}

std::optional<std::vector<ferrit::StatementPtr>> parseLine(
    const std::vector<ferrit::Token> &tokens,
    std::shared_ptr<ferrit::ErrorReporter> errorReporter,
    bool printAst
) {
    ferrit::Parser parser{std::move(errorReporter)};

    auto result = parser.parse(tokens);
    if (!result) return {};

    if (printAst) {
        ferrit::AstPrinter astPrinter(std::cout);
        astPrinter.print(result.value());
    }
    return result;
}

cxxopts::Options makeOptions() {
    cxxopts::Options options("ferritc", "Compiler and interpreter for the Ferrit programming language.");

    options.add_options()
        ("h,help", "display this message")
        ("print-ast", "show program AST", cxxopts::value<bool>()->default_value("false"))
        ("silent", "disable error logging", cxxopts::value<bool>()->default_value("false"))
        ("plain", "disable colors in output", cxxopts::value<bool>()->default_value("false"))
        ("trace-vm", "trace virtual machine execution", cxxopts::value<bool>()->default_value("false"))
        ("file", "files to compile", cxxopts::value<std::vector<std::string>>());

    options.parse_positional("file");
    options.positional_help("FILE...");

    return options;
}

int main(int argc, char *argv[]) {
    try {
        cxxopts::Options optionsSpec = makeOptions();
        cxxopts::ParseResult flags = optionsSpec.parse(argc, argv);
        if (flags.count("help")) {
            std::cout << optionsSpec.help() << std::endl;
            return 0;
        }

        std::shared_ptr<ferrit::ErrorReporter> errorReporter{nullptr};
        if (!flags["silent"].as<bool>()) {
            errorReporter = std::make_shared<ferrit::ErrorReporter>(std::cout, flags["plain"].as<bool>());
        }

        std::cout << "Ferrit Interpreter 0.0.0" << std::endl;
        std::cout << R"(Available commands: "exit", "quit")" << std::endl;

        while (true) {
            std::cout << ">>> ";
            std::string line;
            std::getline(std::cin, line);
            if (line == "exit" || line == "quit") {
                return 0;
            }

            auto tokens = lexLine(errorReporter, line);
            if (!tokens) continue;

            parseLine(*tokens, errorReporter, flags["print-ast"].as<bool>());
        }
    } catch (const std::exception &e) {
        std::cerr << "internal compiler error: " << e.what() << std::endl;
        throw;
    }
}