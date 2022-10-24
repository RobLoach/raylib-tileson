find_package(raylib)

option(RAYLIB_TILESON "raylib-tileson" ON)
option(RAYLIB_TILESON_SHARED "raylib-tileson-shared" OFF)

if(RAYLIB_TILESON)
    add_library(raylib-tileson STATIC "raylib-tileson.cpp")
    target_include_directories(raylib-tileson PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    target_link_libraries(raylib-tileson PUBLIC raylib)
endif()

if(RAYLIB_TILESON_SHARED)
    add_library(raylib-tileson-shared SHARED "raylib-tileson.cpp")
    target_include_directories(raylib-tileson-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    target_link_libraries(raylib-tileson-shared PUBLIC raylib)
endif()
