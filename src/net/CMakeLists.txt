cmake_minimum_required(VERSION 3.10)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set (BASE
    ../base/MutexLock.cpp
    ../base/Logger.cpp
    ../base/Thread.cpp
    ../base/Semaphore.cpp
    ../base/LogLevel.cpp
    ../base/LogEvent.cpp
    ../base/LogFormatter.cpp
    ../base/LogFileAppender.cpp
    ../base/LogEventWarp.cpp
    ../base/Logger.cpp
    ../base/TimeStamp.cpp
)

set (RPC
    ./protobuf/CodeC.cpp
    ./Rpc/rpc.pb.cc
    ./Rpc/rpcservice.pb.cc
    ./Rpc/RpcChannel.cpp
    ./Rpc/RpcServer.cpp
)

set(HTTP
    ./http/HttpData.cpp
    ./http/HttpServer.cpp
)

set(LIB_NET
    EventLoop.cpp
    EventLoopThread.cpp
    EventLoopThreadPool.cpp
    Channel.cpp
    EpollPoller.cpp
    Acceptor.cpp
    TcpServer.cpp
    TcpConnection.cpp
    EchoServer.cpp
    Buffer.cpp
    Timer.cpp
    TimerHeap.cpp
    TimerManager.cpp
    ${RPC}
    ${HTTP}
    ${BASE}
)
#include_directories(../base)
#include_directories(../)
#link_directories(../base)
#link_directories(../)

add_library(my_tiny_muduo_net STATIC ${LIB_NET})
#target_link_libraries(my_tiny_muduo_net ${PATH}/libmy_tiny_muduo_base.a pthread protobuf)
target_link_libraries(my_tiny_muduo_net pthread)
#set_target_properties(my_tiny_muduo_net PROPERTIES OUTPUT_NAME "net")

#install(TARGETS my_tiny_muduo_net DESTINATION lib)

#add_executable(test_echo test/test_echo.cpp)
#target_link_libraries(test_echo my_tiny_muduo_net)
##
#add_executable(test_echo_main test/test_echo_main.cpp)
#target_link_libraries(test_echo_main my_tiny_muduo_net)
##
#add_executable(rpc_client test/rpc/client.cpp)
#target_link_libraries(rpc_client my_tiny_muduo_net)
##
#add_executable(rpc_server test/rpc/server.cpp)
#target_link_libraries(rpc_server my_tiny_muduo_net)

set (RPC_HEADERS
    ./protobuf/CodeC.h
    ./Rpc/rpc.pb.h
    ./Rpc/rpcservice.pb.h
    ./Rpc/RpcChannel.h
    ./Rpc/RpcServer.h
)
set(HEADERS
    EventLoop.h
    EventLoopThread.h
    EventLoopThreadPool.h
    Channel.h
    EpollPoller.h
    Acceptor.h
    TcpServer.h
    TcpConnection.h
    EchoServer.h
    Buffer.h
    Timer.h
    TimerHeap.h
    TimerManager.h
    ${RPC_HEADERS}
)

#install(FILES ${HEADERS} DESTINATION include/mymuduo/net)


add_subdirectory(test)