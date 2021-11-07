add_executable(ferrit_tests testmain.cpp TestLexer.cpp TestParser.cpp vm/TestChunk.cpp vm/TestVm.cpp IntegrationTests.cpp)
target_include_directories(ferrit_tests PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(ferrit_tests PUBLIC ferrit PRIVATE Catch2::Catch2)

add_test(NAME TestLexer COMMAND Test)
add_test(NAME TestParser COMMAND Test)
add_test(NAME TestChunk COMMAND Test)
add_test(Name TestVm COMMAND Test)
add_test(Name IntegrationTests COMMAND Test)