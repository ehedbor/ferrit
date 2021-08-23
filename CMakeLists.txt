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

# Verify that the submodules were installed
function(install_submodule submodule_name submodule_path)
    if (NOT EXISTS "${PROJECT_SOURCE_DIR}/${submodule_path}/CMakeLists.txt")
        message(FATAL_ERROR "Submodule '${submodule_name}' not downloaded! Please update submodules and try again.")
    endif()
    add_subdirectory(${submodule_path})
endfunction()

install_submodule(result extern/result)
install_submodule(Catch2 extern/Catch2)


find_package(LLVM 12 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} in \"${LLVM_DIR}\"")

# Add extra warnings
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
