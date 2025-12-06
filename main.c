/*
 * Build and run instructions (Linux only):
 *
 * 1. Compile:
 *      gcc -Wall -Wextra -O2 -o keyboard_reader main.c
 *
 * 2. Run (usually requires root privileges to read /dev/input):
 *
 *    a) Z domyślną ścieżką z KEYBOARD_DEVICE_PATH:
 *         sudo ./keyboard_reader
 *
 *    b) Z podaną ścieżką do urządzenia (np. /dev/input/event3):
 *         sudo ./keyboard_reader /dev/input/event3
 *
 * 3. To stop the program, press Ctrl+C.
 */

#include <stdio.h>

#ifndef __linux__
#error "This program can only be compiled on Linux systems."
#endif

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 1024
#define KEYBOARD_DEVICE_PATH "/dev/input/event0"

/* Open keyboard device and return file descriptor, or -1 on error */
int open_keyboard_device(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open device file %s: %s\n", path, strerror(errno));
    }
    return fd;
}

/* Process captured data from buffer */
void process_data(const char *buffer, size_t len) {
    /* Example: print raw bytes as hex values */
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", (unsigned char)buffer[i]);
    }
    printf("\n");
    fflush(stdout);
}

/* Read loop: reads from device and passes data to process_data() */
void read_loop(int fd) {
    char buffer[MAX_BUFFER_SIZE];

    while (1) {
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("Error reading from device file");
            break;
        }
        if (bytes_read == 0) {
            /* End of file or device closed */
            fprintf(stderr, "End of device stream\n");
            break;
        }

        process_data(buffer, (size_t)bytes_read);
    }
}

int main(int argc, char *argv[]) {
    const char *device_path = KEYBOARD_DEVICE_PATH;

    if (argc > 1) {
        device_path = argv[1];
    } else {
        fprintf(stderr,
                "Using default device path: %s\n"
                "You can override it: sudo ./keyboard_reader /dev/input/eventX\n",
                device_path);
    }

    int fd = open_keyboard_device(device_path);
    if (fd == -1) {
        return 1;
    }

    read_loop(fd);

    close(fd);
    return 0;
}
