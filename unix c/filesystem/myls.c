#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>

char mtoa(unsigned int mode)
{
	int i = 0;
	int num[5];
	int k = 5;
	char str[8][4] = {"---","--x","-w-","-wx","r--","r-x","rw-","rwx"};
    if (S_ISREG(mode))
    {
        printf("-");
    }
    else if (S_ISDIR(mode))
    {
        printf("d");
    }
    else if (S_ISCHR(mode))
    {
        printf("c");
    }
    else if (S_ISBLK(mode))
    {
        printf("b");
    }
    else if (S_ISFIFO(mode))
    {
        printf("p");
    }
    else if (S_ISLNK(mode))
    {
        printf("l");
    }
    else
    {
        printf("s");
    }
	while (mode)
	{
		num[--k] = mode % 8;
		mode /= 8;
	}
	for (i = 2; i < 5; ++i)
	{
		printf("%s", str[num[i]]);
	}
}

int main(int argc, char *argv[])
{
	struct stat buf;
    char timebuf[30];
	int rs;
    if (argc < 2)
    {
        printf("./myls filename");
    }
	rs = lstat(argv[1], &buf);
	if (rs == -1)
	{
		perror("stat error");
	}
	mtoa(buf.st_mode);
    ctime_r(&buf.st_mtime, timebuf);
    timebuf[strlen(timebuf) - 1] = '\0';
    printf(" %ld", buf.st_nlink);
    printf(" %s", getpwuid(buf.st_uid)->pw_name);
    printf(" %s", getgrgid(buf.st_gid)->gr_name);
    printf(" %ld", buf.st_size);
    printf(" %s %s\n", timebuf, argv[1]);
	return 0;
}
