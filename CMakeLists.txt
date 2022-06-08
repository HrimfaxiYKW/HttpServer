cmake_minimum_required(VERSION 2.8)

project(httpserver_main)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_CLIENT false CACHE BOOL false)
set(BUILD_SERVER false CACHE BOOL false)

add_subdirectory(src)