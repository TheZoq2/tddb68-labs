#include "../lib/user/syscall.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int success = 1;

    //Create a test file
    const unsigned test_len = 512;
    bool create_success = create("test", test_len);

    if(!create_success)
    {
        printf("WARNING: File creation failed, does the file exist?\n");
    }
    int fd = open("test");

    if(fd == -1)
    {
        printf("Opening file failed\n");
        return -1;
    }

    //Generate content for it
    char file_content[512];

    for(unsigned i = 0; i < test_len; ++i)
    {
        //Selecting 'random' printable ascii chars
        file_content[i] = 33 + i % (126 - 33);
    }

    //Write that content
    int amount = write(fd, file_content, 512);

    if(amount != test_len)
    {
        printf("Incorrect number of bytes read. Read %i, expected %i\n", amount, test_len);

        success = 0;
    }

    close(fd);

    //Reopen and test read functions
    fd = open("test");
    if(fd == -1)
    {
        printf("Opening file failed when opening to read\n");
        return -1;
    }

    printf("Filesize test\n");
    int fz = filesize(fd);
    
    if(fz != test_len)
    {
        printf("Filesize is wrong. Got %i, expected %i\n", fz, test_len);
        success = 0;
    }

    printf("Seek test\n");
    const unsigned seek_amount = 5;
    const unsigned seek_spots[seek_amount] = {50, 25, 1, 100, 400};

    for(unsigned i = 0; i < seek_amount; ++i)
    {
        seek(fd, seek_spots[i]);

        char buffer;
        int amount = read(fd, &buffer, 1);

        if(amount != 1)
        {
            printf("Read %i bytes, expected %i\n", amount, 1);

            sccess = 0;
        }

        if(buffer != file_content[seek_spots[i]])
        {
            printf("Read %c, expected %c\n", buffer, file_content[seek_spots[i]]);
            
            success = 0;
        }
    }
    
    printf("Single threaded tests done\n");
}
