cmake_minimum_required(VERSION 3.28)
project(CM_Project3)

if (!${GNU})
	set(CMAKE_PREFIX_PATH "C:/dev/vcpkg/installed/x64-windows")
	set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../include/")

add_executable(app main.cpp shape.cpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(app PRIVATE sfml-system sfml-graphics sfml-window)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	if (${GNU})
		target_compile_options(app PUBLIC -O3 -flto)
	else()
		target_compile_options(app PUBLIC /LTO)
	endif()
endif()