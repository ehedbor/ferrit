#include "Interpreter.h"

#include <catch2/catch.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace ferrit::tests {
    SCENARIO("Integration tests", "[interpreter]") {
        std::ostringstream output;
        std::ostringstream errors;
        std::istringstream input;
        Interpreter interpreter{{.plain = true}, output, errors, input};

        GIVEN("Some Ferrit source code files") {
            std::vector<std::string> files{
                "fe_test/FunctionDeclarations.fe"
            };

            WHEN("the files are executed") {
                for (const std::string &filePath : files) {
                    std::ifstream fileStream{filePath};
                    std::string code{
                        std::istreambuf_iterator<char>(fileStream),
                        std::istreambuf_iterator<char>()};

                    InterpretResult result = interpreter.run(code);

                    THEN("there will be no errors") {
                        REQUIRE(result == InterpretResult::Ok);
                        REQUIRE(errors.str().empty());
                    }
                }
            }
        }
    }
}