# Whitespace Interpreter

A lightweight, fully tested interpreter for the Whitespace programming language written in C++.

Whitespace is an esoteric programming language that uses only whitespace characters (Space, Tab, and Linefeed) as syntax, ignoring all other characters.

## Features

- **Full Specification Compliance:** Supports all stack operations, arithmetic, heap access, flow control, and I/O operations.
- **Separated Architecture:** Clean separation between the Parser (lexical analysis) and the Interpreter (execution state).
- **Safe Execution:** Properly catches and handles runtime exceptions (e.g., division by zero, stack underflow, invalid heap access, and missing exit statements) instead of causing segmentation faults.
- **Extensive Testing:** Thoroughly covered by unit tests and integration tests using Google Test (gtest).

## Building

The project uses CMake as its build system. To build the executable and the test suite, run the following commands:

```sh
mkdir build
cd build
cmake ..
cmake --build .

```

## Usage

You can run the interpreter from the command line. It takes the target `.ws` source file as the first argument, and an optional input string as the second.

```sh
./whitespace_app <filename.ws> [optional_input]

```

**Examples:**

Run a script without input:

```sh
./whitespace_app helloworld.ws

```

Run a script with an input string:

```sh
./whitespace_app program.ws "Example input data"

```

## Testing

The project includes a comprehensive test suite covering the `Parser`, `Reader`, and the `Interpreter` logic. To run the tests, execute `ctest` from your build directory:

```sh
ctest --output-on-failure
```
