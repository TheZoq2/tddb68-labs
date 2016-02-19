#include <stdlib.h>
#include <stdio.h>

//Create a file
#include <syscall.h>

int
main (void)
{
    char* filename = "yolo";
    int size = 124;

    //printf("Original ptr %p", filename);
    bool result = create(filename, size);

    //printf("touch: Done create");

    //halt();
}
