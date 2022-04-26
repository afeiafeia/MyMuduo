#ifndef CONDITION_H
#define CONDITION_H
#include <bits/pthreadtypes.h>
#include "noncopyable.h"
#include "MutexLock.h"
#include <pthread.h>
namespace afa
{
    class Condition:public noncopyable
    {
    private:
        pthread_cond_t m_cond;
        MutexLock &m_mutex;//注意是引用
    public:
        Condition(MutexLock& mutex)
        :m_mutex(mutex)
        {
            pthread_cond_init(&m_cond,nullptr);
        }
        ~Condition()
        {
            pthread_cond_destroy(&m_cond);
        }

        void Wait()
        {
            pthread_cond_wait(&m_cond,m_mutex.get());
        }
        void Broadcast()
        {
            pthread_cond_broadcast(&m_cond);
        }
        void Signal()
        {
            pthread_cond_signal(&m_cond);
        }
    };
}

#endif