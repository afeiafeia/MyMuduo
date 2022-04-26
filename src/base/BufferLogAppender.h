#ifndef BUFFER_LOG_APPENDER_H
#define BUFFER_LOG_APPENDER_H
#include "LogAppender.h"
#include "LogFormatter.h"
#include "FixedBuffer.h"
namespace afa
{
    const int kSmallBuffer = 4096;       //4KB
    const int kLargeBuffer = 4096*1024;  //4MB

    class BufferAppender:public LogAppender
    {
    public:
        typedef FixedBuffer<kSmallBuffer> Buffer;
        typedef std::shared_ptr<Buffer>   BufferPtr;
        typedef std::vector<BufferPtr>    BufferVct;

    private:
        BufferPtr m_curBuffer;
        BufferPtr m_nextBuffer;
        LogFormatter::ptr m_formatter;

        BufferAppender();
        void Log(LogLevel::Level level,LogEvent::ptr event);
        

    };
}


#endif