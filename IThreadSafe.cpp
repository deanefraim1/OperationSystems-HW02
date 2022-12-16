#include "IThreadSafe.hpp"
#include "Helpers.hpp"

IThreadSafe::IThreadSafe()
{
    if(pthread_mutex_init(&mutex, NULL) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_init failed\n");

    if(pthread_cond_init(&doneCondition, NULL) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_cond_init failed\n");

    readers = 0;
}

IThreadSafe::~IThreadSafe()
{
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&doneCondition);
}


void IThreadSafe::EnterReader()
{
    pthread_mutex_lock(&mutex);

    readers++;
    if(readers == 1)
        pthread_cond_wait(&doneCondition, &mutex);

    pthread_mutex_unlock(&mutex);
}

void IThreadSafe::ExitReader()
{
    pthread_mutex_lock(&mutex);

    readers--;
    if(readers == 0)
        pthread_cond_signal(&doneCondition);

    pthread_mutex_unlock(&mutex);
}

void IThreadSafe::EnterWriter()
{
    pthread_mutex_lock(&mutex);
    if(readers > 0)
        pthread_cond_wait(&doneCondition, &mutex);
}

void IThreadSafe::ExitWriter()
{
    pthread_cond_broadcast(&doneCondition);
    pthread_mutex_unlock(&mutex);
}