#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#define BLOCK_SIZE 0x100000000

typedef struct
{
    char *readadr;
    char *writeadr;
    off_t size;
    off_t block_size;
    off_t write_size;
    int tid;
}Info;

void sys_err(char *str)
{
    perror(str);
    exit(1);
}

int CreateNewFile(char *srcfile, char *filename, off_t *size)
{
    struct stat buf;
    int fd;
    if (stat(srcfile, &buf) < 0)
    {
        sys_err("stat");
    }
	*size = buf.st_size;
    if ((fd = open(filename, O_RDWR | O_TRUNC | O_CREAT, buf.st_mode)) < 0)
    {
        sys_err("open");
    }
    if (lseek(fd, buf.st_size-1, SEEK_SET) < 0)
    {
        sys_err("lseek");
    }
    if (write(fd, "0", 1) < 0)
    {
        sys_err("write");
    }
    return fd;
}

void *th_func(void *arg)
{
    Info *info = (Info *)arg;
    char *rbeg = info->readadr + (info->tid * info->block_size);
    char *rend = info->readadr + ((info->tid+1) * info->block_size);
    char *wbeg = info->writeadr + (info->tid * info->block_size);
    char *fihish = info->readadr + info->size;
    if (rend > fihish)
    {
        memcpy(wbeg, rbeg, fihish - rbeg);
        info->write_size = fihish - rbeg;
    }
    else
    {
        memcpy(wbeg, rbeg, rend - rbeg);
        info->write_size = rend - rbeg;
    }
}

void CopyFile(char *srcfile, char *destfile, int threadnum)
{
    Info *info;
    int fd1, fd2, i, j;
    off_t srcfilesize, block_size, finishsize, block_num, mmap_size, copysize = 0;
    char *addr1, *addr2;
    pthread_t tid;
    fd1 = CreateNewFile(srcfile, destfile, &srcfilesize);
    if ( (fd2 = open(srcfile, O_RDONLY)) < 0)
    {
        sys_err("fd2 open");
    }
    info = (Info *)malloc(sizeof(Info)*threadnum);
    //利用mmap映射到内存
    block_num = srcfilesize / BLOCK_SIZE + 1;
    for (j = 0; j < block_num; ++j)
    {
        if (srcfilesize - copysize < BLOCK_SIZE)
        {
            mmap_size = srcfilesize - copysize;
        }
		else
		{
			mmap_size = BLOCK_SIZE;
		}
        if ((addr1 = (char *)mmap(NULL, mmap_size, PROT_READ, MAP_SHARED, fd2, copysize)) == MAP_FAILED)
        {
            sys_err("mmap1");
        }
        if ((addr2 = (char *)mmap(NULL, mmap_size, PROT_WRITE, MAP_SHARED, fd1, copysize)) == MAP_FAILED)
        {
            sys_err("mmap2");
        }

        block_size = mmap_size / threadnum + 1;
        //创建线程 把线程设置为分离态
        for (i = 0; i < threadnum; ++i)
        {
            info[i].readadr = addr1;
            info[i].writeadr = addr2;
            info[i].size = mmap_size;
            info[i].block_size = block_size;
            info[i].write_size = 0;
            info[i].tid = i;
            pthread_create(&tid, NULL, th_func, &info[i]);
            pthread_detach(tid);
        }
        //判断是否完成了块拷贝
        while (1)
        {
            finishsize = 0;
            for (i = 0; i < threadnum; ++i)
            {
                finishsize += info[i].write_size;
            }
            if (finishsize < mmap_size)
            {
			}
            else
            {
				copysize += mmap_size;
				if (munmap(addr1, mmap_size) < 0)
				{
					sys_err("munmap1");
				}
				if (munmap(addr2, mmap_size) < 0)
				{
					sys_err("munmap2");
				}
                break;
            }
            usleep(10);
        }
    }
    free(info);
    info = NULL;
}


int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("./mycp srcfile destfile threadnum(default 5)\n");
		exit(1);
    }
    if (argc == 4)
    {
		CopyFile(argv[1], argv[2], atoi(argv[3]));
    }
	else
	{
		CopyFile(argv[1], argv[2], 5);
	}
    printf("Successful copy file..\n");
    return 0;
}
