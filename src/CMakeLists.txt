add_library(ferrit Lexer.cpp Lexer.h Token.cpp Token.h Expression.cpp Expression.h Statement.cpp Statement.h Parser.cpp Parser.h AstElements.cpp AstElements.h AstPrinter.cpp AstPrinter.h Visitor.h Error.h Error.cpp ErrorReporter.cpp ErrorReporter.h vm/Chunk.cpp vm/Chunk.h vm/Disassembler.cpp vm/Disassembler.h vm/Value.cpp vm/Value.h vm/VirtualMachine.cpp vm/VirtualMachine.h Interpreter.cpp Interpreter.h vm/BytecodeInterpreter.cpp vm/BytecodeInterpreter.h vm/BytecodeCompiler.cpp vm/BytecodeCompiler.h vm/NativeHandler.h vm/NativeHandler.cpp ParseError.h ParseError.cpp vm/CompileError.cpp vm/CompileError.h)
target_include_directories(ferrit SYSTEM PUBLIC ${LLVM_INCLUDE_DIRS})
target_compile_definitions(ferrit PUBLIC ${LLVM_DEFINITIONS})
target_link_libraries(ferrit PUBLIC termcolor cxxopts)
llvm_config(ferrit core orcjit native)

add_executable(ferritc main.cpp)
target_link_libraries(ferritc PUBLIC ferrit)