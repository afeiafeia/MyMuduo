/**
 * @copyright Copyright (c) 2022..
 * 
 * @file    MutexLockGuard.h
 * @brief   利用RAII机制封装互斥锁(对象创建时上锁，销毁时解锁)
 * @author  zhangafei
 * @date    2022-09-15
 * @email   1763029328@qq.com
 */
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