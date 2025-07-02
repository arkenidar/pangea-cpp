#!/bin/bash

echo "Building Pangea C++ Interpreter..."

# Set C++20 compiler flags
CXX_FLAGS="-std=c++20 -Wall -Wextra -pedantic -g -O0"

# Include directories
INCLUDES="-Iinclude"

# Source files
SOURCES="src/main.cpp src/value.cpp src/parser.cpp src/interpreter.cpp src/function_entry.cpp"

# Build the executable
echo "Compiling with g++..."
g++ $CXX_FLAGS $INCLUDES $SOURCES -o pangea

if [ $? -eq 0 ]; then
    echo "✅ Build successful! Executable: ./pangea"
    echo "Testing basic functionality..."
    echo -e 'println "Hello from Pangea!"\nexit' | ./pangea
else
    echo "❌ Build failed!"
    exit 1
fi
