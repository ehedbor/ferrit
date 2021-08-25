add_executable(tests TestLexer.cpp TestParser.cpp)
target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(tests PUBLIC escompiler PRIVATE Catch2::Catch2)

add_test(NAME TestLexer COMMAND Test)
add_test(NAME TestParser COMMAND Test)