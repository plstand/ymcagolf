#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stopwatch.h"
#include "ymca_timing.h"

int main(int argc, char *argv[])
{
    struct timespec tp_start;
    const struct timedtext *item;

    /* Record the starting time, from which this program measures
       all time intervals. */
    read_clock(&tp_start);

    /* For each item... */
    item = ymca_text;
    do {
        /* Write the item to standard output after the appropriate delay. */
        sleep_until(&tp_start, item->start);
        fputs(item->text, stdout);

        /* This is not strictly necessary unless there is
           an item not ending in a newline. */
        fflush(stdout);
    } while ((++item)->text);

    exit(EXIT_SUCCESS);
}
