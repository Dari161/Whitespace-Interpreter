cmake_minimum_required(VERSION 3.14)
project(WhiteSpaceInterpreter)

# Policies
cmake_policy(SET CMP0092 NEW)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

enable_testing()

# Dependencies
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Executable
add_executable(whitespace_app
    src/Parser.cpp
    src/Reader.cpp
    src/WhiteSpaceInterpreter.cpp
    src/main.cpp
)

add_subdirectory(tests)