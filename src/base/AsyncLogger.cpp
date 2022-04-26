#include "AsyncLogger.h"
#include "MutexLockGuard.h"
#include "Log.h"
#include <assert.h>
#include <iostream>
namespace afa
{

    AsyncLogger::AsyncLogger(const std::string &basename,int flushPeriodic)
    :baseName_(basename)
    ,periodic_(flushPeriodic)
    ,running_(false)
    ,thread_(std::bind(&AsyncLogger::ThreadFun,this),"AsyncLogger")
    ,mutex_()
    ,cond_(mutex_)
    ,curBuffer_(new Buffer)
    ,nextBuffer_(new Buffer)
    ,buffers_()
    ,sem_(0)
    {
        curBuffer_->bzero();
        nextBuffer_->bzero();
    }

    AsyncLogger::~AsyncLogger()
    {
        if(running_)
        {
            stop();
        }
    }

    void AsyncLogger::Append(const char* logLine,int len)
    {
        //异步模式下：创建一个StreamAppender,
        MutexLockGuard lockGuard(&mutex_);

        if(curBuffer_->capacity()>len)
        {
            curBuffer_->Append(logLine,len);
        }
        else
        {
            buffers_.push_back(curBuffer_);
            curBuffer_.reset();
            if(nextBuffer_)
            {
                curBuffer_ = std::move(nextBuffer_);
            }
            else
            {
                curBuffer_.reset(new Buffer);
            }
            curBuffer_->Append(logLine,len);
            cond_.Signal();
        }

    }

    //线程函数用于将缓冲区数据写入日志文件
    void AsyncLogger::ThreadFun()
    {
        sem_.Post();
        LogFile output(baseName_);
        BufferPtr curBuffer1_(new Buffer);
        BufferPtr nextBuffer1_(new Buffer);
        curBuffer1_->bzero();
        nextBuffer1_->bzero();
        BufferVct BufferToWrite;
        BufferToWrite.reserve(16);
        while(running_)
        {
            {
                MutexLockGuard lockGuard(&mutex_);
                if(buffers_.empty())
                {
                    cond_.Wait();
                }
                buffers_.push_back(curBuffer_);
                curBuffer_ = std::move(curBuffer1_);
                BufferToWrite.swap(buffers_);
                //std::swap(buffers_,BufferToWrite);
                if(!nextBuffer_)
                {
                    nextBuffer_ = std::move(nextBuffer1_);
                }
                
            }

            for(int i=0;i<BufferToWrite.size();++i)
            {
                output.Append(BufferToWrite[i]->data(),BufferToWrite[i]->length());
                BufferToWrite[i]->reset();
            }
            if(BufferToWrite.size()>2)
            {
                BufferToWrite.resize(2);
            }
            if(!curBuffer1_)
            {
                assert(!BufferToWrite.empty());
                curBuffer1_ = BufferToWrite.back();
                BufferToWrite.pop_back();
                curBuffer1_->reset();
            }
            if(!nextBuffer1_)
            {
                assert(!BufferToWrite.empty());
                nextBuffer1_ = BufferToWrite.back();
                BufferToWrite.pop_back();
                nextBuffer1_->reset();
            }
            BufferToWrite.clear();
            output.Flush();
        }
        output.Flush();
    }

}