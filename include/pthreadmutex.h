#pragma once
#include <pthread.h>

class PthreadMutex
{
public:
    class ScopedLock
    {
    public:
        ScopedLock(PthreadMutex& p_mutex);
        ~ScopedLock();
    private:
        PthreadMutex& m_mutex;
    };

    PthreadMutex();
    void lock();
    void unlock();

private:
    pthread_mutex_t m_mutex;
};
