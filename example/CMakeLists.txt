find_package(raylib)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# raylib-tileson-example
add_executable(raylib-tileson-example raylib-tileson-example.c)
target_link_libraries(raylib-tileson-example PUBLIC
    raylib
    raylib-tileson
)

# Copy the resources
file(GLOB resources resources/*)
set(test_resources)
list(APPEND test_resources ${resources})
file(COPY ${test_resources} DESTINATION "resources/")
