#ifndef THREAD_H
#define THREAD_H
#include "noncopyable.h"
#include "Semaphore.h"
#include <pthread.h>
#include <functional>
namespace afa
{
    //extern double afaaa;

    class Thread:public noncopyable
    {
    public:
        typedef std::function<void()> ThreadFun;
    private:
        bool        m_started;
        bool        m_joined;         
        pthread_t   m_pthread_id;
        pid_t       m_tid;
        std::string m_name;
        ThreadFun   m_func;
        Semaphore   m_sem;
    public:
        Thread(const ThreadFun& fun,const std::string &name = std::string());
        ~Thread();
        void Start();
        std::string GetName(){return m_name;}

        void Join();


    };
}



#endif