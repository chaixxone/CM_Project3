cmake_minimum_required(VERSION 3.28)
project(CM_Project3)

set(CMAKE_PREFIX_PATH "C:/dev/vcpkg/installed/x64-windows")
set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

add_executable(app main.cpp)