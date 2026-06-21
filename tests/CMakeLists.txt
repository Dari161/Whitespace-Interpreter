include(FetchContent)

# Google Test letöltése
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# MSVC kompatibilitás
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Tesztelõ alkalmazás fordítása a forráskódokkal együtt
add_executable(run_tests 
    reader_tests.cpp
    parser_tests.cpp
    ../src/Reader.cpp
    ../src/Parser.cpp
    ../src/WhiteSpaceInterpreter.cpp
 "whitespaceinterpreter_test.cpp")

# Header fájlok elérési útja
target_include_directories(run_tests PRIVATE ../src)

# GTest linkelés
target_link_libraries(run_tests gtest_main)

# Teszt regisztrálása
add_test(NAME WhitespaceTests COMMAND run_tests)