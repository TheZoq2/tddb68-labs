#include <stdio.h>

#include "syscall.h"

int main(int argc, char** argv)
{
    char* filename = "yolo";

    create(filename, 300);
    int file = open(filename);

    printf("Opened file: %i", file);

    printf("Filesize: %i", filesize(file));

    remove(filename);
}
