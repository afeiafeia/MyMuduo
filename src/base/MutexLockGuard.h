#ifndef MUTEX_LOCK_GUARD_H
#define MUTEX_LOCK_GUARD_H
#include "noncopyable.h"
#include "MutexLock.h"
namespace afa
{
    class MutexLockGuard:public noncopyable
    {
    private:
        MutexLock* m_mutexlock;
    public:
        MutexLockGuard(MutexLock* mutexlock)
        :m_mutexlock(mutexlock)
        {
            m_mutexlock->Lock();
        }
        ~MutexLockGuard()
        {
            m_mutexlock->UnLock();
        }
    };
}
#endif