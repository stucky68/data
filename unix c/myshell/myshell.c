#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "myshell.h"

#define MAX_BUF 256

void sys_err(char *str, int exitno)
{
    perror(str);
    exit(exitno);
}

void get_cmd(char *buf)
{
    char ch;
    int len = 0;
    while((ch = getchar()) != '\n')
    {
        buf[len++] = ch;
    }
    buf[len++] = '\n';
    buf[len] = '\0';
}

int analysis_cmd(char *buf, char cmd_str[20][256])
{
    char *p = buf;
    char *q = buf;
    int num = 0, count = 0;
    while (1)
    {
        if (p[0] == '\n')
        {
            break;
        }
        if (p[0] == ' ')
        {
            p++;
        }
        else
        {
            q = p;
            num = 0;
            while (p[0] != ' ' && p[0] != '\n')
            {
                p++;
                num++;
            }
            strncpy(cmd_str[count], q, num+1);
            cmd_str[count][num] = '\0';
			count++;
            q = p;
        }
    }
	return count;
}

void get_type(char cmd_str[20][256], cmd *pcmd)
{
	int i = 0;
	for (i = 0; i < pcmd->cmd_num; ++i)
	{
		pcmd->cmd_agr[i] = cmd_str[i];
		if (strcmp(cmd_str[i], "<") == 0)
		{
			pcmd->mylredi[pcmd->lredi_num++] = i;
		}
		else if (strcmp(cmd_str[i], ">") == 0)
		{
			pcmd->myrredi[pcmd->rredi_num++] = i;
		}
		else if(strcmp(cmd_str[i], "|") == 0)
		{
			pcmd->mypipe[pcmd->pipe_num++] = i;
		}
	}
	pcmd->cmd_agr[i] = NULL;
	if (!pcmd->lredi_num && !pcmd->rredi_num && !pcmd->pipe_num)
	{
		pcmd->type = 0;
	}
	else if (pcmd->lredi_num && !pcmd->rredi_num && !pcmd->pipe_num)
 	{
		pcmd->type = 1;	
	}
	else if (pcmd->rredi_num && !pcmd->lredi_num && !pcmd->pipe_num)
	{
		pcmd->type = 2;
	}
	else if (pcmd->pipe_num && !pcmd->lredi_num && !pcmd->rredi_num)
	{
		pcmd->type = 3;
	}
	else if (pcmd->lredi_num && pcmd->pipe_num && !pcmd->rredi_num)
	{
		pcmd->type = 4;
	}
	else if (pcmd->rredi_num && pcmd->pipe_num && !pcmd->lredi_num)
	{
		pcmd->type = 5;
	}
	else 
	{
		pcmd->type = 6;
	}

}

void normal_cmd(cmd *pcmd)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		sys_err("normal_cmd fork", 1);
	}
	if (pid == 0)
	{
		// in child
		if (execvp(pcmd->cmd_agr[0], pcmd->cmd_agr) < 0)
		{
			sys_err("normal_cmd execvp", 1);
		}
		exit(1);
	}
	wait();
}

void lredi_cmd(cmd *pcmd)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		sys_err("lredi_cmd fork", 1);
	}
	if (pid == 0)
	{
		// in child
		int fd;
		char *filename = pcmd->cmd_agr[pcmd->mylredi[0] + 1];
		pcmd->cmd_agr[pcmd->mylredi[0]] = NULL;
		if ((fd = open(filename, O_RDONLY)) < 0)
		{
			sys_err("lredi_cmd open", 1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		if (execvp(pcmd->cmd_agr[0], pcmd->cmd_agr) < 0)
		{
			sys_err("lredi_cmd execvp", 1);
		}
		exit(1);
	}
	wait();
}

void rredi_cmd(cmd *pcmd)
{

	pid_t pid;
	if ((pid = fork()) < 0)
	{
		sys_err("rredi_cmd fork", 1);
	}
	if (pid == 0)
	{
		// in child
		int fd;
		char *filename = pcmd->cmd_agr[pcmd->myrredi[0] + 1];
		pcmd->cmd_agr[pcmd->myrredi[0]] = NULL;
		if ((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
		{
			sys_err("rredi_cmd open", 1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		if (execvp(pcmd->cmd_agr[0], pcmd->cmd_agr) < 0)
		{
			sys_err("rredi_cmd execvp", 1);
		}
		exit(1);
	}
	wait();
}

void pipe_cmd(cmd *pcmd)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		sys_err("pipe_cmd fork", 1);
	}
	if (pid == 0)
	{
		// in child
		int fd[2], index = pcmd->mypipe[0], j;
		char *temp[10];
		pid_t pid2;
		if (pipe(fd) < 0)
		{
			sys_err("pipe_cmd pipe", 1);
		}
		if ((pid2 = fork()) < 0)
		{
			sys_err("pipe_cmd fork2", 1);
		}
		if (pid2 == 0)
		{
			pcmd->cmd_agr[index] = NULL;
			// close read
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			if (execvp(pcmd->cmd_agr[0], pcmd->cmd_agr) < 0)
			{
				sys_err("pipe_r execvp", 1);
			}
		}
		wait();
		for (j = index+1; pcmd->cmd_agr[j] != NULL; ++j)
		{
			temp[j-index-1] = pcmd->cmd_agr[j];
		}
		temp[j-index-1] = pcmd->cmd_agr[j];
		// close write
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (execvp(temp[0], temp) < 0)
		{
			sys_err("pipe_w execvp", 1);
		}
		exit(1);
	}
	wait();
}

void lredi_pipe_cmd(cmd *pcmd)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		sys_err("lredi_pipe_cmd fork", 1);
	}
	if (pid == 0)
	{
		// in child
		int fd[2], index = pcmd->mypipe[0], j, filefd;
		char *temp[10];
		pid_t pid2;
		if (pipe(fd) < 0)
		{
			sys_err("lredi_pipe_cmd pipe", 1);
		}
		if ((pid2 = fork()) < 0)
		{
			sys_err("lredi_pipe_cmd fork2", 1);
		}
		if (pid2 == 0)
		{
			char *filename = pcmd->cmd_agr[pcmd->mylredi[0] + 1];
			pcmd->cmd_agr[pcmd->mylredi[0]] = NULL;
			// open file
			if ((filefd = open(filename, O_RDONLY)) < 0)
			{
				sys_err("lredi_pipe_open", 1);
			}
			// close read
			close(fd[0]);
			dup2(filefd, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			close(filefd);
			if (execvp(pcmd->cmd_agr[0], pcmd->cmd_agr) < 0)
			{
				sys_err("lredi_pipe_r execvp", 1);
			}
		}
		wait();
		for (j = index+1; pcmd->cmd_agr[j] != NULL; ++j)
		{
			temp[j-index-1] = pcmd->cmd_agr[j];
		}
		temp[j-index-1] = pcmd->cmd_agr[j];
		// close write
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (execvp(temp[0], temp) < 0)
		{
			sys_err("lredi_pipe_w execvp", 1);
		}
		exit(1);
	}
	wait();
}

void rredi_pipe_cmd(cmd *pcmd)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		sys_err("rredi_pipe_cmd fork", 1);
	}
	if (pid == 0)
	{
		// in child
		int fd[2], index = pcmd->mypipe[0], j, filefd;
		char *temp[10];
		pid_t pid2;
		if (pipe(fd) < 0)
		{
			sys_err("rredi_pipe_cmd pipe", 1);
		}
		if ((pid2 = fork()) < 0)
		{
			sys_err("rredi_pipe_cmd fork2", 1);
		}
		if (pid2 == 0)
		{
			pcmd->cmd_agr[index] = NULL;
			// close read
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			if (execvp(pcmd->cmd_agr[0], pcmd->cmd_agr) < 0)
			{
				sys_err("rredi_pipe_r execvp", 1);
			}
		}
		wait();
		char *filename = pcmd->cmd_agr[pcmd->myrredi[0] + 1];
		pcmd->cmd_agr[pcmd->myrredi[0]] = NULL;
		for (j = index+1; pcmd->cmd_agr[j] != NULL; ++j)
		{
			temp[j-index-1] = pcmd->cmd_agr[j];
		}
		temp[j-index-1] = pcmd->cmd_agr[j];
		if ((filefd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
		{
			sys_err("rredi_pipe_cmd open", 1);
		}
		// close write
		close(fd[1]);
		dup2(filefd, STDOUT_FILENO);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(filefd);
		if (execvp(temp[0], temp) < 0)
		{
			sys_err("rredi_pipe_w execvp", 1);
		}
		exit(1);
	}
	wait();
}

void lredi_rredi_pipe_cmd(cmd *pcmd)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		sys_err("lredi_rredi_pipe_cmd fork", 1);
	}
	if (pid == 0)
	{
		// in child
		int fd[2], index = pcmd->mypipe[0], j, filefd;
		char *temp[10];
		pid_t pid2;
		if (pipe(fd) < 0)
		{
			sys_err("lredi_rredi_pipe_cmd pipe", 1);
		}
		if ((pid2 = fork()) < 0)
		{
			sys_err("lredi_rredi_pipe_cmd fork2", 1);
		}
		if (pid2 == 0)
		{
			char *filename = pcmd->cmd_agr[pcmd->mylredi[0] + 1];
			pcmd->cmd_agr[pcmd->mylredi[0]] = NULL;
			// open file
			if ((filefd = open(filename, O_RDONLY)) < 0)
			{
				sys_err("lredi_pipe_open", 1);
			}
			// close read
			close(fd[0]);
			dup2(filefd, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			close(filefd);
			if (execvp(pcmd->cmd_agr[0], pcmd->cmd_agr) < 0)
			{
				sys_err("lredi_rredi_pipe_r execvp", 1);
			}
		}
		wait();
		char *filename = pcmd->cmd_agr[pcmd->myrredi[0] + 1];
		pcmd->cmd_agr[pcmd->myrredi[0]] = NULL;
		for (j = index+1; pcmd->cmd_agr[j] != NULL; ++j)
		{
			temp[j-index-1] = pcmd->cmd_agr[j];
		}
		temp[j-index-1] = pcmd->cmd_agr[j];
		if ((filefd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
		{
			sys_err("lredi_rredi_pipe_cmd open", 1);
		}
		// close write
		close(fd[1]);
		dup2(filefd, STDOUT_FILENO);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(filefd);
		if (execvp(temp[0], temp) < 0)
		{
			sys_err("lredi_rredi_pipe_w execvp", 1);
		}
		exit(1);
	}
	wait();
}

void do_cmd(char cmd_str[20][256], cmd *pcmd)
{
	get_type(cmd_str, pcmd);
	switch(pcmd->type)
	{
		case 0:
			normal_cmd(pcmd);
			break;
		case 1:
			lredi_cmd(pcmd);
			break;
		case 2:
			rredi_cmd(pcmd);
			break;
		case 3:
			pipe_cmd(pcmd);
			break;
		case 4:
			lredi_pipe_cmd(pcmd);
			break;
		case 5:
			rredi_pipe_cmd(pcmd);
			break;
		case 6:
			lredi_rredi_pipe_cmd(pcmd);
			break;
	}
}

void init_cmd(cmd *pcmd)
{
	pcmd->cmd_num = 0;
	pcmd->type = 0;
	pcmd->pipe_num = 0;
	pcmd->lredi_num = 0;
	pcmd->rredi_num = 0;
	memset(pcmd->mypipe, 0, 5);
	memset(pcmd->mylredi, 0, 5);
	memset(pcmd->myrredi, 0, 5);
	memset(pcmd->cmd_agr, 0, 10);
}

int main(void)
{
    char buf[256];
    char cmd_str[20][256];
	cmd pcmd;
    while (1)
    {
        printf("mysh$ ");
		init_cmd(&pcmd);
		get_cmd(buf);
		pcmd.cmd_num = analysis_cmd(buf, cmd_str);
		do_cmd(cmd_str, &pcmd);
    }
    return 0;
}
