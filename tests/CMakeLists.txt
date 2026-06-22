include(FetchContent)

# Download Google Test
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# MSVC compatibility
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Build the test executable along with the source files
add_executable(run_tests 
    reader_tests.cpp
    parser_tests.cpp
    whitespaceinterpreter_test.cpp
    ../src/Reader.cpp
    ../src/Parser.cpp
    ../src/WhiteSpaceInterpreter.cpp)

# Include directories for header files
target_include_directories(run_tests PRIVATE ../src)

# Link GTest
target_link_libraries(run_tests gtest_main)

# Register the test
add_test(NAME WhitespaceTests COMMAND run_tests)