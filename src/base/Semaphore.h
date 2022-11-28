/**
 * @copyright Copyright (c) 2022..
 * 
 * @file    Semaphore.h
 * @brief   posix信号量的封装
 * @author  zhangafei
 * @date    2022-09-15
 * @email   1763029328@qq.com
 */
#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include "noncopyable.h"
#include <semaphore.h>

namespace afa
{
    class Semaphore:public noncopyable
    {
    private:
        int   m_val;  //信号量的值
        sem_t m_sem;  //posix信号量
    public:
        Semaphore(int val=1);
        ~Semaphore();

        void Wait();
        void Post();


    };
}


#endif