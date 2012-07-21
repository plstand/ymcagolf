#ifndef YMCA_STOPWATCH_H
#define YMCA_STOPWATCH_H

#include <time.h>

void read_clock(struct timespec *tp);
double seconds_since(const struct timespec *tp_start);
void sleep_until(const struct timespec *tp_start, const double seconds);

#endif
