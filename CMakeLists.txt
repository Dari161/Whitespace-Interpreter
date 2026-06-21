include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)

# MSVC compatibility
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

cmake_minimum_required(VERSION 3.14)
project(WhiteSpaceInterpreter)

cmake_policy(SET CMP0092 NEW)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

enable_testing()

# Build the main executable
# Add your main.cpp and other source files to this list
add_executable(whitespace_app
    src/Parser.cpp
    src/Reader.cpp
    src/WhiteSpaceInterpreter.cpp
    src/main.cpp
)

# Include the tests directory
add_subdirectory(tests)