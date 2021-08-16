cmake_minimum_required(VERSION 3.16)
project(es)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM 12 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} in \"${LLVM_DIR}\"")

add_executable(es src/main.cpp)
target_include_directories(es SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(es PRIVATE ${LLVM_DEFINITIONS})
llvm_config(es core orcjit native)
