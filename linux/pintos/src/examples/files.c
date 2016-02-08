#include <stdio.h>

#include "syscall.h"

int main(int argc, char** argv)
{
    char* filename = "yolo";

    create(filename, 300);
    int file = open(filename);

    printf("Opened file: %i\n", file);

    printf("Filesize: %i\n", filesize(file));

    printf("Wrote %i bytes\n", write(file, "#Yolo", 5));

    //remove(filename);
}
