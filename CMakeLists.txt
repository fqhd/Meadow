cmake_minimum_required(VERSION 4.1.0)
project(mcpathtracer VERSION 0.1.0 LANGUAGES CXX C)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/meadow/*.cpp")

add_executable(meadow ${SRC_FILES} ./src/meadow/glad.c ./src/base64.cpp)

target_link_libraries(meadow PRIVATE glfw glm)

set_target_properties(meadow PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)


file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/pathtracer/*.cpp")

add_executable(pathtracer ${SRC_FILES} ./src/base64.cpp)

target_link_libraries(pathtracer PRIVATE Vulkan::Vulkan glm)

set_target_properties(pathtracer PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
