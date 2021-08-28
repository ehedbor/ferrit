#include <iostream>
#include <filesystem>
#include <utility>

#include <boost/program_options.hpp>

#include "Lexer.h"
#include "Parser.h"
#include "AstPrinter.h"

namespace fs = std::filesystem;
namespace opts = boost::program_options;


std::optional<std::vector<es::Token>> lexLine(const std::string &line, bool showOutput) {
    std::vector<es::Token> result;
    bool hadError = false;
    es::Lexer lexer{line};

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
        bool shouldBreak = (maybeToken->type == es::TokenType::EndOfFile);
        if (!hadError) result.push_back(std::move(*maybeToken));
        if (shouldBreak) break;
    }

    return hadError ? std::nullopt : std::optional(result);
}

void parseLine(std::vector<es::Token> tokens, bool showOutput) {
    es::Parser parser{std::move(tokens)};

    auto result = parser.parse();
    if (!result) {
        for (auto &err : result.error()) {
            std::cerr << err << "\n";
        }
        std::cerr << std::flush;
        return;
    }

    if (showOutput) {
        es::AstPrinter printer{std::cout};
        printer.print(*result);
    }
}

int main(int argc, char *argv[]) {
    opts::positional_options_description posDesc{};
    posDesc.add("input_file", -1);

    opts::options_description desc("Options");
    desc.add_options()
        ("help,h", "display this message")
        ("lexer,l", "show lexer output")
        ("parser,p","show parser output");

    opts::variables_map vars;
    opts::store(opts::command_line_parser(argc, argv).options(desc).positional(posDesc).run(),vars);
    opts::notify(vars);


    if (vars.count("help")) {
        std::string programName = fs::path(argv[0]).filename().string();
        std::cout << "Usage: " << programName << " [options] input_file\n";
        std::cout << desc << std::endl;
        return 0;
    }
    bool lexerOutput = false;
    if (vars.count("lexer")) {
        lexerOutput = true;
    }
    bool parserOutput = false;
    if (vars.count("parser")) {
        parserOutput = true;
    }
    if (vars.count("input-file")) {
        // TOOD: add file support
        std::cout << "Got a file argument: " << vars["input_file"].as<std::string>() << std::endl;
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
}
