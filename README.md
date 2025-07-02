# Pangea C++

A modern C++20 implementation of the Pangea programming language interpreter, ported from the original Java version.

## Features

- **Modern C++20**: Utilizes the latest C++ features including concepts, ranges, and std::variant
- **Type-safe Value System**: Robust value representation using std::variant
- **Phrase-building Parser**: Implements the unique phrase-building parsing mechanism
- **Interactive Mode**: REPL for experimentation and learning
- **Comprehensive Testing**: Full test suite using Catch2
- **Cross-platform**: Builds on Linux, macOS, and Windows

## Building

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.20 or higher
- Catch2 (automatically fetched via CMake)

### Build Instructions

```bash
# Clone the repository
git clone <repository-url>
cd pangea-cpp

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Run tests
./tests/pangea_tests

# Run the interpreter
./src/pangea
```

### Quick Build Script

For convenience, you can use the provided build script:

```bash
./scripts/build.sh
```

## Usage

### Interactive Mode

Start the interpreter without arguments to enter interactive mode:

```bash
$ ./pangea
Pangea C++ Interpreter
Type 'exit' to quit, 'help' for help.

pangea> println "Hello, World!"
Hello, World!
pangea> plus 2 3
=> 5
pangea> exit
Goodbye!
```

### Execute Files

Run Pangea programs from files:

```bash
$ ./pangea examples/hello.pangea
Hello, World!
```

### Evaluate Code Directly

Execute code directly from the command line:

```bash
$ ./pangea -e "plus 2 3"
5
```

### Command Line Options

- `-h, --help`: Show help message
- `-i, --interactive`: Start interactive mode
- `-e, --eval CODE`: Evaluate code directly

## Language Syntax

Pangea uses a prefix notation with phrase-building semantics:

### Basic Arithmetic

```pangea
plus 2 3        # 5
minus 10 4      # 6
times 3 4       # 12
divide 15 3     # 5
power 2 3       # 8
```

### String Operations

```pangea
plus "Hello, " "World!"    # "Hello, World!"
length "hello"             # 5
```

### Comparisons

```pangea
equal 5 5       # true
less 3 7        # true
greater 10 5    # true
```

### Logical Operations

```pangea
and true false  # false
or true false   # true
not false       # true
```

### Nested Expressions

```pangea
plus times 2 3 4    # (2 * 3) + 4 = 10
```

### I/O Operations

```pangea
print "Hello"           # Print without newline
println "Hello"         # Print with newline
input                   # Read user input
```

### Type System

```pangea
type 42                 # "number"
type "hello"            # "string"
type true               # "boolean"

number "42"             # Convert to number
string 123              # Convert to string
```

## Project Structure

```
pangea-cpp/
├── CMakeLists.txt          # Main build configuration
├── README.md               # This file
├── .github/
│   └── copilot-instructions.md
├── include/                # Header files
│   ├── value.hpp          # Type-safe value system
│   ├── function_entry.hpp # Function registry and metadata
│   ├── parser.hpp         # Tokenization and parsing
│   └── interpreter.hpp    # Main interpreter class
├── src/                   # Source files
│   ├── value.cpp
│   ├── function_entry.cpp
│   ├── parser.cpp
│   ├── interpreter.cpp
│   └── main.cpp
├── tests/                 # Test files
│   └── test_main.cpp
├── examples/              # Example Pangea programs
│   ├── hello.pangea
│   ├── basics.pangea
│   └── types.pangea
└── scripts/               # Build and utility scripts
    └── build.sh
```

## Architecture

### Value System

The `Value` class provides type-safe representation of all Pangea values using `std::variant`:

- Numbers (double)
- Strings (std::string)
- Booleans (bool)
- Arrays (std::vector<Value>)
- Objects (std::unordered_map<std::string, Value>)
- Void (for functions with no return value)

### Function Registry

The `FunctionEntry` class manages function metadata and invocation:

- Function name and arity
- Built-in function implementations
- Type-safe parameter handling

### Parser

The `Parser` class handles:

- Tokenization with whitespace handling
- String literal parsing with quote support
- Comment removal (# to end of line)
- Input cleaning and normalization

### Interpreter

The `Interpreter` class implements:

- Phrase-building parsing mechanism
- Function call resolution and execution
- Built-in function library
- Stack management for calls and loops

## Built-in Functions

### Arithmetic
- `plus a b` - Addition or string concatenation
- `minus a b` - Subtraction
- `times a b` - Multiplication
- `divide a b` - Division
- `power a b` - Exponentiation

### Comparison
- `equal a b` - Equality test
- `less a b` - Less than
- `greater a b` - Greater than

### Logical
- `and a b` - Logical AND
- `or a b` - Logical OR
- `not a` - Logical NOT

### I/O
- `print value` - Print without newline
- `println value` - Print with newline
- `input` - Read user input

### Type Operations
- `type value` - Get type name
- `string value` - Convert to string
- `number value` - Convert to number
- `length value` - Get length

### Data Structures
- `array` - Create empty array
- `object` - Create empty object
- `get collection key` - Get value from collection
- `set collection key value` - Set value in collection

### Control Flow
- `if condition then else` - Conditional execution
- `times_loop count body` - Loop execution
- `each collection body` - Iterate over collection

## Testing

The project includes comprehensive tests using Catch2:

```bash
# Run all tests
./build/tests/pangea_tests

# Run specific test sections
./build/tests/pangea_tests "[value]"
./build/tests/pangea_tests "[interpreter]"
```

## Contributing

1. Follow the C++20 modern practices outlined in `.github/copilot-instructions.md`
2. Add tests for new features
3. Update documentation as needed
4. Use meaningful commit messages

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Original Pangea interpreter implemented in Java
- Catch2 testing framework
- Modern C++ community for best practices and standards
