cmake_minimum_required(VERSION 3.20)

project(Learn_CP)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(Learn_CP
    src/main.cpp
    src/tracker.cpp
)
