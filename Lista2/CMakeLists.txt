cmake_minimum_required(VERSION 3.3)
project(lista3_zad1_mcrypt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmcrypt")

set(SOURCE_FILES main.cpp Koder.cpp Koder.h Dane.cpp Dane.h)
add_executable(lista3_zad1_mcrypt ${SOURCE_FILES})