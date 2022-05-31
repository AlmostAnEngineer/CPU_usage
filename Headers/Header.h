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
#define THREADS_NUM 5
#define MUTEX_NUM 3

// Messages
#define ETERNITY 1
#define INIT "\tSTART OF EXECUTING, PLEASE WAIT 0%%"
#define INIT_SIGTERM "\tSIGTERM SOFT QUIT INITIATED 20%%"
#define INIT_MEM "\tDYNAMIC MEMORY ALLOCATED 40%%"
#define INIT_SEM "\tSEMAPHORES INITIATED 60%%"
#define INIT_MUTEX "\tMUTEXES INITIATED 80%%"
#define INIT_THR "\tTHREADS CREATED, START WORKING 100%%"

#define CONSOLE(message) (puts(message))
#define THR_CAN_NUM 4

#define TIME 60
#define STARTMSG "PRESETTING ENDED WITH SUCCESS"

#define FAIL_MEM            "FAILED TO ALLOCATE MEMORY"
#define FAIL_THREAD         "FAILED TO CREATE THREAD"

#define THR_TIME_OUT        "THREAD TIMED OUT: STOP EXECUTION"
#define SIGTERM_ERROR       "RECIVED SIGTERM: STOP EXECUTION"

#define READER_RECIVE       "RECIVED SIGNAL FROM PRINTER: READING DATA"
#define READER_SEND         "DATA RECIVED: SENDING RAW DATA TO ANALYSER"

#define ANALYSER_RECIVE     "DATA RECIVED: STARTING ANALYSING"
#define ANALYSER_SEND       "ANALYSING DONE: SEND DATA TO PRINTER"

#define PRINTER_RECIVE      "RECIVED DATA: PRINTING"
#define PRINTER_SEND        "PRINTING DONE: SEND SIGNAL TO READER"

#define END_THR             "THREADS JOINED"
#define END_SEM             "SEMAPHORES DESTROYED"
#define MUT_END             "MUTEX DESTROYED"
#define END_MEM             "MEMORY CLEARED"

#define CONSOLE_CLEAR       (system("clear"))

//######################################################################################################################################################

double **CPU_Measures;
double prevsum[PROC_DATA] = {0};
double previdle[PROC_DATA] = {0};
double *usage;
double totalusage;
pthread_mutex_t mutex[MUTEX_NUM] = {PTHREAD_MUTEX_INITIALIZER};
sem_t semaphore[SEM_NUM];
pthread_t P[THREADS_NUM];
volatile clock_t start;
double cpu_time_used = 0;
char *logmsg;
bool allow = true;

//######################################################################################################################################################

void *reader();

void *analyser();

void *printer();

void init();

void *watchdog();

void *logger();
void initlogger();
void sendlog(char *msg);

void sigcatch();
#endif