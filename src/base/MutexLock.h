#ifndef MUTEX_LOCK_H
#define MUTEX_LOCK_H
#include "noncopyable.h"
#include <pthread.h>
namespace afa
{
    class MutexLock:public noncopyable
    {
    private:
        pthread_mutex_t m_pthread_mutex;
    public:
        MutexLock();
        ~MutexLock();

        void Lock();
        void UnLock();

        pthread_mutex_t* get()
        {
            return &m_pthread_mutex;
        }
        
    };
}



#endif