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

//######################################################################################################################################################

void *reader();
void *analyser();
void *printer();
void init();
void *watchdog();
//void *logger();
void sigcatch(int signum);

//######################################################################################################################################################

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
    start = clock();                        // Start counting time in watchdog
    sem_post(&semaphore[0]);                //starting signal

    for (int i = 0; i < THREADS_NUM; i++)
    {
        pthread_join(P[i], NULL);           // Free threads from memory
    }
    for (int i = 0; i < SEM_NUM; i++)
    {
        sem_destroy(&semaphore[i]);         // Free semaphores from memory
    }
    free(CPU_Measures);
    free(usage);
}
//######################################################################################################################################################
void *reader()
{
    char *buffer = NULL;
    size_t len = 256;
    while (cpu_time_used < 2 || sig != 1)
    {

        sem_wait(&semaphore[0]);
        pthread_mutex_lock(&mutex[0]);
        sleep(1);
        start = clock();
        FILE *read = fopen("/proc/stat", "r");
        getline(&buffer, &len, read);
        for (int i = 0; i <= CPU_NUM; i++)
        {
            if (getline(&buffer, &len, read) == -1)
            {
                perror("error");
                exit(1);
            }
            buffer += 5;
            sscanf(buffer, "%lf  %lf  %lf  %lf  %lf  %lf  %lf", CPU_Measures[0] + i, CPU_Measures[1] + i, CPU_Measures[2] + i, CPU_Measures[3] + i, CPU_Measures[4] + i, CPU_Measures[5] + i, CPU_Measures[6] + i);
            buffer = NULL;
        }
        fclose(read);
        pthread_mutex_unlock(&mutex[0]);
        sem_post(&semaphore[1]);
    }
    return EXIT_SUCCESS;
}

//######################################################################################################################################################
void *analyser()
{
    double idle, sum;
    while (cpu_time_used < 2 || sig != 1)
    {
        sem_wait(&semaphore[1]);
        pthread_mutex_lock(&mutex[1]);
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
        sem_post(&semaphore[2]);
    }
    return EXIT_SUCCESS;
}

//######################################################################################################################################################

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

//######################################################################################################################################################

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

//######################################################################################################################################################

/*
void *logger()
{
    FILE *log = fopen("debuglogger.log", "w");
    while (cpu_time_used < 2 || sig != 1)
    {
        sem_wait(&semaphore[3]);
        pthread_mutex_lock(&mutex[3]);
        fprintf(log, "%s", logmsg);
        pthread_mutex_unlock(&mutex[3]);
    }
}
Zużycie procesora 100%, gdzieś jest bug
*/

//######################################################################################################################################################

void sigcatch(int signum)
{
    sig = 1;
}
//######################################################################################################################################################