#ifndef _I_THREAD_SAFE_HPP
#define _I_THREAD_SAFE_HPP

#include <pthread.h>

class IThreadSafe
{
public:

    pthread_mutex_t mutex;
    pthread_cond_t doneCondition;
    int readers;

    IThreadSafe();
    ~IThreadSafe();
    void EnterReader();
    void ExitReader();
    void EnterWriter();
    void ExitWriter();
};



#endif