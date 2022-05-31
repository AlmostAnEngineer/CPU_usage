#include "Header.h"

void *printer()
{
    do{
        sem_wait(&semaphore[2]);
        pthread_mutex_lock(&mutex[2]);
        sendlog(PRINTER_RECIVE);
        start = clock();
        CONSOLE_CLEAR;
        puts("\n");
        printf("\t\tŚrednie zużycie \t %.2lf\t%%\n", totalusage);
        for (int u = 0; u < CPU_NUM; u++)
        {
            printf("\t\tZużycie rdzenia %d \t %.2lf\t%%\n", u + 1, usage[u]);
        }
        pthread_mutex_unlock(&mutex[2]);
        sendlog(PRINTER_SEND);
        sem_post(&semaphore[0]);
    }while (allow != false);
    return EXIT_SUCCESS;
}