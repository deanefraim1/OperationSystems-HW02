#ifndef _I_THREAD_SAFE_HPP
#define _I_THREAD_SAFE_HPP

#include <pthread.h>

class IThreadSafe
{
public:

    pthread_mutex_t writerMutex;
    pthread_mutex_t readersMutex;
    pthread_mutex_t queueMutex;
    int readers;

    IThreadSafe();
    ~IThreadSafe();
    void EnterReader();
    void ExitReader();
    void EnterWriter();
    void ExitWriter();
};

#endif
