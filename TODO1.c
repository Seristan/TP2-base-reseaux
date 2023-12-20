#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MSG_LENG 512


int main(int argc, char *argv[]) {
    if (argc != 3) {
        char usage[50];
        int len = snprintf(usage, 50, "Usage: %s server filename\n", argv[0]);
        if (len > 0) {
            write(STDERR_FILENO, usage, len);
        }
        return EXIT_FAILURE;
    }

    const char *servername = argv[1];
    const char *filename = argv[2];
    char info[MSG_LENG];

    int len_server = snprintf(info, sizeof(info), "Server Name: %s\n", servername);
    if (len_server > 0) {
        write(STDOUT_FILENO, info, len_server);
    }

    int len_file = snprintf(info, sizeof(info), "Filename: %s\n", filename);
    if (len_file > 0) {
        write(STDOUT_FILENO, info, len_file);
    }


    return EXIT_SUCCESS;
}