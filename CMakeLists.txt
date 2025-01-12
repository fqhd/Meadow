cmake_minimum_required(VERSION 3.5.0)
project(mcpathtracer VERSION 0.1.0 LANGUAGES CXX C)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(mcpathtracer ${SRC_FILES} ./src/glad.c)

add_compile_options(-ldl -framework OpenGL)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17
                                                 CXX_STANDARD_REQUIRED ON
                                                 CXX_EXTENSIONS OFF)
