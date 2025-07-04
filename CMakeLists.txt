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

# Optional testing
option(BUILD_TESTS "Build test suite with Catch2 v3" OFF)
option(FORCE_FETCHCONTENT_CATCH2 "Force use of FetchContent for Catch2 instead of system package" OFF)

if(BUILD_TESTS)
    enable_testing()
    
    # Try to find system Catch2 v3 first (unless override specified)
    if(NOT FORCE_FETCHCONTENT_CATCH2)
        find_package(Catch2 3 QUIET)
    endif()
    
    if(Catch2_FOUND AND NOT FORCE_FETCHCONTENT_CATCH2)
        message(STATUS "Using system Catch2 v3")
    else()
        # Use FetchContent if system Catch2 v3 not found or explicitly requested
        if(FORCE_FETCHCONTENT_CATCH2)
            message(STATUS "Using FetchContent for Catch2 v3 (forced)")
        else()
            message(STATUS "System Catch2 v3 not found, using FetchContent")
        endif()
        include(FetchContent)
        
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.4.0
        )
        
        FetchContent_MakeAvailable(Catch2)
        message(STATUS "Building tests with Catch2 v3 (FetchContent)")
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
