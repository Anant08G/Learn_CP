cmake_minimum_required(VERSION 3.14)
project(My_Project CXX)

add_library(my_lib STATIC
    src/library.cpp
    src/queue.cpp
    src/log.cpp
)

target_include_directories(my_lib PUBLIC include)

add_executable(Learn_CP src/main.cpp)

target_link_libraries(Learn_CP PRIVATE my_lib)
