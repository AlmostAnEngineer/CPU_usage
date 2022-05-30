#include <stdio.h>
#include "Header.h"
#include "Printer.c"
#include "Analyser.c"
#include "Reader.c"
#include "Watchdog.c"
#include "sigcatch.c"

int main()
{
    // SIGTERM

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigcatch;
    sigaction(SIGTERM, &action, NULL);

    // Declare usage array
    usage = (double *)calloc(PROC_DATA, sizeof(double));
    if (usage == NULL)
    {
        perror("Failed to allocate error for usage array");
        exit(1);
    }
    // Declare CPU_Measures
    CPU_Measures = (double **)calloc(PROC_DATA, sizeof(double *));
    if (CPU_Measures == NULL)
    {
        perror("Failed to allocate error for CPU_Measures array");
        exit(1);
    }
    for (int i = 0; i < PROC_DATA; ++i)
    {
        CPU_Measures[i] = (double *)calloc(CPU_NUM, sizeof(double));
        if (CPU_Measures[i] == NULL)
        {
            perror("Failed to allocate error for array");
            exit(1);
        }
    }
    // Declare semaphores
    for (int i = 0; i < SEM_NUM; i++)
    {
        sem_init(&semaphore[i], 0, 0);
    }
    if (pthread_create(&P[3], NULL, watchdog, NULL) != 0)
    {
        perror("Failer to create watchdog thread");
        exit(1);
    }
    if (pthread_create(&P[0], NULL, reader, NULL) != 0)
    {
        perror("Failer to create reader thread");
        exit(1);
    }
    if (pthread_create(&P[1], NULL, analyser, NULL) != 0)
    {
        perror("Failer to create analyser thread");
        exit(1);
    }
    if (pthread_create(&P[2], NULL, printer, NULL) != 0)
    {
        perror("Failer to create printer thread");
        exit(1);
    }
    start = clock();         // Start counting time in watchdog
    sem_post(&semaphore[0]); // starting signal

    for (int i = 0; i < THREADS_NUM; i++)
    {
        pthread_join(P[i], NULL); // Free threads from memory
    }
    for (int i = 0; i < SEM_NUM; i++)
    {
        sem_destroy(&semaphore[i]); // Free semaphores from memory
    }
    free(CPU_Measures);
    free(usage);
}

