#ifndef Header
#define Header

#include <stdio.h>
#include <sys/sysinfo.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>


#define CPU_NUM (get_nprocs_conf())
#define PROC_DATA 7
#define IDLE_NUM 3
#define MAX_MSG_LEN 100
#define SEM_NUM 3
#define THREADS_NUM 4
#define MUTEX_NUM 5

//######################################################################################################################################################

double **CPU_Measures;
double prevsum[PROC_DATA] = {0};
double previdle[PROC_DATA] = {0};
double *usage;
double totalusage; // Przechowywanie danych o zużyciu procesora
pthread_mutex_t mutex[MUTEX_NUM] = {PTHREAD_MUTEX_INITIALIZER}; // mutex blokujący wyścigi
sem_t semaphore[SEM_NUM];                                       // semafory
pthread_t P[THREADS_NUM];                                       // inicjowanie wątków
volatile clock_t start;                                         // Zmienna odliczająca ilość tick
double cpu_time_used = 0;                                       // Zmienna globalna do kalkulacji czasu
volatile sig_atomic_t sig = 0;

//######################################################################################################################################################

void *reader();
void *analyser();
void *printer();
void init();
void *watchdog();
//void *logger();
void sigcatch(int signum);
#endif