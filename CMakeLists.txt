# Copyright (C) 2016 Apple Inc. All Rights Reserved.
cmake_minimum_required(VERSION 2.6)

if (NOT APPLE)
  set(CMAKE_C_COMPILER clang-3.5)
  set(CMAKE_CXX_COMPILER clang++-3.5)
endif()

#set(CMAKE_CXX_FLAGS "-Wall -g")

project(cpp-sandbox)

enable_testing()

add_definitions("-c")

set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -stdlib=libc++ -g")

add_executable(cpp-sandbox main.cpp)

add_subdirectory(compile-failure-tests)

add_subdirectory(streams-gcc-bug)

add_subdirectory(template-specialization)