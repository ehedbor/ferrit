#include <iostream>
#include <filesystem>
#include <utility>

#include <cxxopts.hpp>

#include "Lexer.h"
#include "Parser.h"
#include "AstPrinter.h"

namespace fs = std::filesystem;

std::optional<std::vector<ferrit::Token>> lexLine(const std::string &line, bool showOutput) {
    std::vector<ferrit::Token> result;
    bool hadError = false;
    ferrit::Lexer lexer{line};

    while (true) {
        auto maybeToken = lexer.lex();
        if (!maybeToken) {
            hadError = true;
            std::cerr << maybeToken.error() << std::endl;
            continue;
        }
        if (showOutput) {
            std::cout << *maybeToken << std::endl;
        }
        bool shouldBreak = (maybeToken->type == ferrit::TokenType::EndOfFile);
        if (!hadError) result.push_back(std::move(*maybeToken));
        if (shouldBreak) break;
    }

    return hadError ? std::nullopt : std::optional(result);
}

void parseLine(std::vector<ferrit::Token> tokens, bool showOutput) {
    ferrit::Parser parser{std::move(tokens)};

    auto result = parser.parse();
    if (!result) {
        for (auto &err : result.error()) {
            std::cerr << err << "\n";
        }
        std::cerr << std::flush;
        return;
    }

    if (showOutput) {
        ferrit::AstPrinter printer{std::cout};
        printer.print(*result);
    }
}

int main(int argc, char *argv[]) {
    cxxopts::Options options("ferritc", "Compiler and interpreter for the Ferrit programming language.");

    options.add_options()
        ("h,help", "display this message")
        ("l,lexer","show lexer output")
        ("p,parser", "show parser output")
        ("file", "files to compile", cxxopts::value<std::vector<std::string>>());

    options.parse_positional("file");
    options.positional_help("FILE...");

    bool lexerOutput, parserOutput;
    try {
        cxxopts::ParseResult result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        lexerOutput = result.count("lexer") > 1;
        parserOutput = result.count("parser") > 1;
        if (result.count("file")) {
            // TOOD: add file support
            auto files = result["file"].as<std::vector<std::string>>();
            std::cout << "Got a file argument: ";
            for (const auto &file : files) {
                std::cout << "'" << file << "' ";
            }
            std::cout << std::endl;
        }

        while (true) {
            std::cout << "> " << std::flush;
            std::string line;
            std::getline(std::cin, line);
            if (line == "exit") {
                return 0;
            }

            auto tokens = lexLine(line, lexerOutput);
            if (!tokens) continue;

            parseLine(*tokens, parserOutput);
        }
    } catch (const cxxopts::OptionParseException &e) {
        std::cerr << "Error parsing arguments: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 2;
    }
}
