# 1. Force the policy minimum before anything else
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

cmake_minimum_required(VERSION 3.11...3.28) # Range syntax is safer for modern CMake

# 2. CMP0135 is still vital for FetchContent timestamps
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)

project(queue-snake C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.0
)

# Optimization: Don't build raylib tests/examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

# --- Sources ---
set(SOURCES
        src/main.c
        src/queue.c
)

add_executable(snake ${SOURCES})
target_include_directories(snake PRIVATE include)

# --- Linking ---
find_package(Threads REQUIRED)

if(APPLE)
    target_link_libraries(snake raylib Threads::Threads "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif(UNIX)
    # Added 'm' (math) and 'dl' (dynamic loader) which are usually needed on Linux
    target_link_libraries(snake raylib Threads::Threads m dl rt X11)
else()
    target_link_libraries(snake raylib Threads::Threads winmm gdi32)
endif()