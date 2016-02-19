#include <stdio.h>
#include <string.h>

#include "syscall.h"

int main(int argc, char** argv)
{
    char* filename = "yolo";

    create(filename, 20);
    int file = open(filename);

    printf("Opened file: %i\n", file);

    printf("Filesize: %i\n", filesize(file));

    printf("Wrote %i bytes\n", write(file, "#Yolo", 5));

    close(file);
    file = open(filename);

    char buff[256];
    strlcpy(buff, "Fuck you", 9);
    
    unsigned read_amount = read(file, buff, 256);

    printf("Read %i bytes saying %s\n", read_amount, buff);

    //remove(filename);
}
