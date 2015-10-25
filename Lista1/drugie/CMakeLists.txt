cmake_minimum_required(VERSION 3.3)
project(lista2_zad2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmcrypt")

set(SOURCE_FILES main.cpp)
add_executable(lista2_zad2 ${SOURCE_FILES})