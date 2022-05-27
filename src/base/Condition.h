#ifndef CONDITION_H
#define CONDITION_H

#include <bits/pthreadtypes.h>
#include <pthread.h>

#include "MutexLock.h"
#include "noncopyable.h"

namespace afa
{
    class Condition:public noncopyable
    {
    private:
        pthread_cond_t m_cond;  //条件变量
        MutexLock      &m_mutex;//注意是引用
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

        //唤醒所有等待在条件变量上的锁
        void Broadcast()
        {
            pthread_cond_broadcast(&m_cond);
        }

        //唤醒一个等待在条件变量的锁
        void Signal()
        {
            pthread_cond_signal(&m_cond);
        }
    };
}

#endif