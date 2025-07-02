cmake_minimum_required(VERSION 3.20)
project(pangea-cpp VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags for modern C++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Source files
set(SOURCES
    src/value.cpp
    src/function_entry.cpp
    src/parser.cpp
    src/interpreter.cpp
)

# Create static library for core functionality
add_library(pangea_core STATIC ${SOURCES})
target_include_directories(pangea_core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(pangea_core PUBLIC cxx_std_20)

# Main executable
add_executable(pangea src/main.cpp)
target_link_libraries(pangea PRIVATE pangea_core)

# Install targets
install(TARGETS pangea DESTINATION bin)
install(DIRECTORY examples/ DESTINATION share/pangea/examples)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Optional testing (disabled by default to avoid dependency issues)
option(BUILD_TESTS "Build test suite (requires Catch2)" OFF)

if(BUILD_TESTS)
    enable_testing()
    message(STATUS "Building tests enabled")
    
    # Try to find system Catch2 first
    find_package(Catch2 3 QUIET)
    if(Catch2_FOUND)
        message(STATUS "Found system Catch2 v3")
    else()
        find_package(Catch2 2 QUIET)
        if(Catch2_FOUND)
            message(STATUS "Found system Catch2 v2")
        else()
            message(WARNING "Catch2 not found. Install with: sudo apt-get install catch2-dev")
            return()
        endif()
    endif()
    
    # Test executable
    add_executable(pangea_tests 
        tests/test_main.cpp
    )
    
    target_link_libraries(pangea_tests PRIVATE 
        pangea_core 
        Catch2::Catch2WithMain
    )
    
    target_include_directories(pangea_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    # Register tests with CTest
    include(CTest)
    add_test(NAME pangea_unit_tests COMMAND pangea_tests)
    
endif()
