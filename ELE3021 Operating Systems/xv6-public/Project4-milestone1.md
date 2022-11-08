# Expand the Maximum Size of a File

xv6 consists of 12 direct blocks and 1 indirect block. In order to implement a triple indirect block, a double indirect block must also be implemented together.

Therefore, xv6 is designed to have 10 direct blocks, 1 indirect block, 1 double indirect block, and 1 triple indirect block.



## file.h

``` c
// in-memory copy of an inode
struct inode {
  uint dev;           // Device number
  uint inum;          // Inode number
  int ref;            // Reference count
  struct sleeplock lock; // protects everything below here
  int valid;          // inode has been read from disk?

  short type;         // copy of disk inode
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[NDIRECT+3];  /* project 4 */
};
```

Since the value of `NDIRECT` decreased by 2, the value of `addrs` was changed accordingly.



## fs.h

``` c
// *-------------* PROJECT 04 *-------------*
#define NDIRECT 10
#define NINDIRECT (BSIZE / sizeof(uint))
#define NDINDIRECT (NINDIRECT * NINDIRECT)
#define NTINDIRECT (NDINDIRECT * NINDIRECT)
#define MAXFILE (NDIRECT + NINDIRECT + NDINDIRECT + NTINDIRECT)
// ******************************************

// On-disk inode structure
struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEV only)
  short minor;          // Minor device number (T_DEV only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT+3];   // Data block addresses /* project 4 */
};
```

Change `NDIRECT` which indicates the number of direct blocks to 10.

`NDINDIRECT` denotes the size of a double indirect block, and `NTINDIRECT` denotes the size of a triple indirect block.

The value of `MAXFILE` changed as double indirect block and triple indirect block were added.






## param.h

``` c
/* Project 4 */
#define FSSIZE       40000  // size of file system in blocks
```

It was revised as indicated in the project specification.



## fs.c

### bmap()

``` c
// *-------------* PROJECT 04 *-------------*
if(bn < NDINDIRECT)
{
  if((addr = ip->addrs[NDIRECT + 1]) == 0)
    ip->addrs[NDIRECT + 1] = addr = balloc(ip->dev);
  bp = bread(ip->dev, addr);
  a = (uint*)bp->data;
  if((addr = a[bn / NINDIRECT]) == 0){
    a[bn / NINDIRECT] = addr = balloc(ip->dev);
    log_write(bp);
  }
  brelse(bp);

  bp = bread(ip->dev, addr);
  a = (uint*)bp->data;
  if((addr = a[bn % NINDIRECT]) == 0){
    a[bn % NINDIRECT] = addr = balloc(ip->dev);
    log_write(bp);
  }
  brelse(bp);
  return addr;
}

if(bn < NTINDIRECT)
  {
  if((addr = ip->addrs[NDIRECT + 2]) == 0)
    ip->addrs[NDIRECT + 2] = addr = balloc(ip->dev);
  bp = bread(ip->dev, addr);
  a = (uint*)bp->data;
  if((addr = a[bn / NDINDIRECT]) == 0){
    a[bn / NDINDIRECT] = addr = balloc(ip->dev);
    log_write(bp);
  }
  brelse(bp);

  bp = bread(ip->dev, addr);
  a = (uint*)bp->data;
  if((addr = a[(bn % NDINDIRECT) / NINDIRECT]) == 0){
    a[(bn % NDINDIRECT) / NINDIRECT] = addr = balloc(ip->dev);
    log_write(bp);
  }
  brelse(bp);

  bp = bread(ip->dev, addr);
  a = (uint*)bp->data;
  if((addr = a[bn % NINDIRECT]) == 0){
    a[bn % NINDIRECT] = addr = balloc(ip->dev);
    log_write(bp);
  }
  brelse(bp);
  return addr;
}
// ******************************************
```

`bmap()` was modified to handle double indirect block and triple indirect block.

Since the double indirect block and the triple indirect block are sequentially located right next to the indirect block, the double indirect block is placed at `addrs[NDIRECT+1]` , and the triple indirect block is placed at `addrs[NDIRECT+2]`.

In order to divide the address into first data block and second data block, the double indirect block represents the first data block as `bn / NINDIRECT` and the second data block as `bn % NINDIRECT`.

The triple indirect block represents the first data block as `bn / NDINDIRECT` and the second data block as `(bn % NDINDIRECT) / NINDIRECT` and triple data block as `bn % NINDIRECT`.



### itrunc()

``` c
// *-------------* PROJECT 04 *-------------*
if(ip->addrs[NDIRECT + 1])
  itruncdt(ip, ip->addrs[NDIRECT + 1], 1);

if(ip->addrs[NDIRECT + 1])
  itruncdt(ip, ip->addrs[NDIRECT + 2], 2);
// ******************************************
```

In `itrunc()` the contents of double indirect block and triple indirect block were added.

I tried to implement it by referring to the case of the indirect block, but there were many overlapping parts in the function, so the case of double indirect block and the case of triple indirect block were separately implemented through `itruncdt()`.



### itruncdt()

```c
static void
itruncdt(struct inode* ip, uint addr, int dt)
{
  int i;
  struct buf *bp;
  uint *a;

  bp = bread(ip->dev, addr);
  a = (uint*)bp->data;

  for(i = 0; i < NINDIRECT; i++){
    if(a[i]){
      if(dt == 0)
        bfree(ip->dev, a[i]);
      else
        itruncdt(ip, a[i], dt - 1);
    }
  }

  brelse(bp);
  bfree(ip->dev, addr);
}
```

It is a function implemented for double-indirect block and triple indirect block. It was implemented using the case of the indirect block of `itrunc()`.

If `dt` is 1, it is a double indirect block, and if `dt` is 2, it is a triple indirect block.



# Test Program for Milestone 1

## test_file.c

The test code was written as follows to suit the assignment specification.

``` c
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
```
