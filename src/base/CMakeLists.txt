cmake_minimum_required(VERSION 3.10)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(LIB_SRC
    MutexLock.cpp
    Logger.cpp
    Thread.cpp
    Semaphore.cpp
    LogLevel.cpp
    LogEvent.cpp
    LogFormatter.cpp
    LogFileAppender.cpp
    LogEventWarp.cpp
    Logger.cpp
    TimeStamp.cpp
)
add_library(my_tiny_muduo_base ${LIB_SRC})
target_link_libraries(my_tiny_muduo_base pthread rt)

install(TARGETS my_tiny_muduo_base DESTINATION lib)

#set_target_properties((my_tiny_muduo_base PROPERTIES OUTPUT_NAME "base")
add_subdirectory(test)