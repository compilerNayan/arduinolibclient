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

# Fetch arduino-core (includes springbootplusplus-web and springbootplusplus-data)
FetchContent_Declare(
    arduino-core
    GIT_REPOSITORY https://github.com/compilerNayan/arduinolib5.git
    GIT_TAG        main  # or use a specific tag/commit like "v1.0.0"
)

# Fetch and use the library's CMakeLists.txt
# This will automatically fetch and include arduinolib2 and arduinolib3
FetchContent_MakeAvailable(arduino-core)

# Add test executable only (ignore other cpp files)
add_executable(user_repository_tests
    src/rest_tests.cpp
)

# Add server executable
add_executable(desktop_server
    src/desktop_server.cpp
)

# Include directories (if needed for headers)
target_include_directories(user_repository_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(desktop_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Find libcurl
find_package(CURL REQUIRED)

# Link the library (arduinolib4 includes both springbootplusplus-web and springbootplusplus-data)
target_link_libraries(user_repository_tests PRIVATE 
    arduino-core
    CURL::libcurl
)

target_link_libraries(desktop_server PRIVATE 
    arduino-core
    CURL::libcurl
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(user_repository_tests PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
    target_compile_options(desktop_server PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Generate device macros from device_config.ini
find_program(PYTHON_EXECUTABLE python3 python REQUIRED)
set(DEVICE_CONFIG_INI "${CMAKE_CURRENT_SOURCE_DIR}/device_config.ini")
set(DEVICE_MACRO_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_device_macros.py")

if(EXISTS "${DEVICE_CONFIG_INI}" AND EXISTS "${DEVICE_MACRO_SCRIPT}")
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} "${DEVICE_MACRO_SCRIPT}" cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE DEVICE_MACRO_FLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    
    if(DEVICE_MACRO_FLAGS)
        # Parse the flags and add them as compile definitions
        # The script outputs space-separated -D flags like "-DMACRO1=value1 -DMACRO2=value2"
        string(REPLACE " " ";" MACRO_LIST "${DEVICE_MACRO_FLAGS}")
        foreach(FLAG ${MACRO_LIST})
            if(FLAG MATCHES "^-D(.+)$")
                # Extract macro definition (e.g., SWITCH_1_RELAY_PIN_INDEX=101)
                string(REGEX REPLACE "^-D" "" MACRO_DEF "${FLAG}")
                # Split into name and value
                string(FIND "${MACRO_DEF}" "=" EQUAL_POS)
                if(NOT EQUAL_POS EQUAL -1)
                    math(EXPR EQUAL_POS "${EQUAL_POS}")
                    string(SUBSTRING "${MACRO_DEF}" 0 ${EQUAL_POS} MACRO_NAME)
                    math(EXPR VALUE_START "${EQUAL_POS} + 1")
                    string(SUBSTRING "${MACRO_DEF}" ${VALUE_START} -1 MACRO_VALUE)
                    target_compile_definitions(user_repository_tests PRIVATE ${MACRO_NAME}=${MACRO_VALUE})
                    target_compile_definitions(desktop_server PRIVATE ${MACRO_NAME}=${MACRO_VALUE})
                endif()
            endif()
        endforeach()
        message(STATUS "Device macros generated from device_config.ini")
    endif()
else()
    message(STATUS "device_config.ini not found, skipping device macro generation")
endif()

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

