cmake_minimum_required(VERSION 3.10)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(PATH
    /home/zaf/MyProject/MyMuduo/src/base/build
)
include_directories(${PROJECT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(test_log test_log.cpp)
target_link_libraries(test_log ${PATH}/libmy_tiny_muduo_base.a pthread)

#target_link_libraries(Test_Log my_tiny_muduo_base pthread)

add_executable(test_thread test_thread.cpp)
target_link_libraries(test_thread my_tiny_muduo_base)

#add_executable(EventLoop_Test EventLoop_Test.cpp)
#target_link_libraries(EventLoop_Test libbase)