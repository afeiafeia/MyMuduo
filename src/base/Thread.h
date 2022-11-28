/**
 * @copyright Copyright (c) 2022..
 * 
 * @file    Thread.h
 * @brief   posix线程的封装
 * @author  zhangafei
 * @date    2022-10-06
 * @email   1763029328@qq.com
 */
#ifndef THREAD_H
#define THREAD_H
#include "noncopyable.h"
#include "Semaphore.h"
#include <pthread.h>
#include <functional>
namespace afa
{

    class Thread:public noncopyable
    {
    public:
        typedef std::function<void()> ThreadFun;
    private:
        bool        m_started;          //是否已经启动的标志
        bool        m_joined;           //是否join
        pthread_t   m_pthread_id;       //线程id
        pid_t       m_tid;              //线程的tid
        std::string m_name;             //线程的名称
        ThreadFun   m_func;             //线程的工作函数
        Semaphore   m_sem;              //信号量
    public:
        /**
         * @brief   线程构造函数
         * @param   fun         线程的工作函数
         * @param   name        线程的名称
         * @author  zhangafei
         * @date    2022-10-06
         */
        Thread(const ThreadFun& fun,const std::string &name = std::string());

        /**
         * @brief   析构函数
         * @author  zhangafei
         * @date    2022-10-06
         */
        ~Thread();

        /**
         * @brief   线程启动函数，由主线程调用，将会使主线程阻塞直到线程工作函数运行起来
         * @author  zhangafei
         * @date    2022-10-06
         */
        void Start();

        /**
         * @brief   获取线程的名称
         * @return  std::string 
         * @author  zhangafei
         * @date    2022-10-06
         */
        std::string GetName(){return m_name;}

        /**
         * @brief   调用pthread_join，由主线程调用，调用线程(主线程)将阻塞，直到该线程运行完成退出(结束)
         * @author  zhangafei
         * @date    2022-10-06
         */
        void Join();


    };
}



#endif