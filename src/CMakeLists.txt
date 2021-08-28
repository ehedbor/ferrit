add_library(escompiler Lexer.cpp Lexer.h Token.cpp Token.h Expression.cpp Expression.h Statement.cpp Statement.h Parser.cpp Parser.h AstElements.cpp AstElements.h ResultTry.h AstPrinter.cpp AstPrinter.h Visitor.h Error.h Error.cpp)
target_include_directories(escompiler SYSTEM PUBLIC ${LLVM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_compile_definitions(escompiler PUBLIC ${LLVM_DEFINITIONS})
target_link_libraries(escompiler PUBLIC Result::Result Boost::program_options)
llvm_config(escompiler core orcjit native)

add_executable(es main.cpp)
target_link_libraries(es PUBLIC escompiler)