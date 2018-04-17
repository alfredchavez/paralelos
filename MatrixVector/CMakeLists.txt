cmake_minimum_required(VERSION 3.10)
project(MatrixVector)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH /usr/lib64/mpich)

find_package(MPI)

find_package(MPI REQUIRED)

add_executable(MatrixVector main.cpp)

target_link_libraries(MatrixVector
        PRIVATE
        ${MPI_C_LIBRARIES})

target_include_directories(MatrixVector
        PRIVATE
        ${MPI_C_INCLUDE_PATH})

