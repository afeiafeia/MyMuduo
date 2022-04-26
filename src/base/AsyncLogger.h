#ifndef ASYNC_LOGGER_H
#define ASYNC_LOGGER_H
#include <vector>
#include <string>
#include <memory>
#include "Thread.h"
#include "MutexLock.h"
#include "Condition.h"
#include "FixedBuffer.h"
#include "Semaphore.h"
namespace afa
{
    const int kSmallBuffer = 4096;       //4KB
    const int kLargeBuffer = 4096*1024;  //4MB
    class AsyncLogger
    {
    public:
        typedef FixedBuffer<kSmallBuffer> Buffer;
        typedef std::shared_ptr<Buffer>   BufferPtr;
        typedef std::vector<BufferPtr>    BufferVct;
    private:
        std::string baseName_;  //日志文件名称
        int periodic_;
        bool running_;
        Thread thread_;         //异步日志线程
        MutexLock mutex_;       //互斥锁
        Condition cond_;        //条件变量
        //双缓冲区
        BufferPtr curBuffer_;
        BufferPtr nextBuffer_;
        BufferVct buffers_;
        Semaphore sem_;
        

    public:
        AsyncLogger(const std::string &basename,int flushPeriodic);
        ~AsyncLogger();

        void Append(const char* logLine,int len);

        void Start()
        {
            running_ = true;
            thread_.Start();
            sem_.Wait();

        }

        void stop()
        {
            running_ = false;
            cond_.Signal();
            thread_.Join();
        }
    private:
        void ThreadFun();




    };
}



#endif