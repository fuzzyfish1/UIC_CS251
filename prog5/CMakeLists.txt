cmake_minimum_required(VERSION 3.16)
project(CS251Prog5)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run tests.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main Threads::Threads)

add_executable(Google_Tests_runA testsA.cpp)

target_link_libraries(Google_Tests_runA gtest gtest_main Threads::Threads)

