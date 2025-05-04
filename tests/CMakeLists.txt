cmake_minimum_required(VERSION 3.28)
project(CM_Project3)

set(CMAKE_PREFIX_PATH "C:/dev/vcpkg/installed/x64-windows")
set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../include/")

add_executable(unit_tests unittest.cpp  "../include/math.hpp")

find_package(Catch2 REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain sfml-system sfml-graphics sfml-window)

include(CTest)
include(Catch)
catch_discover_tests(unit_tests)