#include <stdio.h>

#include "syscall.h"

int main(int argc, char** argv)
{
    char* filename = "yolo";

    create(filename, 10);
    int file = open(filename);

    printf("Opened file: %i", file);

    remove(filename);
}
