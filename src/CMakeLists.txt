﻿cmake_minimum_required(VERSION 3.28)
project(CM_Project3)

set(CMAKE_PREFIX_PATH "C:/dev/vcpkg/installed/x64-windows")
set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../include/")

add_executable(app main.cpp  "../include/math.hpp" "../include/projection.hpp" "../include/collision_response.hpp")

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(app PRIVATE sfml-system sfml-graphics sfml-window)