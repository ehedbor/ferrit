cmake_minimum_required(VERSION 3.16)
project(es)

set(CMAKE_CXX_STANDARD 17)

# Update submodules if Git is installed
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Updating Git submodules")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMODULE_RESULT)
        if (NOT GIT_SUBMODULE_RESULT EQUAL "0")
            message(FATAL_ERROR, "git submodule update --init --recursive failed with ${GIT_SUBMODULE_RESULT}")
        endif()
    endif()
endif()

# Verify that result was installed
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/result/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'result' not downloaded! Please update submodules and try again.")
endif()
add_subdirectory("extern/result")


find_package(LLVM 12 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} in \"${LLVM_DIR}\"")

add_executable(es src/main.cpp src/Lexer.cpp src/Lexer.h src/Token.cpp src/Token.h)
target_include_directories(es SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(es PRIVATE ${LLVM_DEFINITIONS})
target_link_libraries(es PRIVATE Result::Result)
llvm_config(es core orcjit native)
