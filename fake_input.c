/*
 * Fake input event generator (Linux only)
 *
 * Build:
 *   gcc -Wall -Wextra -O2 -o fake_input fake_input.c
 *
 * Run:
 *   ./fake_input
 *
 * This will write a few example input_event structures to /tmp/fake_event.
 * Then you can run your keyboard reader like this:
 *
 *   ./keyboard_reader /tmp/fake_event
 */

#ifndef __linux__
#error "This program can only be compiled on Linux systems."
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <time.h>
#include <errno.h>

int main(void) {
    const char *path = "/tmp/fake_event";

    /* Open output file that simulates /dev/input/eventX */
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        fprintf(stderr, "Failed to open %s: %s\n", path, strerror(errno));
        return 1;
    }

    struct input_event ev;
    memset(&ev, 0, sizeof(ev));

    /* Set example timestamp */
    ev.time.tv_sec = time(NULL);
    ev.time.tv_usec = 0;

    /* Example: press and release key 'A' (KEY_A = 30) */
    ev.type = EV_KEY;
    ev.code = 30;    /* KEY_A */
    ev.value = 1;    /* key press */
    if (write(fd, &ev, sizeof(ev)) != sizeof(ev)) {
        perror("write");
    }

    ev.value = 0;    /* key release */
    if (write(fd, &ev, sizeof(ev)) != sizeof(ev)) {
        perror("write");
    }

    /* Synchronization event EV_SYN to look like real input */
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    if (write(fd, &ev, sizeof(ev)) != sizeof(ev)) {
        perror("write");
    }

    close(fd);

    printf("Wrote example input events to %s\n", path);
    printf("Run your program like this:\n");
    printf("    ./keyboard_reader %s\n", path);

    return 0;
}
