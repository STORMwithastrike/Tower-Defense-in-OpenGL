cmake_minimum_required(VERSION 3.15)
project(TowerDefense LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- GLFW (built from the source included in third_party/glfw) ----
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# ---- GLAD (OpenGL 3.3 core loader) ----
add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)

# ---- Game executable (all .cpp files under src/ are picked up automatically) ----
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(TowerDefense ${GAME_SOURCES})

target_include_directories(TowerDefense PRIVATE
    src
    third_party/glm
    third_party/stb
    third_party/miniaudio)

target_link_libraries(TowerDefense PRIVATE glfw glad ${CMAKE_DL_LIBS})

# Assets are loaded from the project's assets/ folder (absolute path baked in at configure time),
# so the game works no matter where the executable is launched from.
target_compile_definitions(TowerDefense PRIVATE
    ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets/"
    GLM_ENABLE_EXPERIMENTAL)

if(MSVC)
    target_compile_options(TowerDefense PRIVATE /W3)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TowerDefense)
else()
    target_compile_options(TowerDefense PRIVATE -Wall -Wextra)
endif()
