cmake_minimum_required(VERSION 3.16)
project(P7)

set(CMAKE_CXX_STANDARD 20)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(app application.cpp)

add_executable(test testing.cpp)

