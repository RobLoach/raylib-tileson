cmake_minimum_required(VERSION 3.11)
project (raylib-tileson
    VERSION 1.0.0
    DESCRIPTION "raylib-tileson"
    HOMEPAGE_URL "https://github.com/robloach/raylib-tileson"
    LANGUAGES C CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib ${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib)
endif()

# raylib-tileson
add_subdirectory(src)

# Examples
option(BUILD_RAYLIB_TILESON_EXAMPLE "Example" ON)
if(BUILD_RAYLIB_TILESON_EXAMPLE)
    add_subdirectory(example)
endif()

# Testing
# include(CTest)
# enable_testing()
# if(BUILD_TESTING AND BUILD_RAYLIB_TILESON_EXAMPLE)
#     set(CTEST_CUSTOM_TESTS_IGNORE
#         pkg-config--static
#     )
#     add_subdirectory(test)
# endif()
