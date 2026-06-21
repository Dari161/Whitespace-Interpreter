cmake_minimum_required(VERSION 3.14)
project(WhiteSpaceInterpreter)

enable_testing()

# Fõ alkalmazás fordítása
# Ha van külön main.cpp fájlod valahol, azt add hozzá ehhez a listához!
add_executable(whitespace_app
    src/Parser.cpp
    src/Reader.cpp
    src/WhiteSpaceInterpreter.cpp
 "src/main.cpp")

# Behúzza a tesztek mappáját
add_subdirectory(tests)