cmake_minimum_required(VERSION 3.20)

project(dxrend VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(dxrend WIN32
    source/winmain.cpp
)


target_include_directories(dxrend PRIVATE
    include/
    third_party/
)

target_link_libraries(dxrend PRIVATE
    d3d12
    dxgi
    d3dcompiler
)
