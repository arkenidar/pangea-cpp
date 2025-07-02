<!-- Use this file to provide workspace-specific custom instructions to Copilot. For more details, visit https://code.visualstudio.com/docs/copilot/copilot-customization#_use-a-githubcopilotinstructionsmd-file -->

# Pangea C++ Interpreter - Copilot Instructions

This is a C++ implementation of the Pangea programming language interpreter, ported from Java.

## Project Context

- **Language**: Modern C++20
- **Build System**: CMake 3.20+
- **Architecture**: Based on phrase-building parsing mechanism
- **Original**: Ported from Java implementation with similar architecture

## Code Style Guidelines

- Use modern C++20 features (concepts, ranges, std::variant, etc.)
- Prefer smart pointers (std::unique_ptr, std::shared_ptr) over raw pointers
- Use RAII principles for resource management
- Follow Google C++ Style Guide naming conventions
- Use meaningful variable and function names

## Key Components

1. **Value**: Type-safe value system using std::variant
2. **Interpreter**: Core phrase-building interpreter logic
3. **Parser**: Tokenization and comment handling
4. **FunctionEntry**: Function registry and metadata

## Architecture Notes

- **Phrase-building**: Calculates phrase lengths before execution
- **Function calls**: Uses parameter indices, not direct values
- **Operators**: Support prefix, infix, and postfix operators
- **Built-ins**: Arithmetic, comparison, control flow, iteration functions

## Testing

- Use Catch2 testing framework
- Write unit tests for all core components
- Include integration tests with example Pangea programs

## Performance Considerations

- Optimize hot paths in wordExec and phraseLength
- Use move semantics where appropriate
- Consider memory pooling for Value objects
- Profile and optimize parsing performance
