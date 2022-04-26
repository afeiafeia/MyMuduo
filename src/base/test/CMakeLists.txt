cmake_minimum_required(VERSION 3.10)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(Test_Log Test_Log.cpp)
target_link_libraries(Test_Log my_tiny_muduo_base)

add_executable(test_thread test_thread.cpp)
target_link_libraries(test_thread my_tiny_muduo_base)

#add_executable(EventLoop_Test EventLoop_Test.cpp)
#target_link_libraries(EventLoop_Test libbase)