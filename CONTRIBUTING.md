# Contributing to Pangea C++

Thank you for your interest in contributing to Pangea C++! This document provides guidelines for contributing to the project.

## Code of Conduct

This project follows a simple code of conduct: be respectful, inclusive, and constructive in all interactions.

## Getting Started

1. **Fork the repository** on GitHub
2. **Clone your fork** locally
3. **Create a new branch** for your feature or bugfix
4. **Make your changes** following the coding standards
5. **Test your changes** thoroughly
6. **Submit a pull request** with a clear description

## Development Setup

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.20 or higher
- Git

### Building

```bash
git clone https://github.com/your-username/pangea-cpp.git
cd pangea-cpp
./scripts/build.sh
```

### Running Tests

```bash
cd build
./pangea_tests
```

## Coding Standards

Please follow the coding standards outlined in `.github/copilot-instructions.md`:

- **Modern C++20**: Use contemporary C++ features appropriately
- **RAII**: Follow Resource Acquisition Is Initialization principles
- **Smart Pointers**: Prefer smart pointers over raw pointers
- **Const Correctness**: Use const wherever appropriate
- **Meaningful Names**: Use descriptive variable and function names

### Code Style

- Use 4 spaces for indentation (no tabs)
- Line length limit of 100 characters
- Follow Google C++ Style Guide naming conventions
- Include comprehensive documentation for public APIs

## Types of Contributions

### Bug Reports

When reporting bugs, please include:

- Clear description of the issue
- Steps to reproduce
- Expected vs actual behavior
- System information (OS, compiler version)
- Minimal code example if applicable

### Feature Requests

For new features:

- Describe the use case and motivation
- Provide examples of how it would be used
- Consider backward compatibility
- Discuss implementation approach if applicable

### Code Contributions

#### Areas for Contribution

1. **Core Interpreter**

   - Bug fixes in parsing or execution
   - Performance improvements
   - Memory optimization

2. **Built-in Functions**

   - New arithmetic or logical operations
   - Enhanced string manipulation
   - File I/O operations

3. **Language Features**

   - Control flow enhancements
   - Object-oriented features
   - Module system

4. **Testing**

   - Unit test coverage improvements
   - Integration tests
   - Performance benchmarks

5. **Documentation**
   - API documentation
   - Usage examples
   - Tutorial content

#### Pull Request Process

1. **Create a feature branch** from `main`
2. **Write tests** for your changes
3. **Ensure all tests pass** and code compiles without warnings
4. **Update documentation** if needed
5. **Commit with clear messages** following conventional commits format
6. **Push to your fork** and create a pull request

#### Commit Message Format

Use conventional commits format:

```
type(scope): description

[optional body]

[optional footer]
```

Types:

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `test`: Test additions or modifications
- `chore`: Maintenance tasks

Examples:

```
feat(interpreter): add support for lambda functions
fix(parser): handle escaped quotes in string literals
docs(readme): update build instructions for Windows
```

## Project Structure

```
pangea-cpp/
├── include/           # Public headers
├── src/              # Implementation files
├── tests/            # Unit and integration tests
├── examples/         # Example Pangea programs
├── scripts/          # Build and utility scripts
├── .github/          # GitHub workflows and templates
└── docs/            # Additional documentation
```

## Testing Guidelines

- Write unit tests for all new functions
- Include integration tests for major features
- Test edge cases and error conditions
- Maintain high test coverage
- Use Catch2 framework for consistency

## Documentation

- Document all public APIs with Doxygen comments
- Include usage examples in function documentation
- Update README.md for significant changes
- Add examples to the `examples/` directory

## Performance Considerations

- Profile code changes that might affect performance
- Consider memory usage in hot paths
- Use move semantics where appropriate
- Benchmark critical algorithms

## Backwards Compatibility

- Maintain API compatibility when possible
- Deprecate old APIs before removing them
- Document breaking changes clearly
- Provide migration guidance

## Release Process

Releases follow semantic versioning (MAJOR.MINOR.PATCH):

- **MAJOR**: Incompatible API changes
- **MINOR**: New functionality (backward compatible)
- **PATCH**: Bug fixes (backward compatible)

## Questions and Support

- **Issues**: Use GitHub Issues for bug reports and feature requests
- **Discussions**: Use GitHub Discussions for questions and ideas
- **Email**: Contact maintainers directly for security issues

## Recognition

Contributors will be acknowledged in:

- CONTRIBUTORS.md file
- Release notes
- GitHub contributors section

Thank you for contributing to Pangea C++!
