cmake_minimum_required(VERSION 3.6)
project(Sniffer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES functions.h functions.c sniffer.c cli.c daemon.c)
add_executable(Sniffer ${SOURCE_FILES})