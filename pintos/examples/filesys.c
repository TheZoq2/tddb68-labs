#include "../lib/user/syscall.h"
#include <stdio.h>
#include <string.h>

int success = 1;
int total_success = 1;
static void test_success()
{
    if(!success)
    {
        printf("TEST FAIL\n");

        total_success = 0;
    }
    else
    {
        printf("TEST PASS\n");
    }

    success = 1;
}

int main()
{

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
    char file_content[test_len];

    for(unsigned i = 0; i < test_len; ++i)
    {
        //Selecting 'random' printable ascii chars
        file_content[i] = 33 + i % (126 - 33);
    }

    //Write that content
    unsigned amount = write(fd, file_content, 512);

    if(amount != test_len)
    {
        printf("Incorrect number of bytes written. Wrote %i, expected %i\n", amount, test_len);

        success = 0;
    }

    test_success();

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

    test_success();

    printf("Regular read test\n");
    char read_buff[test_len];

    read(fd, read_buff, test_len);

    for(unsigned i = 0; i < test_len; ++i)
    {
        if(file_content[i] != read_buff[i])
        {
            printf("Read %c but expected %c when reading sequentially\n", file_content[i], read_buff[i]);

            success = 0;
        }
    }

    test_success();

    printf("Seek test\n");
    const unsigned seek_amount = 5;
    unsigned seek_spots[5];
    
    seek_spots[0] = 50;
    seek_spots[1] = 25; 
    seek_spots[2] = 1 ;
    seek_spots[3] = 10;
    seek_spots[4] = 40;

    for(unsigned i = 0; i < seek_amount; ++i)
    {
        seek(fd, seek_spots[i]);

        char buffer;
        int amount = read(fd, &buffer, 1);

        if(amount != 1)
        {
            printf("Read %i bytes, expected %i\n", amount, 1);

            success = 0;
        }

        if(buffer != file_content[seek_spots[i]])
        {
            printf("Read %c, expected %c when seeking to %i\n", buffer, file_content[seek_spots[i]], seek_spots[i]);
            
            success = 0;
        }
    }

    test_success();

    close(fd);
    int removed = remove("test");

    if(!removed)
    {
        printf("File removal failed");
        success = 0;
    }

    test_success();
    
    printf("Single threaded tests done\n");

    if(total_success)
    {
        printf("All tests PASSED");
    }
}
