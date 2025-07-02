#!/bin/bash

# Build script for Pangea C++ interpreter
set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building Pangea C++ Interpreter${NC}"

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo -e "${RED}Error: CMakeLists.txt not found. Please run this script from the project root.${NC}"
    exit 1
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo -e "${YELLOW}Creating build directory...${NC}"
    mkdir build
fi

cd build

# Configure with CMake
echo -e "${YELLOW}Configuring with CMake...${NC}"
cmake ..

# Build the project
echo -e "${YELLOW}Building project...${NC}"
make -j$(nproc)

# Check if build was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build successful!${NC}"
    
    # Run tests if they exist
    if [ -f "tests/pangea_tests" ]; then
        echo -e "${YELLOW}Running tests...${NC}"
        ./tests/pangea_tests
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}All tests passed!${NC}"
        else
            echo -e "${RED}Some tests failed!${NC}"
            exit 1
        fi
    fi
    
    echo -e "${GREEN}Pangea C++ interpreter is ready!${NC}"
    echo -e "Run ${YELLOW}./build/src/pangea${NC} to start the interpreter"
    echo -e "Or try ${YELLOW}./build/src/pangea examples/hello.pangea${NC}"
else
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi
