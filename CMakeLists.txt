cmake_minimum_required (VERSION 3.3)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
project(benchmark CXX)


set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra -O3")


if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif(CMAKE_BUILD_TYPE MATCHES DEBUG)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)


# Prepare "Catch" library for other executables
add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/catch)


find_package(Threads)

set(LIBS "numa" "pthread")

file(GLOB SRC_FILES ./src/*.cpp)
add_library(${PROJECT_NAME}_lib ${SRC_FILES})
target_include_directories(${PROJECT_NAME}_lib PRIVATE ./include)
target_link_libraries(${PROJECT_NAME}_lib ${LIBS})


add_executable(${PROJECT_NAME}_test_basic_local ./catch/main.cpp ./test/basic.cpp)
target_link_libraries(${PROJECT_NAME}_test_basic_local ${PROJECT_NAME}_lib ${LIBS} )
target_include_directories(${PROJECT_NAME}_test_basic_local PRIVATE ./include ./catch)

