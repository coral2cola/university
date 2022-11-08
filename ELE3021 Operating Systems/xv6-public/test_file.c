#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define FSIZE        (16*1024*1024)
#define BSIZE         512

int
main(int argc, char *argv[])
{
    int i, j; 
    int r, fd, size;
    char data[BSIZE];
    char buf[BSIZE];
    int cnt = FSIZE / BSIZE;
    char *path = (argc > 1) ? argv[1] : "File";

    const int sz = sizeof(data);
    for (i = 0; i < sz; i++)
    {
        data[i] = i % 128;
    }

    printf(1, "(1) Create test\n");
    fd = open(path, O_CREATE | O_RDWR);
    for(i = 0; i < cnt; i++)
    {
        if ((r = write(fd, data, sizeof(data))) != sizeof(data))
        {
            printf(1, "write returned %d : failed\n", r);
            exit();
        }
        if (i % 100 == 0)
        {
            printf(1, "%d bytes written in file\n", i * BSIZE);
        }
    }
    printf(1, ">> RESULT : %d bytes written in file! <<\n\n", cnt * BSIZE);
    close(fd);


    printf(1, "(2) Read test\n");
    fd = open(path, O_RDONLY);
    for (i = 0; i < cnt; i++){
        if ((r = read(fd, buf, sizeof(data))) != sizeof(data))
        {
            printf(1, "read returned %d : failed\n", r);
            exit();
        }
        if (i % 100 == 0)
        {
            printf(1, "%d bytes read from file\n", i * BSIZE);
        }
        for (j = 0; j < sz; j++) 
        {
            if (buf[j] != data[j]) 
            {
                printf(1, "data inconsistency detected\n");
                exit();
            }
        }
    }
    printf(1, ">> RESULT : %d bytes read from file! <<\n\n", cnt * BSIZE);
    close(fd);


    printf(1, "(3) Stress test\n");
    size = 0;
    for (i = 0; i < 4; i++) {
        printf(1, "Stress test %d \n", i + 1);
        if(unlink(path) < 0)
        {
            printf(1, "rm: %s failed to delete\n", path);
            exit();
        }

        fd = open(path, O_CREATE | O_RDWR);
        for(j = 0; j < cnt; j++)
        {
            if ((r = write(fd, data, sizeof(data))) != sizeof(data))
            {
                printf(1, "write returned %d : failed\n", r);
                exit();
            }
            if (j % 100 == 0)
            {
                printf(1, "%d bytes totally written\n", size);
            }
            size += sizeof(data);
        }
        printf(1, ">> RESULT : %d bytes written in file! <<\n\n", size);
        size = 0;
        close(fd);
    }

    exit();
}
