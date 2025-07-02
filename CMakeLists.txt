cmake_minimum_required(VERSION 3.20)
project(pangea-cpp VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags for better debugging and warnings
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/value.cpp
    src/function_entry.cpp
    src/parser.cpp
    src/interpreter.cpp
)

# Create static library for core functionality
add_library(pangea_core STATIC ${SOURCES})
target_include_directories(pangea_core PUBLIC include)

# Main executable
add_executable(pangea src/main.cpp)
target_link_libraries(pangea pangea_core)

# Enable testing
enable_testing()

# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

# Test executable
add_executable(pangea_tests 
    tests/test_main.cpp
)
target_link_libraries(pangea_tests pangea_core Catch2::Catch2WithMain)

# Add test
add_test(NAME pangea_unit_tests COMMAND pangea_tests)

# Install targets
install(TARGETS pangea DESTINATION bin)
install(DIRECTORY examples/ DESTINATION share/pangea/examples)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
