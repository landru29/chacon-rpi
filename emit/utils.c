#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <malloc.h>
#include "utils.h"



/**
 * Calculate a duration
 *
 * @param start start of the computing or null if
 * this is the first time this function is inoked
 *
 * @return current time
 */
struct timeval* showTime(struct timeval* start)
{
    struct timeval* stop;
    long tv_sec;
    long tv_usec;
    stop = (struct timeval*)malloc(sizeof(struct timeval));
    gettimeofday(stop, 0);
    if (!start) return stop;
    tv_sec = (start->tv_usec > stop->tv_usec) ? stop->tv_sec - start->tv_sec - 1 : stop->tv_sec - start->tv_sec;
    tv_usec = (start->tv_usec > stop->tv_usec) ? 1000000 + stop->tv_usec - start->tv_usec : stop->tv_usec - start->tv_usec;
    fprintf(stderr, "%lus %lums %luµs\n", tv_sec, tv_usec/1000, tv_sec % 1000);
    return stop;
}
