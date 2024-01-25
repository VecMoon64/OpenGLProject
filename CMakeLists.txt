cmake_minimum_required(VERSION 3.12)
project(OpenGLApp)

set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_CXX_STANDARD 14)

# Set the path to GLFW and GLAD
set(GLFW_DIR "./lib/glfw-3.3.8")
set(GLAD_DIR "./lib/glad")

# Include directories
include_directories(${GLFW_DIR}/include ${GLAD_DIR}/include)

# Link directories
link_directories(${GLFW_DIR}/lib-mingw-w64)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui-master)
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)
file(GLOB IMGUI_SRC ${IMGUI_DIR}/*.cpp)
file(GLOB IMGUI_BACKEND_SRC 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp )

# Add the source file to the project
add_executable(OpenGLApp main.cpp glad.c ${IMGUI_SRC} ${IMGUI_BACKEND_SRC})

# Link the necessary libraries
target_link_libraries(OpenGLApp glfw3 opengl32)
