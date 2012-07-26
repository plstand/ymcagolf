#define _XOPEN_SOURCE 600

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "stopwatch.h"
#include "ymca_timing.h"

#define LOW_ERROR_LIMIT -0.125
#define HIGH_ERROR_LIMIT 0.125



static int process_item(FILE * file, struct timespec *tp_start,
                        const struct timedtext *item);
static int print_timestamp(double seconds);
static FILE *pty_open(const char *command, pid_t *pid);
static int pty_close(FILE *stream, pid_t pid);

int main(int argc, char *argv[])
{
    FILE *file;
    pid_t cpid;
    struct timespec tp_start;
    const struct timedtext *item;
    int num_failures = 0;

    /* If the second argument is omitted or is "-", read standard input.
       Otherwise, run the specified program and process its output. */
    if (argc < 2 || (argc == 2 && !strcmp(argv[1], "-"))) {
        file = stdin;
    }
    else if (argc == 2) {
        file = pty_open(argv[1], &cpid);

        if (!file) {
            perror("tester: opening PTY failed");
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "%s: too many arguments\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Record the starting time, from which this program measures
       all time intervals. */
    read_clock(&tp_start);

    /* For each item... */
    item = ymca_text;
    do {
        if (process_item(file, &tp_start, item)) {
            /* Show progress indicator. */
            fputs(".", stdout);
        }
        else {
            num_failures++;
        }
        fflush(stdout);
    } while ((++item)->text);

    if (file != stdin) {
        pty_close(file, cpid);
    }

    printf("\nTest completed with %d failure(s).\n", num_failures);

    if (!num_failures) {
        exit(EXIT_SUCCESS);
    }

    exit(EXIT_FAILURE);
}

static int process_item(FILE * file, struct timespec *tp_start,
                        const struct timedtext *item)
{
    double now, epsilon = 0.0, first_epsilon = 0.0;
    const char *expected_char;
    int actual_char, is_wrong = 0, retval = 1;

    /* For each character in the item... */
    expected_char = item->text;
    while (*expected_char) {

        /* When reading characters, skip carriage returns, as a PTY
           is used to read the output of the program under test. */
        do {

            /* Get the actual character's time and value. */
            actual_char = fgetc(file);
            now = seconds_since(tp_start);
            epsilon = now - item->start;

            if (actual_char == EOF) {
                print_timestamp(now);
                printf("Premature end of file or error!\n");
                exit(EXIT_FAILURE);
            }

        } while (actual_char == '\r');

        /* The first character's time determines if the item is early.
           (The last character's time determines if the item is late.) */
        if (expected_char == item->text) {
            first_epsilon = epsilon;
        }

        if (actual_char != *expected_char) {
            is_wrong = 1;
        }

        expected_char++;
    }

    /* There should not be any zero character item, but just in case... */
    now = seconds_since(tp_start);

    if (is_wrong) {
        print_timestamp(now);
        printf("Expected \"%s\"; got something else!\n", item->text);
        retval = 0;
    }
    if (first_epsilon < LOW_ERROR_LIMIT) {
        print_timestamp(now);
        printf("Early for \"%s\" by %.3f s!\n", item->text, -first_epsilon);
        retval = 0;
    }
    if (epsilon > HIGH_ERROR_LIMIT) {
        print_timestamp(now);
        printf("Late for \"%s\" by %.3f s!\n", item->text, epsilon);
        retval = 0;
    }

    return retval;
}

/* Prints a decimal number of seconds to standard output. */
static int print_timestamp(double seconds)
{
    return printf("[%.3f] ", seconds);
}

/* Opens a process by creating a PTY, forking, and invoking the shell. */
static FILE *pty_open(const char *command, pid_t *cpid)
{
    int fdm, fds;
    pid_t pid;

    /* Open master side. */
    fdm = posix_openpt(O_RDWR | O_NOCTTY);
    if (fdm < 0 || grantpt(fdm) || unlockpt(fdm)) {
        return NULL;
    }

    /* Open slave side. */
    fds = open(ptsname(fdm), O_RDWR);
    if (fds == -1) {
        return NULL;
    }

    /* Fork and execute the process. */
    switch ((pid = fork())) {

    case -1: /* Error */
        close(fds);
        close(fdm);
        return NULL;

    case 0: /* Child */

        /* Close master side. */
        close(fdm);

        /* Create a new session and set the controlling TTY to allow the
           process to terminate when the master side is closed. */
        setsid();
        ioctl(fds, TIOCSCTTY, 0);

        /* Redirect standard output to slave side. */
        if (fds != STDOUT_FILENO) {
            dup2(fds, STDOUT_FILENO);
            close(fds);
        }

        /* Invoke the shell. */
        execl("/bin/sh", "sh", "-c", command, (char *)0);
        _exit(127);

    }

    /* Parent: close slave side. */
    if (close(fds)) {
        return NULL;
    }

    *cpid = pid;
    return fdopen(fdm, "w+");
}

/* Closes a process opened by pty_close(). */
static int pty_close(FILE *stream, pid_t pid)
{
    int status;

    if (fclose(stream) || waitpid(pid, &status, 0) == -1) {
        return -1;
    }

    return status;
}
