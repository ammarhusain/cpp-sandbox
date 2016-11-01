# Copyright (C) 2016 Apple Inc. All Rights Reserved.
cmake_minimum_required(VERSION 2.6)

#set(CMAKE_CXX_FLAGS "-Wall --std=c++14 -stdlib=libc++")
set(CMAKE_CXX_FLAGS "-Wall --std=c++14")

project(cpp-sandbox)

enable_testing()

add_definitions("-c")

add_executable(cpp-sandbox main.cpp)

add_subdirectory(compile-failure-tests)

add_subdirectory(static-loop-unrollers)
