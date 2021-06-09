#include "pthreadmutex.h"

PthreadMutex::PthreadMutex()
{
    pthread_mutex_init(&m_mutex, nullptr);
}

void PthreadMutex::lock()
{
    pthread_mutex_lock(&m_mutex);
}

void PthreadMutex::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

PthreadMutex::ScopedLock::ScopedLock(PthreadMutex& p_mutex)
    : m_mutex(p_mutex)
{
    m_mutex.lock();
}

PthreadMutex::ScopedLock::~ScopedLock()
{
    m_mutex.unlock();
}
