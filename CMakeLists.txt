cmake_minimum_required(VERSION 3.10)
project(FirstOpenGLProject)

set(CMAKE_CXX_STANDARD 17)

# Forrásfájlok beolvasása
message(STATUS "SOURCES:")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS Src/*.cpp lib/glad/src/*.c)
foreach(SOURCE ${SOURCES})
    message(STATUS "  - ${SOURCE}")
endforeach()


# Include könyvtárak: mindkét platformra mehet, ezek jók
include_directories(
    lib/glfw/include
    lib/glad/include
)

# Végrehajtható fájl létrehozása
add_executable(FirstOpenGLProject ${SOURCES})

# 🧠 PLATFORM-SPECIFIKUS BEÁLLÍTÁSOK:
if(WIN32)
    # Windows alatt használjuk a letöltött mingw libet
    target_link_directories(FirstOpenGLProject PRIVATE lib/glfw/lib-mingw)
    target_link_libraries(FirstOpenGLProject glfw3 opengl32)
elseif(UNIX)
    # Linux alatt a rendszerkönyvtárban van minden
    find_package(glfw3 REQUIRED) # optional
    target_link_libraries(FirstOpenGLProject glfw GL)
endif()

