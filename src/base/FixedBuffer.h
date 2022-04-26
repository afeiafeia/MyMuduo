#ifndef FIXED_BUFFER_H
#define FIXED_BUFFER_H
#include <stdlib.h>
#include <string.h>
#include "noncopyable.h"
namespace afa
{
    template<int SIZE>
    class FixedBuffer:public noncopyable
    {
    private:
        char  buffer_[SIZE];
        char* cur_;

    public:
        FixedBuffer()
        :cur_(buffer_)
        {

        }
        ~FixedBuffer(){}

        void Append(const char* logLine,int len)
        {
            memcpy(cur_,logLine,len);
            cur_+=len;
        }
        
        int length()
        {
            return static_cast<int>(cur_-buffer_);
        }

        int capacity()
        {
            char* tmp = buffer_+sizeof buffer_;
            return static_cast<int>(tmp-cur_);
        }

        const char* data()
        {
            return buffer_;
        }

        const char* current()
        {
            return cur_;
        }

        void reset()
        {
            cur_ = buffer_;
        }

        void bzero()
        {
            memset(buffer_,0,sizeof buffer_);
        }



    };
}
#endif