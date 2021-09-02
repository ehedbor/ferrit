add_library(ferrit Lexer.cpp Lexer.h Token.cpp Token.h Expression.cpp Expression.h Statement.cpp Statement.h Parser.cpp Parser.h AstElements.cpp AstElements.h ResultTry.h AstPrinter.cpp AstPrinter.h Visitor.h Error.h Error.cpp)
target_include_directories(ferrit SYSTEM PUBLIC ${LLVM_INCLUDE_DIRS} PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_definitions(ferrit PUBLIC ${LLVM_DEFINITIONS})
target_link_libraries(ferrit PUBLIC Result::Result PRIVATE Boost::program_options)
llvm_config(ferrit core orcjit native)

add_executable(ferritc main.cpp)
target_link_libraries(ferritc PUBLIC ferrit)