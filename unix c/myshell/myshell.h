typedef struct
{
	int cmd_num;	
	int type;		
	int mypipe[5];
	int mylredi[5];	
	int myrredi[5];
	int pipe_num;
	int lredi_num;
	int rredi_num;
	char *cmd_agr[10];
}cmd;
