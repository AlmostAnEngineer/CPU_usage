#include "Header.h"

void sigcatch(int signum)
{
    sendlog(SIGTERM_ERROR);
    allow = false;
}