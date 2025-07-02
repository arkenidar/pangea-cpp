# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Initial C++20 implementation of Pangea interpreter
- Core Value system using std::variant for type safety
- FunctionEntry system for function registry and metadata
- Parser class for tokenization and string handling
- Comprehensive built-in function library
- Interactive REPL mode
- Command-line interface with file execution and eval modes
- CMake build system with modern C++20 support
- Catch2 testing framework integration
- Example Pangea programs demonstrating language features
- Comprehensive documentation and build scripts

### Changed

- Ported from Java implementation to modern C++20
- Improved type safety with std::variant-based Value system
- Enhanced error handling with C++ exception system

### Deprecated

- None

### Removed

- None

### Fixed

- None

### Security

- None

## [0.1.0] - 2025-07-02

### Added

- Initial release of Pangea C++ interpreter
- Core language features:
  - Phrase-building parsing mechanism
  - Arithmetic operations (plus, minus, times, divide, power)
  - Comparison operations (equal, less, greater)
  - Logical operations (and, or, not)
  - String operations and concatenation
  - Type system with conversion functions
  - I/O operations (print, println, input)
  - Control flow (if conditions)
  - Basic iteration support
- Modern C++20 codebase with:
  - Smart pointer usage throughout
  - RAII principles
  - Move semantics optimization
  - Comprehensive const correctness
- Development infrastructure:
  - CMake 3.20+ build system
  - VS Code integration with tasks
  - Automated build scripts
  - GitHub Copilot instructions for consistent development
- Documentation:
  - Comprehensive README with examples
  - API documentation with Doxygen comments
  - Contributing guidelines
  - Example programs in `examples/` directory
