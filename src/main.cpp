#include <cstdlib>
#include <filesystem>
#include <iostream>

#include <cxxopts.hpp>

#include "AstPrinter.h"
#include "CompileOptions.h"
#include "Lexer.h"
#include "Parser.h"

namespace fs = std::filesystem;


ferrit::CompileOptions parseArguments(int argc, char *argv[]);
auto lexLine(const ferrit::CompileOptions& options, const std::string &line) -> std::optional<std::vector<ferrit::Token>>;
void parseLine(const ferrit::CompileOptions& options, const std::vector<ferrit::Token> &tokens);

int main(int argc, char *argv[]) {
    try {
        ferrit::CompileOptions compileOpts = parseArguments(argc, argv);

        std::cout << "Ferrit Interpreter 0.0.0" << std::endl;
        std::cout << "Available commands: \"exit\", \"quit\"" << std::endl;

        while (true) {
            std::cout << ">>> " << std::flush;
            std::string line;
            std::getline(std::cin, line);
            if (line == "exit" || line == "quit") {
                return EXIT_SUCCESS;
            }

            auto tokens = lexLine(compileOpts, line);
            if (!tokens) continue;

            parseLine(compileOpts, *tokens);
        }
    } catch (const std::exception &e) {
        std::cerr << "error: unhandled exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

std::optional<std::vector<ferrit::Token>> lexLine(const ferrit::CompileOptions& options, const std::string &line) {
    ferrit::Lexer lexer(options);
    auto tokens = lexer.lex(line);
    return tokens;
}

void parseLine(const ferrit::CompileOptions& options, const std::vector<ferrit::Token> &tokens) {
    ferrit::Parser parser(options);

    auto result = parser.parse(tokens);
    if (!result) {
        return;
    }

    if (options.showParserOutput()) {
        ferrit::AstPrinter astPrinter(std::cout);
        for (const ferrit::StatementPtr &decl : result.value()) {
            decl->accept(astPrinter);
        }
    }
}

ferrit::CompileOptions parseArguments(int argc, char *argv[]) {
    try {
        cxxopts::Options options("ferritc", "Compiler and interpreter for the Ferrit programming language.");

        options.add_options()
            ("h,help", "display this shortMessage")
            ("l,lexer","show lexer output")
            ("p,parser", "show parser output")
            ("file", "files to compile", cxxopts::value<std::vector<std::string>>());

        options.parse_positional("file");
        options.positional_help("FILE...");

        cxxopts::ParseResult result = options.parse(argc, argv);
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            std::exit(EXIT_SUCCESS);
        }

        ferrit::CompileOptions compileOpts;
        compileOpts.setShowLexerOutput(result.count("lexer") > 1);
        compileOpts.setShowParserOutput(result.count("parser") > 1);
        if (result.count("file")) {
            // TOOD: add file support
            std::vector<std::string> files = result["file"].as<std::vector<std::string>>();
            std::cout << "Got a file argument: ";
            for (const auto &file: files) {
                std::cout << "'" << file << "' ";
            }
            std::cout << std::endl;
        }

        return compileOpts;
    } catch (const cxxopts::OptionParseException &e) {
        std::cerr << "error while parsing command line arguments: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    } catch (const cxxopts::OptionSpecException &e) {
        std::cerr << "error while defining command line options: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}