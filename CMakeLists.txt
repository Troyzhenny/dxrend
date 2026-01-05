cmake_minimum_required(VERSION 3.20)

project(zhengine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(zhengine
    source/main.cpp
)


target_include_directories(zhengine PRIVATE
    include/
    third_party/
)

# Linking libraries
# target_link_libraries(zhengine PRIVATE
#     glad
#     OpenGL::GL
#     "${CMAKE_SOURCE_DIR}/third_party/GLFW/glfw3.lib"
# )
