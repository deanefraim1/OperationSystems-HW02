#include "IThreadSafe.hpp"
#include "Helpers.hpp"

IThreadSafe::IThreadSafe()
{
    if(pthread_mutex_init(&writerMutex, NULL) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_init failed\n");

    if(pthread_mutex_init(&readersMutex, NULL) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_init failed\n");

    if(pthread_mutex_init(&queueMutex, NULL) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_init failed\n");

    readers = 0;
}

IThreadSafe::~IThreadSafe()
{
    pthread_mutex_destroy(&writerMutex);
    pthread_mutex_destroy(&readersMutex);
    pthread_mutex_destroy(&queueMutex);
}


void IThreadSafe::EnterReader()
{
    pthread_mutex_lock(&queueMutex);
    pthread_mutex_lock(&readersMutex);

    if (readers == 0) 
        pthread_mutex_lock(&writerMutex);

    readers++;

    pthread_mutex_unlock(&queueMutex);
    pthread_mutex_unlock(&readersMutex);
}

void IThreadSafe::ExitReader()
{
    pthread_mutex_lock(&readersMutex);

    readers--;

    if (readers == 0) 
        pthread_mutex_unlock(&writerMutex);
    
    pthread_mutex_unlock(&readersMutex);
}

void IThreadSafe::EnterWriter()
{
    pthread_mutex_lock(&queueMutex);
    pthread_mutex_lock(&writerMutex);
    pthread_mutex_unlock(&queueMutex);
}

void IThreadSafe::ExitWriter()
{
    pthread_mutex_unlock(&writerMutex);
}
