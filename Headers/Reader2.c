#include "Header.h"

void *reader(){
    char useless[10];
    while(cpu_time_used < 2 || sig != 0){
    sem_wait(&semaphore[0]); 
    start =clock();                                           //Oczekiwanie na sygnał z semafora
    FILE* prevread = fopen("/proc/stat","r");
    sleep(1);                                                           // Sleep umożliwia pobranie danych z następnej sekudny
    FILE* read = fopen("/proc/stat","r");                               // Sposób bezpieczniejszy i protszy niż pobieranie danych bez lagtime
    fgets(dump,sizeof(dump),prevread);
    for(int i = 0;i<=get_nprocs_conf();i++){
        fgets(dump,sizeof(dump),prevread);
        sscanf(dump,"%s  %Lf  %Lf  %Lf  %LF  %Lf  %Lf  %Lf",useless,&Prev_CPU_Measures[0][i],&Prev_CPU_Measures[1][i],&Prev_CPU_Measures[2][i],&Prev_CPU_Measures[3][i],&Prev_CPU_Measures[4][i],&Prev_CPU_Measures[5][i],&Prev_CPU_Measures[6][i]);
    }
    fclose(prevread);

    fgets(dump,sizeof(dump),read);                                      // gdzie mierzymy czas i sprawdzamy czy przekroczył sekudnę
    for(int i = 0;i<=get_nprocs_conf();i++){
        fgets(dump,sizeof(dump),read);
        sscanf(dump,"%s  %Lf  %Lf  %Lf  %Lf  %Lf  %Lf  %Lf",useless,&CPU_Measures[0][i],&CPU_Measures[1][i],&CPU_Measures[2][i],&CPU_Measures[3][i],&CPU_Measures[4][i],&CPU_Measures[5][i],&CPU_Measures[6][i]);
    }
    fclose(read);
    sem_post(&semaphore[1]);
}}