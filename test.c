#include <stdio.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 256

int main() {

    FILE *fd;
    char buf[BUFFER_SIZE];
    char *text;

    fd = fopen("plik.txt", "r");
    if (fd == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return 1;
    }
    while ((text = fgets((char *) buf, BUFFER_SIZE, fd)) != NULL) {
        printf("[%i] - %s", (int) strlen(text), text);
    }
    if (errno != 0) {
        fprintf(stderr, "Error while reading lines: %s\n", strerror(errno));
        return 1;
    }
    fclose(fd);

    return 0;
}
