cmake_minimum_required(VERSION 3.15)
project(arduinolibclient VERSION 1.0.0 LANGUAGES CXX)


# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Fetch libraries from GitHub
include(FetchContent)

# Fetch arduinolib2
FetchContent_Declare(
    arduinolib2
    GIT_REPOSITORY https://github.com/compilerNayan/arduinolib2.git
    GIT_TAG        dev  # or use a specific tag/commit like "v1.0.0"
)

# Fetch arduinolib3
FetchContent_Declare(
    arduinolib3
    GIT_REPOSITORY https://github.com/compilerNayan/arduinolib3.git
    GIT_TAG        dev  # or use a specific tag/commit like "v1.0.0"
)

# Fetch and use the libraries' CMakeLists.txt
# This will automatically run the pre-build scripts
FetchContent_MakeAvailable(arduinolib2)
FetchContent_MakeAvailable(arduinolib3)

# Add test executable only (ignore other cpp files)
add_executable(user_repository_tests
    src/controller/UserRepositoryTests.cpp
)

# Include directories (if needed for headers)
target_include_directories(user_repository_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link the libraries
target_link_libraries(user_repository_tests PRIVATE 
    arduinolib2
    arduinolib3
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(user_repository_tests PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

