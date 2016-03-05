#include "../lib/user/syscall.h"
#include <stdio.h>
#include <string.h>

int success = 1;
int total_success = 1;
const unsigned test_len = 512;
char* file_content = NULL;

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

static int create_and_open_file(char* filename)
{
    printf("Creating file\n");

    //Create a test file
    bool create_success = create(filename, test_len);

    if(!create_success)
    {
        printf("WARNING: File creation failed, does the file exist?\n");
    }
    int fd = open(filename);

    if(fd == -1)
    {
        printf("Opening file failed\n");
        success = 0;
    }

    printf("Opened file\n");
    return fd;
}



static void write_file(int fd)
{
    printf("Starting write test\n");
    //Generate content for it

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
}

static void filesize_test(int fd)
{
    printf("Filesize test\n");
    int fz = filesize(fd);
    
    if(fz != test_len)
    {
        printf("Filesize is wrong. Got %i, expected %i\n", fz, test_len);
        success = 0;
    }

    test_success();
}

static void regular_read_test(int fd)
{
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
}

static void seek_test(int fd)
{
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
}
static void edge_seek_test(int fd)
{
    printf("Seek beyond file edge test\n");
    const unsigned edge_amount = 4;
    unsigned edge_spots[edge_amount];
    
    edge_spots[0] = 0;
    edge_spots[1] = 1;
    edge_spots[2] = 2; 
    edge_spots[3] = 500;

    for(unsigned i = 0; i < edge_amount; ++i)
    {
        unsigned final_spot = test_len + edge_spots[i];
        seek(fd, final_spot);

        char buffer;
        int amount = read(fd, &buffer, 1);

        if(amount != 1)
        {
            printf("Read %i bytes, expected %i\n", amount, 1);

            success = 0;
        }

        if(buffer != file_content[test_len-1])
        {
            printf("Read '%c', expected when reading %i'%c'\n", buffer, file_content[test_len-1], final_spot);
            
            success = 0;
        }
    }

    test_success();
}

static void remove_test(int fd, char* filename)
{
    printf("File removal test\n");
    close(fd);
    int removed = remove(filename);

    if(!removed)
    {
        printf("File removal failed");
        success = 0;
    }

    test_success();
}

int main()
{
    char* filename = "test";
    int fd= create_and_open_file(filename);

    //Array that will store the file content
    char fc[test_len];
    file_content = fc;

    //The file couldn't be opened. Quit now because nothing else will pass
    if(fd == -1)
    {
        printf("File could not be opened\n");
        return -1;
    }

    write_file(fd);


    close(fd);

    //Reopen and test read functions
    fd = open(filename);
    if(fd == -1)
    {
        printf("Opening file failed when opening to read\n");
        return -1;
    }

    filesize_test(fd);

    regular_read_test(fd);

    seek_test(fd);

    edge_seek_test(fd);

    remove_test(fd, filename);
    
    printf("Single threaded tests done\n");

    if(total_success)
    {
        printf("All tests PASSED\n");
    }
}
