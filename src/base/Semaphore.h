#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include "noncopyable.h"
#include<semaphore.h>

namespace afa
{
    class Semaphore:public noncopyable
    {
    private:
        int m_val;
        sem_t m_sem;
    public:
        Semaphore(int val=1);
        ~Semaphore();

        void Wait();
        void Post();


    };
}


#endif