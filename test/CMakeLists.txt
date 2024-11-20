# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# raylib-tileson-test
add_executable(raylib-tileson-test raylib-tileson-test.cpp)
target_compile_options(raylib-tileson-test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(raylib-tileson-test PUBLIC
    raylib
    raylib-tileson
)

# Copy the resources
file(GLOB resources resources/*)
set(test_resources)
list(APPEND test_resources ${resources})
file(COPY ${test_resources} DESTINATION "resources/")

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME raylib-tileson-test COMMAND raylib-tileson-test)
