# Copyright (C) 2016 Apple Inc. All Rights Reserved.
cmake_minimum_required(VERSION 2.6)
set(CMAKE_C_COMPILER clang-3.5)
set(CMAKE_CXX_COMPILER clang++-3.5)
set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

project(cpp-sandbox)

add_definitions("-c")

add_executable(cpp-sandbox main.cpp)