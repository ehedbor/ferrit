#include <iostream>
#include "Lexer.h"

void lexLine(const std::string &line) {
    Lexer lexer{line};
    while (true) {
        auto token = lexer.lex();
        if (token) {
            std::cout << token.value() << std::endl;
        } else {
            std::cerr << token.error() << std::endl;
        }

        if (token->type == TokenType::EndOfFile) break;
    }
}

int main() {
    while (true) {
        std::cout << "> " << std::flush;
        std::string line;
        std::getline(std::cin, line);
        if (line == "exit")
            return 0;
        else
            lexLine(line);
    }
}
