cmake_minimum_required(VERSION 3.22)
project(SkipList)

set(CMAKE_CXX_STANDARD 14)

add_executable(SkipList main.cpp base/Node.h base/Random.h base/SkipList.h)
