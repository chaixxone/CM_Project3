cmake_minimum_required(VERSION 3.28)
project(CM_Project3)

set(CMAKE_PREFIX_PATH "C:/dev/vcpkg/installed/x64-windows")
set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

include_directories("../include/")

add_executable(app main.cpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(app PRIVATE sfml-system sfml-graphics sfml-window)