#include "Header.h"

void *watchdog()
{
    do
    {
        sleep(0.01);
        cpu_time_used = ((double)(clock() - start)) / CLOCKS_PER_SEC;
        if (cpu_time_used > 2)
            allow = false;
    } while (allow != false);
    if(cpu_time_used < 2) goto JUMP;
    sendlog(THR_TIME_OUT);
    for (int i = 0; i < THREADS_NUM; i++)
    {
        pthread_cancel(P[i]);
    }
    for (int i = 0; i < SEM_NUM; i++)
    {
        sem_destroy(&semaphore[i]);
    }
    sendlog(END_SEM);
    for (int i = 0; i < MUTEX_NUM; i++)
    {
        pthread_mutex_destroy(&mutex[i]);
    }
    for (int i = 0; i < PROC_DATA; ++i)
    {
        free(CPU_Measures[i]);
    }
    free(CPU_Measures);
    free(usage);
    free(logmsg);

    JUMP:

    exit(1);
}