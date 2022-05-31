#include "Header.h"

void *analyser()
{
    double idle, sum;
    do{
        sem_wait(&semaphore[1]);
        pthread_mutex_lock(&mutex[1]);
        sendlog(ANALYSER_RECIVE);
        start = clock();
        for (int k = 0; k < CPU_NUM; k++)
        {
            sum = 0;
            idle = CPU_Measures[IDLE_NUM][k];
            for (int j = 0; j < PROC_DATA; j++)
            { 
                sum += CPU_Measures[j][k];
            }
            usage[k] = (1 - (idle - previdle[k]) / (sum - prevsum[k])) * 100; 
            prevsum[k] = sum;
            previdle[k] = idle;
        }
        totalusage = 0;
        for (int j = 0; j < CPU_NUM; j++)
        {
            totalusage += (double)usage[j]; 
        }
        totalusage /= (double)CPU_NUM;
        pthread_mutex_unlock(&mutex[1]);
        sendlog(ANALYSER_SEND);
        sem_post(&semaphore[2]);
    }while (allow != false);
    return EXIT_SUCCESS;
}