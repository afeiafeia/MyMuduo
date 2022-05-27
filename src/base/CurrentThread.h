#ifndef CURRENT_THREAD_H
#define CURRENT_THREAD_H

namespace afa
{
    namespace CurrentThread
    {
        extern __thread int  t_cachedTid;    //线程id
        extern __thread char t_tidString[32];//线程id的字符串表示
        extern __thread int  t_tidStringLen; //线程id字符数组的有效长度
        extern __thread const char* t_threadName;

        void cacheTid();

        //获取tid
        inline int tid()
        {
            if(t_cachedTid==0)
            {
                cacheTid();
            }
            return t_cachedTid;
        }


        inline const char* tidString()
        {
            return t_tidString;
        }

        inline int tidStringLength()
        {
            return t_tidStringLen;
        }

        inline const char* name()
        {
            return t_threadName;
        }
    }
}





#endif