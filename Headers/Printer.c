#include "Header.h"

void *printer()
{
    while (cpu_time_used < 2 || sig != 1)
    {
        sem_wait(&semaphore[2]);
        pthread_mutex_lock(&mutex[2]);
        start = clock();
        system("clear");
        puts("\n");
        printf("\t\tŚrednie zużycie \t %.2lf\t%%\n", totalusage);
        for (int u = 0; u < CPU_NUM; u++)
        {
            printf("\t\tZużycie rdzenia %d \t %.2lf\t%%\n", u + 1, usage[u]);
        }
        pthread_mutex_unlock(&mutex[2]);
        sem_post(&semaphore[0]);
    }
    return EXIT_SUCCESS;
}