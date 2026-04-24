cmake_minimum_required(VERSION 3.25)

project(opengl 
    VERSION 1.0 
    LANGUAGES C CXX
)

# 1. Standard & Export Commands
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 2. Find Dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
)

set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)  # Skip install rules if embedding

FetchContent_MakeAvailable(glm)


# 3. Automatic File Discovery (with CONFIGURE_DEPENDS)
file(GLOB_RECURSE ENGINE_SOURCES 
    CONFIGURE_DEPENDS
    "src/*.cpp" 
    "src/*.hpp" 
    "src/*.h"
)

# 4. Define the Executable
add_executable(${PROJECT_NAME} 
    ${ENGINE_SOURCES} 
    lib/GLAD/glad.c
)

# 5. Include Directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    src
    lib/GLAD
)

# 6. Preprocessor Definitions (THE FIX)
# This prevents GLFW from including the system OpenGL header
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

# 7. Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw
    OpenGL::GL
    glm::glm
)


# 8. Platform Specifics
if(NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ${CMAKE_DL_LIBS})
endif()