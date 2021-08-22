#include <iostream>
#include <utility>
#include "Lexer.h"
#include "Parser.h"
#include "AstPrinter.h"

std::optional<std::vector<Token>> lexLine(const std::string &line) {
    std::vector<Token> result;
    bool hadError = false;
    Lexer lexer{line};

    while (true) {
        auto maybeToken = lexer.lex();
        if (maybeToken) {
            std::cout << *maybeToken << std::endl;
            bool shouldBreak = (maybeToken->type == TokenType::EndOfFile);
            if (!hadError) result.push_back(std::move(*maybeToken));
            if (shouldBreak) break;
        } else {
            hadError = true;
            std::cerr << maybeToken.error() << std::endl;
        }
    }

    return hadError ? std::nullopt : std::optional(result);
}

void parseLine(std::vector<Token> tokens) {
    Parser parser{std::move(tokens)};

    auto result = parser.parse();
    if (!result) {
        for (auto &err : result.error()) {
            std::cerr << err << "\n";
        }
        std::cerr << std::flush;
        return;
    }

    AstPrinter printer{std::cout, *result};
    printer.print();
}

int main() {
    while (true) {
        std::cout << "> " << std::flush;
        std::string line;
        std::getline(std::cin, line);
        if (line == "exit") {
            return 0;
        }

        auto tokens = lexLine(line);
        if (!tokens) continue;

        parseLine(*tokens);
    }
}
