#include "Header.h"

void initlogger()
{
    time_t t = time(NULL);
    logmsg = malloc(sizeof(char)*MAX_MSG_LEN);
    FILE *logfile = fopen("debuglog.log","w");
    fprintf(logfile,"\t\t\t\t CPU_USAGE_TRACKER_LOGGER\nCreated: \t\t%s\n",ctime(&t));
    fclose(logfile);
}
void *logger()
{
    time_t t = time(NULL);
    FILE *logfile = fopen("debuglog.log","a");
    fprintf(logfile,"-> %s\t\t\t\t %s\n",logmsg,ctime(&t));
    fclose(logfile);
    return 0;
}
void sendlog(char *msg)
{
    strcpy(logmsg,msg);
    if (pthread_create(&P[4], NULL, logger, NULL) != 0)
    {
        perror(FAIL_THREAD);
        exit(1);
    }
    pthread_join(P[4], NULL);
}