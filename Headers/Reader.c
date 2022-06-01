#include "Header.h"

void *reader()
{
    size_t len = 256;
    do
    {
        char *buffer = NULL;
        sem_wait(&semaphore[0]);
        pthread_mutex_lock(&mutex[0]);
        sendlog(READER_RECIVE);
        sleep(1);
        start = clock();
        FILE *read = fopen("/proc/stat", "r");
        getline(&buffer, &len, read);
        for (int i = 0; i < CPU_NUM; i++)
        {
            if (getline(&buffer, &len, read) == -1)
            {
                perror("error");
                exit(1);
            }
            char holder[strlen(buffer) + 1];
            strcpy(holder, buffer + 5);
            sscanf(holder, "%lf  %lf  %lf  %lf  %lf  %lf  %lf", CPU_Measures[0] + i, CPU_Measures[1] + i, CPU_Measures[2] + i, CPU_Measures[3] + i, CPU_Measures[4] + i, CPU_Measures[5] + i, CPU_Measures[6] + i);
        }
        fclose(read);
        pthread_mutex_unlock(&mutex[0]);
        sendlog(READER_SEND);
        sem_post(&semaphore[1]);
        free(buffer);
    } while (allow != false);

    return EXIT_SUCCESS;
}