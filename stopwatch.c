#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "stopwatch.h"

#define NSEC_PER_SEC 1000000000

/* Puts the current value of CLOCK_MONOTONIC in a timespec.
   All other functions in this file assume this function's use. */
void read_clock(struct timespec *tp)
{
    if (clock_gettime(CLOCK_MONOTONIC, tp)) {
        perror("read_clock");
        exit(EXIT_FAILURE);
    }
}

/* Returns the time (in seconds) since a timespec. */
double seconds_since(const struct timespec *tp_start)
{
    struct timespec tp_now;
    read_clock(&tp_now);

    tp_now.tv_nsec -= tp_start->tv_nsec;
    if (tp_now.tv_nsec < 0) {
        tp_now.tv_sec--;
        tp_now.tv_nsec += NSEC_PER_SEC;
    }
    tp_now.tv_sec -= tp_start->tv_sec;
    return (double) (tp_now.tv_sec +
                     (tp_now.tv_nsec / (double) NSEC_PER_SEC));
}

/* Sleeps until a certain time (in seconds) following a timespec.
   This function does not return early following a signal. */
void sleep_until(const struct timespec *tp_start, const double seconds)
{
    struct timespec request;

    /* Set up a new timespec. Truncation is intentional. */
    request.tv_sec = tp_start->tv_sec + (int) seconds;
    request.tv_nsec = tp_start->tv_nsec + (((double) seconds -
                                            (int) seconds) *
                                           (double) NSEC_PER_SEC);

    if (request.tv_nsec >= NSEC_PER_SEC) {
        request.tv_sec++;
        request.tv_nsec -= NSEC_PER_SEC;
    }

    /* Wait until that time. */
    while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &request, NULL)) {
        if (errno != EINTR) {
            perror("clock_nanosleep");
            exit(EXIT_FAILURE);
        }
    }
}
