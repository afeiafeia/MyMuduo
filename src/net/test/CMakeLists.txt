cmake_minimum_required(VERSION 3.10)
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
include_directories(../../base)
include_directories(../)
link_directories(../../base)
link_directories(../)
set(PATH
    /home/zaf/MyProject/MyMuduo/src
)
#add_executable(Test_EventLoop Test_EventLoop.cpp)
#target_link_libraries(Test_EventLoop my_tiny_muduo_base)

add_executable(test_eventloop test_eventloop.cpp)
target_link_libraries(test_eventloop my_tiny_muduo_net)

add_executable(test_echo test_echo.cpp)
#target_link_libraries(test_echo ${PATH}/net/libmy_tiny_muduo_net.a ${PATH}/base/libmy_tiny_muduo_base.a pthread)
target_link_libraries(test_echo my_tiny_muduo_net)
#
add_executable(test_echo_main test_echo_main.cpp)
target_link_libraries(test_echo_main my_tiny_muduo_net)
##
add_executable(rpc_client ./rpc/client.cpp)
target_link_libraries(rpc_client  my_tiny_muduo_net protobuf)
###
add_executable(rpc_server ./rpc/server.cpp)
target_link_libraries(rpc_server my_tiny_muduo_net protobuf)
#

add_executable(test_InetAddress test_InetAddress.cpp)
target_link_libraries(test_InetAddress my_tiny_muduo_net)

add_executable(test_TcpClient test_TcpClient.cpp)
target_link_libraries(test_TcpClient my_tiny_muduo_net)

add_executable(test_http test_http.cpp)
target_link_libraries(test_http my_tiny_muduo_net)