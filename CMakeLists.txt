cmake_minimum_required(VERSION 3.20)
project(GameOfLifeClone)

set(CMAKE_CXX_STANDARD 17)

# glad is compiled as part of your project (it's just a .c file)
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Your executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
        external/GLFW/include
        external/glad/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        glad
        ${CMAKE_SOURCE_DIR}/external/GLFW/lib/glfw3.lib
        opengl32      # Windows system OpenGL library
)