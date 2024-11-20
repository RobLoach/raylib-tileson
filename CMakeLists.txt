cmake_minimum_required(VERSION 3.11)
project(
    raylib-tileson
    VERSION 5.5.0
    DESCRIPTION "raylib-tileson"
    HOMEPAGE_URL "https://github.com/robloach/raylib-tileson"
    LANGUAGES C CXX
)

# Register the cmake folder for find_package()
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# raylib-tileson
add_subdirectory(src)

# Examples
option(BUILD_RAYLIB_TILESON_EXAMPLE "Example" ON)
if(BUILD_RAYLIB_TILESON_EXAMPLE)
    add_subdirectory(example)
endif()

# Testing
include(CTest)
enable_testing()
if(BUILD_TESTING)
    set(CTEST_CUSTOM_TESTS_IGNORE
        pkg-config--static
    )
    add_subdirectory(test)
endif()
