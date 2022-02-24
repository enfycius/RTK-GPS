cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(get_gps)

add_executable(get_gps
    get_gps.cpp
)

find_package(MAVSDK REQUIRED)

target_link_libraries(get_gps
    MAVSDK::mavsdk
)

if(NOT MSVC)
    add_compile_options(get_gps PRIVATE -Wall -Wextra)
else()
    add_compile_options(get_gps PRIVATE -WX -W2)
endif()
