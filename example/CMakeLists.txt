find_package(raylib)

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
