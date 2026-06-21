cmake_minimum_required(VERSION 3.14)
project(WhiteSpaceInterpreter)

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