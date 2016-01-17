#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void do_sig(int n)
{
	static int i = 0;
	printf("i = %d\n", i);
	i += 2;
}

void do_sig2(int n)
{
	printf("signal = %d\n", n);
	static int j = 0;
	printf("j = %d", j);
	j += 2;
}

int main(void)
{
	pid_t pid;
	pid = fork();
	
	signal(SIGUSR2, do_sig);
	signal(SIGUSR1, do_sig2);
	if (pid == 0)
	{
		sleep(3);
		while (1)
		{
			if (kill(getppid(), SIGUSR2) < 0)
			{
				printf("child kill error\n");
				exit(1);
			}
			sleep(1);
		}

	}
	else if (pid > 0)
	{
		sleep(2);
		while (1)
		{
			if (kill(pid, SIGUSR1) < 0)
			{
				printf("parent kill error\n");
				exit(1);
			}
			sleep(2);
		}
	}
	return 0;
}
