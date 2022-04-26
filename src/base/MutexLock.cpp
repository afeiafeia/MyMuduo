#include "MutexLock.h"
#include <pthread.h>
namespace afa
{
    MutexLock::MutexLock()
    {
        pthread_mutex_init(&m_pthread_mutex,nullptr);
    }

    MutexLock::~MutexLock()
    {
        pthread_mutex_unlock(&m_pthread_mutex);
        pthread_mutex_destroy(&m_pthread_mutex);
    }
    void MutexLock::Lock()
    {
        pthread_mutex_lock(&m_pthread_mutex);
    }

    void MutexLock::UnLock()
    {
        pthread_mutex_unlock(&m_pthread_mutex);
    }

}