#include "Header.h"

void *watchdog()
{
    while (cpu_time_used < 2 || sig != 1)
    {
        sleep(0.1);
        cpu_time_used = ((double)(clock() - start)) / CLOCKS_PER_SEC;
    }
    perror("ERROR NO RESPONSE FROM THREAD");
    pthread_cancel(P[0]);
    pthread_cancel(P[1]);
    pthread_cancel(P[2]);
    pthread_cancel(P[3]);
    for (int i = 0; i < SEM_NUM; i++)
    {
        sem_destroy(&semaphore[i]);
    }
    free(CPU_Measures);
    free(usage);
    return EXIT_SUCCESS;
}