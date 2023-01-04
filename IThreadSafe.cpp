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
    if(pthread_mutex_destroy(&writerMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_destroy failed\n");

    if(pthread_mutex_destroy(&readersMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_destroy failed\n");
        
    if(pthread_mutex_destroy(&queueMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_destroy failed\n");
}


void IThreadSafe::EnterReader()
{
    if(pthread_mutex_lock(&queueMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_lock failed\n");
    if(pthread_mutex_lock(&readersMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_lock failed\n");

    if (readers == 0) 
        if(pthread_mutex_lock(&writerMutex) != 0)
            Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_lock failed\n");

    readers++;

    if(pthread_mutex_unlock(&queueMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_unlock failed\n");
    if(pthread_mutex_unlock(&readersMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_unlock failed\n");
}

void IThreadSafe::ExitReader()
{
    if(pthread_mutex_lock(&readersMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_lock failed\n");

    readers--;

    if (readers == 0) 
        if(pthread_mutex_unlock(&writerMutex) != 0)
            Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_unlock failed\n");
    
    if(pthread_mutex_unlock(&readersMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_unlock failed\n");
}

void IThreadSafe::EnterWriter()
{
    if(pthread_mutex_lock(&queueMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_lock failed\n");
    if(pthread_mutex_lock(&writerMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_lock failed\n");
    if(pthread_mutex_unlock(&queueMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_unlock failed\n");
}

void IThreadSafe::ExitWriter()
{
    if(pthread_mutex_unlock(&writerMutex) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_unlock failed\n");
}
