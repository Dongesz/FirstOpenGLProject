cmake_minimum_required(VERSION 3.10)
project(FirstOpenGLProject)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp lib/glad/src/*.c)

include_directories(
    lib/glfw/include
    lib/glad/include
)

add_executable(FirstOpenGLProject ${SOURCES})

target_link_directories(FirstOpenGLProject PRIVATE lib/glfw/lib-mingw)

target_link_libraries(FirstOpenGLProject glfw3 opengl32)
