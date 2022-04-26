#include "Semaphore.h"

namespace afa
{
    Semaphore::Semaphore(int val)
    :m_val(val)
    {
        sem_init(&m_sem,0,m_val);
    }

    Semaphore::~Semaphore()
    {
        sem_destroy(&m_sem);
    }

    void Semaphore::Wait()
    {
        sem_wait(&m_sem);
    }

    void Semaphore::Post()
    {
        sem_post(&m_sem);
    }
}