#include <stdio.h>
#include "Headers/Header.h"
#include "Headers/Printer.c"
#include "Headers/Analyser.c"
#include "Headers/Reader.c"
#include "Headers/Watchdog.c"
#include "Headers/sigcatch.c"
#include "Headers/Logger.c"

int main()
{
    CONSOLE_CLEAR;
    CONSOLE(INIT);
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigcatch;
    sigaction(SIGTERM, &action, NULL);
    CONSOLE(INIT_SIGTERM);
    logmsg = (char *)malloc(sizeof(char)*MAX_MSG_LEN);
    if (logmsg == NULL)
    {
        perror(FAIL_MEM);
        exit(1);
    }
    usage = (double *)calloc(PROC_DATA, sizeof(double));
    if (usage == NULL)
    {
        perror(FAIL_MEM);
        exit(1);
    }
    CPU_Measures = (double **)calloc(PROC_DATA, sizeof(double *));
    if (CPU_Measures == NULL)
    {
        perror(FAIL_MEM);
        exit(1);
    }
    for (int i = 0; i < PROC_DATA; ++i)
    {
        CPU_Measures[i] = (double *)calloc(CPU_NUM, sizeof(double));
        if (CPU_Measures[i] == NULL)
        {
            perror(FAIL_MEM);
            exit(1);
        }
    }
    CONSOLE(INIT_MEM);
    for (int i = 0; i < SEM_NUM; i++)
    {
        sem_init(&semaphore[i], 0, 0);
    }
    CONSOLE(INIT_SEM);
     for (int i = 0; i < MUTEX_NUM; i++)
    {
        pthread_mutex_init(&mutex[i], NULL);
    }
    CONSOLE(INIT_MUTEX);
    if (pthread_create(&P[3], NULL, watchdog, NULL) != 0)
    {
        perror(FAIL_THREAD);
        exit(1);
    }
    if (pthread_create(&P[0], NULL, reader, NULL) != 0)
    {
        perror(FAIL_THREAD);
        exit(1);
    }
    if (pthread_create(&P[1], NULL, analyser, NULL) != 0)
    {
        perror(FAIL_THREAD);
        exit(1);
    }
    if (pthread_create(&P[2], NULL, printer, NULL) != 0)
    {
        perror(FAIL_THREAD);
        exit(1);
    }
    CONSOLE(INIT_THR);
    initlogger();
    sendlog(STARTMSG);
    start = clock();         
    sem_post(&semaphore[0]); 

    for (int i = 0; i < THREADS_NUM; i++)
    {
        pthread_join(P[i], NULL); 
    }
    sendlog(END_THR);
    for (int i = 0; i < SEM_NUM; i++)
    {
        sem_destroy(&semaphore[i]);
    }
   sendlog(END_SEM);
    for (int i = 0; i < MUTEX_NUM; i++)
    {
        pthread_mutex_destroy(&mutex[i]);
    }
    sendlog(MUT_END);
    for (int i = 0; i < PROC_DATA; ++i)
    {
        free(CPU_Measures[i]);
    }
    free(CPU_Measures);
    free(usage);
    free(logmsg);
}

