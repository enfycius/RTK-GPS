cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(get_gps)

add_executable(get_gps
    conv.cpp
)

find_package(Eigen3 3.4 REQUIRED NO_MODULE)

SET( EIGEN3_INCLUDE_DIR "/usr/local/include/eigen3/" )
IF( NOT EIGEN3_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
ENDIF()
INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )

if(NOT MSVC)
    add_compile_options(get_gps PRIVATE -Wall -Wextra)
else()
    add_compile_options(get_gps PRIVATE -WX -W2)
endif()
