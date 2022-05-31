#include "Header.h"

void *watchdog()
{
    do
    {
        sleep(0.01);
        cpu_time_used = ((double)(clock() - start)) / CLOCKS_PER_SEC;
        if (cpu_time_used > 2)
            allow = false;
    } while (allow != false);
    return EXIT_SUCCESS;
}