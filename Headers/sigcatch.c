#include "Header.h"

void sigcatch()
{
    sendlog(SIGTERM_ERROR);
    allow = false;
}