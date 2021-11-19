#include "Interpreter.h"
#include "vm/BytecodeInterpreter.h"

#include <cxxopts.hpp>

#include <fstream>
#include <iostream>


cxxopts::Options makeOptions() {
    cxxopts::Options options("ferritc", "Compiler and interpreter for the Ferrit programming language.");

    options.add_options()
        ("h,help", "display this message")
        ("print-ast", "show program AST", cxxopts::value<bool>()->default_value("false"))
        ("silent", "disable error logging", cxxopts::value<bool>()->default_value("false"))
        ("plain", "disable colors in output", cxxopts::value<bool>()->default_value("false"))
        ("trace-vm", "trace virtual machine execution", cxxopts::value<bool>()->default_value("false"))
        ("file", "file to interpret", cxxopts::value<std::string>());

    options.parse_positional("file");
    options.positional_help("FILE");

    return options;
}

int runRepl(ferrit::Interpreter &interpreter) {
    std::cout << "Ferrit Interpreter 0.0.0" << std::endl;
    std::cout << R"(Available commands: "exit", "quit")" << std::endl;

    while (true) {
        std::cout << ">>> " << std::flush;
        std::string line;
        std::getline(std::cin, line);
        if (line == "exit" || line == "quit") {
            return 0;
        }

        std::string code{line};
        while (!line.empty()) {
            std::cout << "... " << std::flush;
            std::getline(std::cin, line);
            code.append(line);
        }

        interpreter.run(code);
    }
}

int runFile(ferrit::Interpreter &interpreter, const std::string &path) {
    std::ifstream codeFile{path};
    if (!codeFile.is_open()) {
        std::cerr << "error: could not open file at \"" << path << "\"" << std::endl;
        return -1;
    }
    std::string code{
        std::istreambuf_iterator<char>(codeFile),
        std::istreambuf_iterator<char>()};

    ferrit::InterpretResult result = interpreter.run(code);
    switch (result) {
    case ferrit::InterpretResult::Ok:
        return 0;
    case ferrit::InterpretResult::ParseError:
        return 1;
    case ferrit::InterpretResult::CompileError:
        return 2;
    case ferrit::InterpretResult::RuntimeError:
        return 3;
    default:
        // shouldn't happen
        return -1;
    }
}

int main(int argc, char *argv[]) {
    try {
        cxxopts::Options optionsSpec = makeOptions();
        cxxopts::ParseResult flags = optionsSpec.parse(argc, argv);
        if (flags.count("help")) {
            std::cout << optionsSpec.help() << std::endl;
            return 0;
        }

        auto interpreter = std::make_unique<ferrit::BytecodeInterpreter>(
            ferrit::InterpretOptions{
                .printAst = flags["print-ast"].as<bool>(),
                .silent = flags["silent"].as<bool>(),
                .plain = flags["plain"].as<bool>(),
                .traceVm = flags["trace-vm"].as<bool>()
            });

        if (flags.count("file")) {
            return runFile(*interpreter, flags["file"].as<std::string>());
        } else {
            return runRepl(*interpreter);
        }
    } catch (const std::exception &e) {
        std::cerr << "internal compiler error: " << e.what() << std::endl;
        throw;
    }
}